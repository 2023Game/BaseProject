#ifndef CMATERIAL_H
#define CMATERIAL_H
#include "CTexture.h"
#include "CModelX.h"
#define MATERIAL_NAME_LEN 64 //名前の長さ
#include "CColor.h"

class CMyShader;
enum class EBlend;

/*
マテリアルクラス
マテリアルのデータを扱う
*/
class CMaterial {
	friend CMyShader;
private:
	// ブレンド処理を有効化
	void EnableBlend();
	// ブレンド処理を無効化
	void DisableBlend();

	//マテリアル毎の頂点数
	int mVertexNum;
	//拡散光の色RGBA
	float mDiffuse[4];
	float mPower;
	float mSpecular[3];
	float mEmissive[3];

	//テクスチャファイル名
	char* mpTextureFilename;
	//マテリアル名
	char mName[MATERIAL_NAME_LEN + 1];
	//テクスチャ
	CTexture* mpTexture;
	//ブレンド処理のタイプ
	EBlend mBlendType;

public:
	//テクスチャ読み込み
	bool LoadTexture(std::string name, std::string path, bool dontDelete);
	//テクスチャの取得
	CTexture* Texture();
	//マテリアルを無効にする
	void Disabled();
	//デフォルトコンストラクタ
	CMaterial();
	CMaterial(CModelX* model, bool dontDelete);
	~CMaterial();
	//マテリアルを有効にする
	void Enabled(const CColor& color);
	//マテリアルの名前の取得
	char* Name();
	//マテリアルの名前を設定する
	//Name(マテリアルの名前)
	void Name(char* name);
	//mDiffuse配列の取得
	float* Diffuse();
	//頂点数の設定
	//VertexNum(頂点数)
	void VertexNum(int num);
	//頂点数の取得
	int VertexNum();

	//ブレンド処理のタイプを設定
	void SetBlendType(EBlend type);
};

// ブレンド処理のタイプ
enum class EBlend
{
	eAlpha,		// アルファブレンド
	eAdd,		// 加算ブレンド
	eMultiply,	// 乗算ブレンド
	eInvert,	// 色反転
};

#endif

