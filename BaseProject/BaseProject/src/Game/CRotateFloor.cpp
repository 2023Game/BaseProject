#include "CRotateFloor.h"
#include "Maths.h"

#define ROTATE_Y 1.0f

CRotateFloor::CRotateFloor(CModel* model, const CVector& pos, const CVector& scale, float rotateSpeedY)
	: mpModel(model)
	, mRotateSpeedY(rotateSpeedY)
{
	mpColliderMesh = new CColliderMesh(this, ELayer::eField, mpModel, true);

	Position(pos);
	Scale(scale);
}

CRotateFloor::~CRotateFloor()
{
	if (mpColliderMesh != nullptr)
	{
		delete mpColliderMesh;
		mpColliderMesh = nullptr;
	}
}

void CRotateFloor::Update()
{
	Rotate(0.0f, mRotateSpeedY, 0.0f);
}

void CRotateFloor::Render()
{
	mpModel->Render(Matrix());
}
