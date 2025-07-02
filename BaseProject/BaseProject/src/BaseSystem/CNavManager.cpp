#include "CNavManager.h"
#include "CNavNode.h"
#include "CInput.h"
#include "Primitive.h"
#include <assert.h>

CNavManager* CNavManager::spInstance = nullptr;

// �o�H�Ǘ��N���X�̃C���X�^���X���擾
CNavManager* CNavManager::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CNavManager::CNavManager()
	: CTask(ETaskPriority::eNavigation, 0, ETaskPauseType::eGame)
	, mIsRender(false)
{
	assert(spInstance == nullptr);
	spInstance = this;
}

// �f�X�g���N�^
CNavManager::~CNavManager()
{
	spInstance = nullptr;

	auto itr = mNodes.begin();
	auto end = mNodes.end();
	while (itr != end)
	{
		CNavNode* del = *itr;
		itr++;
		delete del;
	}
}

// �o�H�P�T�b�N�p�̃m�[�h��ǉ�
void CNavManager::AddNode(CNavNode* node)
{
	mNodes.push_back(node);
}

// �o�H�T���p�̃m�[�h����菜��
void CNavManager::RemoveNode(CNavNode* node)
{
	auto result = std::remove(mNodes.begin(), mNodes.end(), node);
	mNodes.erase(result, mNodes.end());
}

// �w�肵���m�[�h�ɐڑ��ł���m�[�h���������Đݒ�
int CNavManager::FindConnectNavNodes(CNavNode* node, float distance)
{
	// ���݂̐ڑ���̏���S�ăN���A���Ă���
	node->ClearConnects();

	for (CNavNode* findNode : mNodes)
	{
		//�������g�ł���΁A�X���[
		if (findNode == node) continue;
		// �����I�ɐڑ�����m�[�h�ł���΁A�X���[
		if (node->IsForcedConnectNode(findNode)) continue;
		// �ڑ����Ȃ��m�[�h�ł���΁A�X���[
		if (node->IsBlockedNode(findNode)) continue;

		// �ړI�n��p�m�[�h�͋������l�����Ȃ�
		if (!node->mIsDestNode)
		{
			// �w�肳�ꂽ�����̌��E�l�𒴂���ꍇ�́A�X���[
			float dist = (findNode->GetPos() - node->GetPos()).LengthSqr();
			if (dist > distance * distance) continue;
		}

		// ���g����ڑ���̃m�[�h�܂ł̎Օ����`�F�b�N
		CVector start = node->GetOffsetPos();
		CVector end = findNode->GetOffsetPos();
		CHitInfo hit;
		bool isHit = false;
		// �o�^����Ă���R���C�_�[�S�ĂƔ���
		for (CCollider* col : mColliders)
		{
			// �q�b�g���Ă�����A�q�b�g�t���O��true�ɂ��ă`�F�b�N�I��
			if (CCollider::CollisionRay(col, start, end, &hit))
			{
				isHit = true;
				break;
			}
		}
		// �����Ƀq�b�g�����ꍇ�́A�Օ���������̂Őڑ��ł��Ȃ�
		if (isHit) continue;;

		// �����̏����𖞂������m�[�h��ڑ����X�g�ɒǉ�
		node->AddConnect(findNode);

	}
	return node->mConnectData.size();
}

// �ŒZ�o�H�v�Z�p�̃f�[�^�����Z�b�g
void CNavManager::ResetCalcData()
{
	for (CNavNode* node : mNodes)
	{
		node->ResetCalcData();
	}
}

// �ŒZ�o�H�v�Z�p�̃f�[�^�����Z�b�g
void CNavManager::CalcNextMoveCost(CNavNode* node, CNavNode* goal)
{
	// �w�肳�ꂽ�m�[�h�܂��͖ړI�n�m�[�h���󂾂����ꍇ�́A�ړ��R�X�g�v�Z�s��
	if (node == nullptr || goal == nullptr) return;

	// �ڑ����Ă���S�Ẵm�[�h�ւ̈ړ��R�X�g�����߂�
	for (CNavConnectData& connect : node->mConnectData)
	{
		// �ڑ���̃m�[�h�������ł���΁A�X���[
		if (!connect.node->IsEnable()) continue;
		// �ڑ���������Ă����Ԃł���΁A�X���[
		if (!connect.enabled) continue;

		// �ڑ����Ă���m�[�h���ړI�n��p�m�[�h�̏ꍇ�́A
		// ����̌o�H�T���̖ړI�n�m�[�h�ȊO�͌o�R���Ȃ����߁A�X���[
		if (connect.node->mIsDestNode && connect.node != goal) continue;

		// �ړ��R�X�g = �����܂ł̈ړ��R�X�g + �ڑ���̃m�[�h�܂ł̈ړ��R�X�g
		float cost = node->mCalcMoveCost + connect.cost;
		// �ڑ���̃m�[�h�܂ł̈ړ��R�X�g���܂��v�Z����Ă��Ȃ����A
		// ���߂��ړ��R�X�g�̕������݂̐ڑ���̃m�[�h�܂ł̈ړ��R�X�g��菬�����ꍇ
		if (connect.node->mCalcMoveCost < 0 || cost < connect.node->mCalcMoveCost)
		{
			// �ڑ���̃m�[�h�̈ړ��R�X�g��1�O�̃m�[�h�̃|�C���^���X�V
			connect.node->mCalcMoveCost = cost;
			connect.node->mpCalcFromNode = node;
			// �ڑ���̃m�[�h���ړI�n�̃m�[�h�łȂ���΁A
			// �ڑ���̃m�[�h����ڑ�����Ă���m�[�h�ւ̈ړ��R�X�g���v�Z����
			if (connect.node != goal)
			{
				CalcNextMoveCost(connect.node, goal);
			}
		}
	}
}

