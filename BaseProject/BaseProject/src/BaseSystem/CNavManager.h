#pragma once
#include <vector>
#include "CTask.h"

class CNavNode;

// �o�H�T�������̊Ǘ��N���X
class CNavManager : public CTask
{
public:
	// �o�H�Ǘ��N���X�̃C���X�^���X���擾
	static CNavManager* Instance();

	// �R���X�g���N�^
	CNavManager();
	// �f�X�g���N�^
	~CNavManager();

	// �o�H�P�T�b�N�p�̃m�[�h��ǉ�
	void AddNode(CNavNode* node);
	// �o�H�T���p�̃m�[�h����菜��
	void RemoveNode(CNavNode* node);

	/// <summary>
	///	�w�肵���m�[�h�ɐڑ��ł���m�[�h���������Đݒ�
	/// (�ԂɎՕ���������m�[�h�ɂ͐ڑ��ł��Ȃ�)
	/// </summary>
	/// <param name="node">�ڑ��ł���m�[�h����������m�[�h</param>
	/// <param name="distance">�ڑ��ł��鋗���̌��E�l</param>
	/// <returns>�ڑ������m�[�h�̐�</returns>
	int FindConnectNavNodes(CNavNode* node, float distance);

	/// <summary>
	/// �w�肵���J�n�m�[�h����ړI�n�m�[�h�܂ł̍ŒZ�o�H�����߂�
	/// </summary>
	/// <param name="start">�J�n�m�[�h</param>
	/// <param name="goal">�ړI�n�m�[�h</param>
	/// <param name="route">�ŒZ�o�H�ԋp�p</param>
	/// <returns>�o�H���q�����Ă���ꍇ�́Atrue��Ԃ�</returns>
	bool Navigate(CNavNode* start, CNavNode* goal, std::vector<CNavNode*>& route);

	/// <summary>
	/// �Օ����`�F�b�N�Ɏg�p����R���C�_�[��ǉ�
	/// </summary>
	/// <param name="col"></param>
	void AddCollider(CCollider* col);
	/// <summary>
	/// �Օ����`�F�b�N�Ɏg�p����R���C�_�[����菜��
	/// </summary>
	/// <param name="col"></param>
	void RemoveCollider(CCollider* col);

	// �S�Ẵm�[�h�ƌo�H��`��
	void Render() override;

private:
	// �ŒZ�o�H�v�Z�p�̃f�[�^�����Z�b�g
	void ResetCalcData();
	/// <summary>
	/// �w�肵���m�[�h���玟�̃m�[�h�ւ̈ړ��R�X�g���v�Z
	/// </summary>
	/// <param name="node">�ړ��R�X�g���v�Z����J�n�m�[�h</param>
	/// <param name="goal">�ړI�n�̃m�[�h</param>
	void CalcNextMoveCost(CNavNode* node, CNavNode* goal);

	// �o�H�Ǘ��N���X�̃C���X�^���X�ւ̃|�C���^
	static CNavManager* spInstance;

	// �o�H�T���p�̃m�[�h�̃��X�g
	std::vector<CNavNode*> mNodes;

	// �o�H�T���̃f�o�b�O�\�����s�����ǂ���
	bool mIsRender;
	// �Ō�Ɍv�Z�����ŒZ�o�H�̃f�o�b�O�\���p
	std::vector<CNavNode*> mLastCalcRoute;

	// �Օ����`�F�b�N�Ɏg�p����R���C�_�[�̃��X�g
	std::vector<CCollider*> mColliders;
};