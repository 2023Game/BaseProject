#include "CMeasure.h"
#include "Primitive.h"
#include <glut.h>

// コンストラクタ
CMeasure::CMeasure(int gridCount)
	: CObjectBase(ETag::eNone, ETaskPriority::eDefault)
{
	SetGridCount(gridCount);
}

// デストラクタ
CMeasure::~CMeasure()
{
}

// 表示するグリッド数を設定
void CMeasure::SetGridCount(int count)
{
	mGridCount = std::max(count, 2);
}

// 更新
void CMeasure::Update()
{
}

// 描画
void CMeasure::Render()
{
	glDisable(GL_DEPTH_TEST);

	// グリッドを表示
	CColor gridColor = CColor(0.75f, 0.75f, 0.75f, 1.0f);
	float gridWidth = 1.0f;
	float gridMin = -gridWidth * mGridCount;
	float gridMax = gridWidth * mGridCount;
	float gridLineWidth = 1.0f;
	for (int i = -mGridCount; i <= mGridCount; i++)
	{
		Primitive::DrawLine
		(
			CVector(gridMin, 0.0f, i * gridWidth),
			CVector(gridMax, 0.0f, i * gridWidth),
			gridColor, gridLineWidth
		);
		Primitive::DrawLine
		(
			CVector(i * gridWidth, 0.0f, gridMin),
			CVector(i * gridWidth, 0.0f, gridMax),
			gridColor, gridLineWidth
		);
	}

	// 世界の軸を表示
	float axisLength = 100.0f;
	float axisLineWidth = 2.0f;
	Primitive::DrawLine(CVector::zero, -CVector::right * axisLength, CColor::red, axisLineWidth);
	Primitive::DrawLine(CVector::zero, CVector::up * axisLength, CColor::green, axisLineWidth);
	Primitive::DrawLine(CVector::zero, CVector::forward * axisLength, CColor::blue, axisLineWidth);

	glEnable(GL_DEPTH_TEST);
}
