#ifndef CVERTEX_H
#define CVERTEX_H
#include "CVector.h"
/*
頂点データクラス
頂点毎にデータをまとめます
*/
class CVertex {
public:
	//位置
	CVector mPosition;
	//法線
	CVector mNormal;
	//テクスチャマッピング
	CVector mTextureCoords;

	//スキンウェイト
	float mBoneWeight[4];
	//スキンインデックス
	float mBoneIndex[4];

	CVertex() {
		for (int i = 0; i < 4; i++) {
			mBoneIndex[i] = 0;
			mBoneWeight[i] = 0.0f;
		}
		mBoneWeight[0] = 1.0f;
	}
};

// 三角形ポリゴンの頂点データ
struct STVertex
{
	CVector V[3];
	STVertex() : STVertex(CVector(), CVector(), CVector()) {}
	STVertex(const CVector& v0, const CVector& v1, const CVector& v2)
	{
		Set(v0, v1, v2);
	}
	void Set(const CVector& v0, const CVector& v1, const CVector& v2)
	{
		V[0] = v0;
		V[1] = v1;
		V[2] = v2;
	}
};

#endif


