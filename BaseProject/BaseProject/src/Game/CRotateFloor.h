#pragma once
#include "CRideableObject.h"
#include "CModel.h"
#include "CColliderMesh.h"

class CRotateFloor : public CRideableObject
{
public:
	CRotateFloor(CModel* model, const CVector& pos, const CVector& scale, float rotateSpeedY);
	~CRotateFloor();

	void Update();
	void Render();

private:
	CModel* mpModel;
	CColliderMesh* mpColliderMesh;

	float mRotateSpeedY;
};