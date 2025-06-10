#include "CNavNode.h"
#include "CNavManager.h"
#include "Primitive.h"

// �m�[�h��Y���W�̃I�t�Z�b�g�l
#define NODE_OFFSET_Y 5.0f
// �T���m�[�h�̋����̌��E�l
#define FIND_NODE_DISTANCE 180.0f

// �R���X�g���N�^
CNavNode::CNavNode(const CVector& pos, bool isDestNode)
	: mIsEnable(true)
	, mIsDestNode(isDestNode)
	, mPosition(pos)
	, mCalcMoveCost(-1.0f)
	, mpCalcFromNode(nullptr)
	, mColor(0.0f, 1.0f, 0.0f, 1.0f)
{
	// �Ǘ��N���X�̃��X�g�Ɏ��g��ǉ�
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		navMgr->AddNode(this);
	}

	// ���W��ݒ�
	SetPos(mPosition);
}

// �f�X�g���N�^
CNavNode::~CNavNode()
{
	// �T���m�[�h���폜����O�ɁA���g�Ɛڑ����Ă���m�[�h����菜��
	ClearConnects();

	// �Ǘ��N���X�̃��X�g���玩�g����菜��
	// �Ǘ��N���X�̃��X�g�Ɏ��g��ǉ�
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		navMgr->RemoveNode(this);
	}
}

// �L����Ԃ�ݒ�
void CNavNode::SetEnable(bool enable)
{
	mIsEnable = enable;
}

// ���ݗL�����ǂ���
bool CNavNode::IsEnabel() const
{
	return mIsEnable;
}

// �ŒZ�o�H�v�Z�p�̃f�[�^�����Z�b�g
void CNavNode::ResetCalcData()
{
	mCalcMoveCost = -1.0;
	mpCalcFromNode = nullptr;
}


// �m�[�h�̍��W���擾
const CVector& CNavNode::GetPos() const
{
	return mPosition;
}

// �Օ����`�F�b�N��\���p�ɁA������ɏグ���m�[�h�̍��W��Ԃ�
CVector CNavNode::GetOffsetPos() const
{
	// �n�ʂƏd�Ȃ�Ȃ��悤�ɁA�m�[�h�̈ʒu���班���グ���ʒu��Ԃ�
	return mPosition + CVector(0.0f, NODE_OFFSET_Y, 0.0f);
}

// �m�[�h�̍��W��ݒ�
void CNavNode::SetPos(const CVector& pos)
{
	// �m�[�h�̍��W���X�V
	mPosition = pos;

	// ���W��ύX�����̂ŁA�ڑ��m�[�h���Č���
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		navMgr->FindConnectNavNodes(this, FIND_NODE_DISTANCE);
	}
}

// �ڑ�����m�[�h�ǉ�
void CNavNode::AddConnect(CNavNode* node)
{
	for (CNavConnectData& connect : mConnectData)
	{
		if (connect.node == node) return;
	}

	// �ڑ�����m�[�h�܂ł̋������R�X�g�Ƃ���
	float cost = (node->GetPos() - mPosition).Length();

	// ���g�Ƒ��肻�ꂼ��̐ڑ����Ă���m�[�h���X�g�ɂ��݂���ݒ�
	mConnectData.push_back(CNavConnectData(node, cost));
	node->mConnectData.push_back(CNavConnectData(this, cost));
}

// �ڑ����Ă���m�[�h����菜��
void CNavNode::RemoveConnect(CNavNode* node)
{
	auto result = std::remove_if
	(
		mConnectData.begin(), mConnectData.end(),
		[node](const CNavConnectData& x) { return x.node == node; }
	);
	mConnectData.erase(result, mConnectData.end());
}

// �ڑ����Ă���S�Ẵm�[�h������
void CNavNode::ClearConnects()
{
	// �ڑ�����̐ڑ����X�g���玩�g����菜��
	for (CNavConnectData& connect : mConnectData)
	{
		connect.node->RemoveConnect(this);
	}
	// ���g�̐ڑ����X�g���N���A
	mConnectData.clear();
}

// �m�[�h�̐F�ݒ�i�f�o�b�O�p�j
void CNavNode::SetColor(const CColor& color)
{
	mColor.Set(color.R(), color.G(), color.B());
}

// �m�[�h��`��i�f�o�b�O�p�j
void CNavNode::Render()
{
	// �m�[�h���L����ԏo�Ȃ���΁A�`�悵�Ȃ�
	if (!mIsEnable) return;

	// �ڑ���̃m�[�h�܂ł̃��C����`��
	for (CNavConnectData& connect : mConnectData)
	{
		// �ڑ���̃m�[�h�������ł���΁A�X���[
		if (!connect.node->IsEnabel()) continue;

		Primitive::DrawLine
		(
			GetOffsetPos(),
			connect.node->GetOffsetPos(),
			CColor(0.11f, 0.1f, 0.1f, 1.0f),
			2.0f
		);
	}

	// �m�[�h���W�Ɏl�p�`��`��
	Primitive::DrawWireBox
	(
		GetOffsetPos(),
		CVector::one,
		mColor
	);
}

