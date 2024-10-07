#ifndef CCOLLIDERMESH_H
#define CCOLLIDERMESH_H
#include <list>
#include "CModel.h"
#include "CCollider.h"

/*
���f���f�[�^����O�p�R���C�_�̐���
*/
class CColliderMesh : public CCollider
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner">�R���C�_�[�̎�����</param>
	/// <param name="layer">�Փ˔���p���C���[</param>
	/// <param name="model">���f���f�[�^</param>
	/// <param name="isKinematic">true�Ȃ�΁A�Փˎ��ɉ����߂��̉e�����󂯂Ȃ�</param>
	/// <param name="weight">�R���C�_�[�̏d��</param>
	CColliderMesh(CObjectBase* owner, ELayer layer, CModel* model,
		bool isKinematic = false, float weight = 1.0f);
	// �f�X�g���N�^
	~CColliderMesh();

	void Set(CModel* model);
	const std::list<STVertexData>& Get() const;

	// �R���C�_�[�`��
	void Render() override;

protected:
	// �R���C�_�[�̏����X�V
	void UpdateCol() override;

private:
	//�O�p�R���C�_�̔z��쐬
	std::list<STVertexData> mVertices;
};

#endif
