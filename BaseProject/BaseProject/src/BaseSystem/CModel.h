#ifndef CMODEL_H
#define CMODEL_H
//vectorのインクルード
#include <vector>
#include "CTriangle.h"
#include "CMaterial.h"
#include "CVertex.h"
#include "CResource.h"
#include "CColor.h"

/*
モデルクラス
モデルデータの入力や表示
*/
class CModel : public CResource
{
	friend CResourceManager;

public:
	std::vector<CTriangle> Triangles() const;

	// カラーを設定
	void SetColor(const CColor& color);
	// カラーを取得
	const CColor& GetColor() const;
	// アルファ値設定
	void SetAlpha(float alpha);
	// アルファ値取得
	float GetAlpha() const;

	// ライティングの有効状態を設定
	void SetLighting(bool enable);
	// ライティングの有効状態を取得
	bool IsLighting() const;

	// デプス値の書き込みを行うか設定
	void SetDepthWrite(bool enable);
	// デプス値の書き込みを行うかどうか
	bool IsDepthWrite() const;

	// カリングを行うかどうかを設定
	void SetCullFace(bool enable);
	// カリングを行うかどうか
	bool IsCullFace() const;

	// 描画時のブレンド処理を設定
	void SetBlend(EBlend mode);
	// 描画時のブレンド処理を取得
	EBlend GetBlend() const;

	// エフェクト用の設定
	void SetupEffectSettings();

	// 描画
	void Render();
	// 描画
	// Render(行列)
	void Render(const CMatrix& m);
private:
	CModel();
	~CModel();
	// モデル読み込み
	bool Load(std::string path, bool dontDelete) override;
	// マテリアル読み込み
	bool LoadMaterial(std::string path, bool dontDelete);
	void CreateVertexBuffer();

	// マテリアルポインタの可変長配列
	std::vector<CMaterial*> mpMaterials;
	// 三角形の可変長配列
	std::vector<CTriangle> mTriangles;
	// 頂点の配列
	CVertex* mpVertexes;
	// モデルのカラー
	CColor mColor;

	// ライティングが有効かどうか
	bool mIsLighting;
	// デプス値書き込みを行うかどうか
	bool mIsDepthWrite;
	// カリングを行うかどうか
	bool mIsCullFace;
	// 描画時のブレンド処理
	EBlend mBlendMode;
};

#endif
