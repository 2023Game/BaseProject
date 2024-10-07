#pragma once
#include "CBillBoardImage.h"
#include "CColliderSphere.h"

// ���̃G�t�F�N�g
class CFlame : public CBillBoardImage
{
public:
	// �R���X�g���N�^
	CFlame(ETag tag);
	// �f�X�g���N�^
	~CFlame();

	/// <summary>
	/// �e�p�����[�^��ݒ�
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	/// <param name="dir">�ړ�����</param>
	/// <param name="speed">�ړ����x</param>
	void Setup(const CVector& pos, const CVector& dir, float speed);

	// �J���[��ݒ�
	void SetColor(const CColor& color) override;
	// �u�����h�^�C�v��ݒ�
	void SetBlendType(EBlend type);

	// �폜�t���O�������Ă��邩�ǂ���
	bool IsDeath() const;

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �X�V
	void Update() override;

private:
	// �A�j���[�V�����f�[�^
	static TexAnimData msAnimData;
	CVector mMoveSpeed;	// �ړ����x
	float mElapsedTime;	// �o�ߎ���
	bool mIsDeath;		// �폜�t���O
	CColliderSphere* mpCollider;
};