// �w�肵���J�n�m�[�h����ړI�n�m�[�h�܂ł̍ŒZ�o�H�����߂�
bool CNavManager::Navigate(CNavNode* start, CNavNode* goal, std::vector<CNavNode*>& route)
{
	// �J�n�m�[�h�܂��͖ړI�n�m�[�h���󂾂����ꍇ�́A�o�H�T���s��
	if (start == nullptr || goal == nullptr)return false;
	// �J�n�m�[�h�܂��͖ړI�n�m�[�h�������������ꍇ�́A�o�H�T���s��
	if (!start->IsEnable() || !goal->IsEnable()) return false;

	// �S�Ẵm�[�h�̍ŒZ�o�H�v�Z�p�̃f�[�^���N���A
	ResetCalcData();

	// �J�n�m�[�h�ֈړ��R�X�g��0�ɐݒ�
	start->mCalcMoveCost = 0.0f;
	// �J�n�m�[�h���珇�ԂɁA
	// �e�m�[�h���ڑ����Ă���m�[�h�܂ł̈ړ��R�X�g���v�Z
	CalcNextMoveCost(start, goal);

	// �ŒZ�o�H�̃��X�g���N���A���čŏ��ɖړI�n�m�[�h��o�^
	route.clear();
	route.push_back(goal);

	// �ړI�n�m�[�h���珇�ԂɁA
	// �ŒZ�o�H�ł�1�O�̃m�[�h�����݂�����胋�[�v���āA
	// �ŒZ�o�H�̃��X�g�Ƀm�[�h��o�^���Ă���
	CNavNode* node = goal;
	while (node->mpCalcFromNode != nullptr)
	{
		route.insert(route.begin(), node->mpCalcFromNode);
		node = node->mpCalcFromNode;
	}

	// ���߂��ŒZ�o�H���f�o�b�O�\���p�̍ŒZ�o�H���X�g�ɃR�s�[
	mLastCalcRoute = route;

	// ���ߍŒZ�o�H��1�o���ŏ��̃m�[�h���J�n�m�[�h�ł���΁A
	// �J�n�m�[�h����ړI�n�m�[�h�܂Ōo�H���q�����Ă���
	return route[0] == start;
}

// �w�肵���o�H���q�����Ă��邩�ǂ���
bool CNavManager::IsRouteValid(const std::vector<CNavNode*>& route) const
{
	// �w�肵���o�H��2�ȏ�m�[�h���o�^����Ă��Ȃ���
	// �o�H�ƌ��Ȃ��Ȃ����߁A�o�H�͌q�����Ă��Ȃ��Ɣ��肷��
	int size = route.size();
	if (size <= 1) return false;

	// �w�肵���o�H�̃m�[�h��擪���珇�ԂɌq�����Ă��邩�m�F
	for (int i = 0; i < size - 1; i++)
	{
		// ���݂̃m�[�h�Ǝ��̃m�[�h���q�����Ă��Ȃ���΁A�o�H���q�����Ă��Ȃ�
		if (!route[i]->IsConnectNode(route[i + 1])) return false;
	}

	// �o�H���q�����Ă���
	return true;
}

// �Օ����`�F�b�N�Ɏg�p����R���C�_�[��ǉ�
void CNavManager::AddCollider(CCollider* col)
{
	mColliders.push_back(col);
}

// �Օ����`�F�b�N�Ɏg�p����R���C�_�[����菜��
void CNavManager::RemoveCollider(CCollider* col)
{
	auto result = std::remove(mColliders.begin(), mColliders.end(), col);
	mColliders.erase(result, mColliders.end());
}

// �S�Ẵm�[�h�ƌo�H��`��
void CNavManager::Render()
{
#if _DEBUG
	// [SPACE]�L�[�Ōo�H�T���m�[�h�̕`�惂�[�h��؂�ւ�
	if (CInput::PushKey('N'))
	{
		mIsRender = !mIsRender;
	}

	// �o�H�T���m�[�h��`�悵�Ȃ��Ȃ�΁A�ڍs�������Ȃ�
	if (!mIsRender) return;

	// ���X�g���̃m�[�h�S�ĕ`��
	for (CNavNode* node : mNodes)
	{
		node->Render();
	}

	// �Ō�ɋ��߂��ŒZ�o�H�Ƀ��C��������
	glDisable(GL_DEPTH_TEST);
	int size = mLastCalcRoute.size();
	for (int i = 0; i < size - 1; i++)
	{
		CVector start = mLastCalcRoute[i]->GetOffsetPos();
		CVector end = mLastCalcRoute[i + 1]->GetOffsetPos();
		Primitive::DrawLine(start, end, CColor::cyan, 4.0f);
	}
	glEnable(GL_DEPTH_TEST);
#endif
}

