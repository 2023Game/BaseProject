#include "CCollisionTestScene.h"
#include "CColSphere.h"
#include "CColLine.h"
#include "CColTriangle.h"
#include "CColQuad.h"
#include "CColCapsule.h"
#include "CColBox.h"
#include "CCamera.h"
#include "CMeasure.h"
#include "CDebugInput.h"

// コンストラクタ
CCollisionTestScene::CCollisionTestScene()
	: CSceneBase(EScene::eColTest)
	, mpSelectCol(nullptr)
{
}

// デストラクタ
CCollisionTestScene::~CCollisionTestScene()
{
}

// シーン読み込み
void CCollisionTestScene::Load()
{
	// 背景色設定
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	// 床の板ポリゴン生成
	new CColQuad
	(
		CVector2(200.0f, 200.0f),
		CColor::white,
		true
	);

	// 測定用の表示物を生成
	new CMeasure(100);

	// 球を生成
	//CColSphere* cs = new CColSphere(10.0f, CColor::gray);
	//cs->Scale(2.0f, 2.0f, 2.0f);
	//cs->Position(10.0f, 10.0f, 0.0f);
	//mColList.push_back(cs);

	//CColSphere* cs2 = new CColSphere(1.0f, CColor::gray);
	//cs2->Scale(5.0f, 5.0f, 5.0f);
	//cs2->Position(-10.0f, 10.0f, 0.0f);
	//mColList.push_back(cs2);

	//CColSphere* cs = new CColSphere
	//(
	//	2.0f,
	//	CColor::gray
	//);
	//mColList.push_back(cs);
	//cs->Position(0.0f, 10.0f, 0.0f);

	CColBox* cb = new CColBox
	(
		CVector(2.0f, 1.0f, 3.0f),
		CColor::gray
	);
	mColList.push_back(cb);
	cb->Position(10.0f, 20.0f, 0.0f);
	cb->Scale(2.0f, 2.0f, 2.0f);

	//CColBox* cb2 = new CColBox
	//(
	//	CVector(1.5f, 3.0f, 1.2f),
	//	CColor::gray
	//);
	//mColList.push_back(cb2);
	//cb2->Position(-10.0f, 10.0f, 0.0f);
	//cb2->Scale(1.0f, 1.0f, 1.0f);

	CColTriangle* tri = new CColTriangle
	(
		CVector(-10.0f, 10.0f,  10.0f),
		CVector( 10.0f, 10.0f,  10.0f),
		CVector(  0.0f, 10.0f, -10.0f),
		CColor::gray
	);
	mColList.push_back(tri);
	tri->Position(-10.0f, 10.0f, 0.0f);

	// 線分を生成
	//CColLine* cl = new CColLine
	//(
	//	CVector(0.0f, 11.0f, 0.0f),
	//	CVector(1.1f, 1.0f, 2.0f),
	//	CColor::gray
	//);
	//mColList.push_back(cl);

	// カプセルを生成
	CColCapsule* cc = new CColCapsule
	(
		CVector(-5.0f, 21.0f, 5.0f),
		CVector(-5.0f, 11.0f, 5.0f),
		2.0f,
		CColor::gray
	);
	mColList.push_back(cc);

	//CColCapsule* cc2 = new CColCapsule
	//(
	//	CVector(-10.0f, 15.0f, 2.0f),
	//	CVector(-10.0f, 6.0f, -1.0f),
	//	5.0f,
	//	CColor::gray
	//);
	//mColList.push_back(cc2);

	//mpColTriangle = new CColTriangle
	//(
	//	CVector(-100.0f, -10.0f,  100.0f),
	//	CVector( 100.0f, -10.0f,  100.0f),
	//	CVector(   0.0f, -10.0f, -100.0f)
	//);

	// カメラ生成
	CCamera* mainCamera = new CCamera
	(
		CVector(0.0f, 50.0f, 75.0f),
		CVector::zero
	);
}

// コライダーを選択
void CCollisionTestScene::Select(CColBase* col)
{
	// 選択中のコライダーの選択解除
	if (mpSelectCol != nullptr)
	{
		mpSelectCol->SetSelect(false);
	}

	// 選択中のコライダーが再度指定された場合は
	// 選択解除する
	if (mpSelectCol == col)
	{
		mpSelectCol = nullptr;
	}
	// 選択中のコライダー以外
	else
	{
		mpSelectCol = col;

		// 指定されたコライダーを選択
		if (mpSelectCol != nullptr)
		{
			mpSelectCol->SetSelect(true);
		}
	}
}

// 更新
void CCollisionTestScene::Update()
{
#if _DEBUG
	int size = mColList.size();
	for (int i = 0; i < size; i++)
	{
		if (CDebugInput::PushKey('1' + i))
		{
			Select(mColList[i]);
		}
	}
#endif
}
