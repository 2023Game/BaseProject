#include "CColliderMesh.h"
#include "Primitive.h"

CColliderMesh::CColliderMesh(CObjectBase* owner, ELayer layer, CModel* model,
	bool isKinematic, float weight, int divX, int divY, int divZ)
	: CCollider(owner, layer, EColliderType::eMesh, isKinematic, weight)
	, mDivX(std::max(divX, 1))
	, mDivY(std::max(divY, 1))
	, mDivZ(std::max(divZ, 1))
{
	// メッシュコライダーはデフォルトで非表示
	SetShow(false);

	Set(model);
	UpdateCol(true);
}

CColliderMesh::~CColliderMesh()
{
	mVertices.clear();
}

void CColliderMesh::Set(CModel* model)
{
	mVertices.clear();
	mDivMesh.clear();
	if (model == nullptr) return;

	CBounds bounds;
	auto triangles = model->Triangles();
	int count = triangles.size();
	mVertices.reserve(count);
	for (int i = 0; i < count; i++)
	{
		auto& tri = triangles[i];
		STVertex v(tri.V0(), tri.V1(), tri.V2());
		mVertices.push_back({ v, v });

		if (i == 0)
		{
			bounds = CBounds::GetTriangleBounds
			(
				v.V[0], v.V[1], v.V[2]
			);
		}
		else
		{
			CVector min = bounds.Min();
			CVector max = bounds.Max();
			for (int i = 0; i < 3; i++)
			{
				min = CVector::Min(min, v.V[i]);
				max = CVector::Max(max, v.V[i]);
			}
			bounds.SetRange(min, max);
		}
	}

	int divCnt = mDivX * mDivY * mDivZ;
	if (divCnt > 1)
	{
		mDivMesh.reserve(divCnt);
		CVector start = bounds.Min();
		CVector size = bounds.Size();
		CVector min, max;
		for (int x = 0; x < mDivX; x++)
		{
			min.X(start.X() + size.X() * ((float)x / mDivX));
			max.X(start.X() + size.X() * ((float)(x + 1) / mDivX));
			for (int y = 0; y < mDivY; y++)
			{
				min.Y(start.Y() + size.Y() * ((float)y / mDivY));
				max.Y(start.Y() + size.Y() * ((float)(y + 1) / mDivY));
				for (int z = 0; z < mDivZ; z++)
				{
					min.Z(start.Z() + size.Z() * ((float)z / mDivZ));
					max.Z(start.Z() + size.Z() * ((float)(z + 1) / mDivZ));

					int idx = x * mDivY * mDivZ + y * mDivZ + z;
					STDivMesh dm;
					dm.bounds.SetRange(min, max);
					mDivMesh.push_back(dm);
				}
			}
		}
		for (STVertexData& v : mVertices)
		{
			CBounds bounds = CBounds::GetTriangleBounds(v.lv.V[0], v.lv.V[1], v.lv.V[2]);
			for (STDivMesh& dm : mDivMesh)
			{
				if (CBounds::Intersect(bounds, dm.bounds))
				{
					dm.vertices.push_back(&v);
				}
			}
		}
	}
	else
	{
		STDivMesh dm;
		dm.bounds = bounds;
		for (STVertexData& v : mVertices)
		{
			dm.vertices.push_back(&v);
		}
		mDivMesh.push_back(dm);
	}
}

const std::vector<STVertexData>& CColliderMesh::Get() const
{
	return mVertices;
}

const std::vector<STDivMesh>& CColliderMesh::GetDivMesh() const
{
	return mDivMesh;
}

#if _DEBUG
void CColliderMesh::Render()
{
	// 現在の行列を退避しておく
	glPushMatrix();

	// ブレンド処理を有効化
	Blend::EnableBlend(EBlend::eAlpha);
	// ライトオフ
	glDisable(GL_LIGHTING);

	// DIFFUSE赤色設定
	CColor col = CColor::red;
	col.A(0.2f);
	float* c = (float*)(&col);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(c);

	// 三角形を描画
	glBegin(GL_TRIANGLES);
	for (const STVertexData& v : mVertices)
	{
		glVertex3f(v.wv.V[0].X(), v.wv.V[0].Y(), v.wv.V[0].Z());
		glVertex3f(v.wv.V[1].X(), v.wv.V[1].Y(), v.wv.V[1].Z());
		glVertex3f(v.wv.V[2].X(), v.wv.V[2].Y(), v.wv.V[2].Z());
	}
	glEnd();

	// ライトオン
	glEnable(GL_LIGHTING);
	// ブレンド処理を無効化
	Blend::DisableBlend();

	// 描画前の行列に戻す
	glPopMatrix();
}

