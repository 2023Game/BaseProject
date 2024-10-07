#pragma once
#include "CRideableObject.h"
#include "CModel.h"
#include "CColliderMesh.h"

class CMoveFloor : public CRideableObject
{
public:
	CMoveFloor(CModel* model, const CVector& pos, const CVector& scale, const CVector& move, float moveTime);
	~CMoveFloor();

	void Update();
	void Render();

private:
	CModel* mpModel;
	CColliderMesh* mpColliderMesh;

	CVector mDefaultPos;
	CVector mMoveVec;
	float mMoveTime;
	float mElapsedTime;
};