void CColliderMesh::RenderBounds()
{
	CCollider::RenderBounds();
	RenderDivArea();

	//if (mDivMesh.size() == 1) return;

	//CColor col = CColor::green;
	//if (!IsEnable() ||
	//	(Owner() != nullptr && !Owner()->IsEnableCol()))
	//{
	//	col = CColor::gray;
	//}
	//col.A(0.35f);
	//for (STDivMesh& divMesh : mDivMesh)
	//{
	//	Primitive::DrawWireBox
	//	(
	//		divMesh.bounds.Center(), divMesh.bounds.Size(),
	//		col, EBlend::eAlpha
	//	);
	//}
}

void CColliderMesh::RenderDivArea()
{
	int divCnt = mDivX * mDivY * mDivZ;
	if (divCnt <= 1) return;

	CColor col = CColor::green;
	col.A(0.35f);
	CVector start = mBounds.Min();
	CVector size = mBounds.Size();
	CVector min, max;
	for (int x = 0; x < mDivX; x++)
	{
		min.X(start.X() + size.X() * ((float)x / mDivX));
		max.X(start.X() + size.X() * ((float)(x + 1) / mDivX));
		for (int y = 0; y < mDivY; y++)
		{
			min.Y(start.Y() + size.Y() * ((float)y / mDivY));
			max.Y(start.Y() + size.Y() * ((float)(y + 1) / mDivY));
			for (int z = 0; z < mDivZ; z++)
			{
				min.Z(start.Z() + size.Z() * ((float)z / mDivZ));
				max.Z(start.Z() + size.Z() * ((float)(z + 1) / mDivZ));

				CVector center = (min + max) * 0.5f;
				CVector size2 = max - min;
				Primitive::DrawWireBox
				(
					center, size2,
					col, EBlend::eAlpha
				);
			}
		}
	}
}
#endif

// コライダーの情報を更新
void CColliderMesh::UpdateCol(bool isInit)
{
	// 前回の更新と同じ行列であれば、処理しない
	CMatrix m = Matrix();
	if (!isInit && m == mLastMtx) return;
	mLastMtx = m;

	CVector minMesh;
	CVector maxMesh;
	int size = mVertices.size();
	for (int i = 0; i < size; i++)
	{
		STVertexData& v = mVertices[i];
		v.wv.V[0] = v.lv.V[0] * m;
		v.wv.V[1] = v.lv.V[1] * m;
		v.wv.V[2] = v.lv.V[2] * m;
		v.bounds = CBounds::GetTriangleBounds(v.wv.V[0], v.wv.V[1], v.wv.V[2]);

		const CVector& minTri = v.bounds.Min();
		const CVector& maxTri = v.bounds.Max();
		if (i > 0)
		{
			minMesh = CVector::Min(minMesh, minTri);
			maxMesh = CVector::Max(maxMesh, maxTri);
		}
		else
		{
			minMesh = minTri;
			maxMesh = maxTri;
		}
	}
	mBounds.SetRange(minMesh, maxMesh);

	for (STDivMesh& divMesh : mDivMesh)
	{
		CVector min, max;
		size = divMesh.vertices.size();
		for (int i = 0; i < size; i++)
		{
			STVertexData* v = divMesh.vertices[i];
			CBounds tb = CBounds::GetTriangleBounds(v->wv.V[0], v->wv.V[1], v->wv.V[2]);
			CVector minV = tb.Min();
			CVector maxV = tb.Max();
			if (i > 0)
			{
				min = CVector::Min(min, minV);
				max = CVector::Max(max, maxV);
			}
			else
			{
				min = minV;
				max = maxV;
			}
		}
		divMesh.bounds.SetRange(min, max);
	}
}
