#ifndef CMODELX_H
#define CMODELX_H
#include <vector>		//vectorクラスのインクルード
#include "CMatrix.h"	//マトリクスクラスのインクルード
#include "CVector.h"	//Vectorクラスのインクルード
#include "CMyShader.h"	//シェーダークラスのインクルード
#include "CResource.h"
#include "CColor.h"

class CModelX;			//CModelクラスの宣言
class CModelXFrame;		//CModelXFrameクラスの宣言
class CMesh;			//CMeshクラスの宣言
class CMaterial;		//マテリアルの宣言
class CSkinWeights;		//スキンウェイトクラスの宣言
class CAnimationSet;	//アニメーションセットクラスの宣言
class CAnimation;		//アニメーションクラスの宣言
class CAnimationKey;	//アニメーションキークラスの宣言
class CXCharacter;		//CXCharacterクラスの宣言

/*
CModel
Xファイル形式の3Dモデルデータをプログラムで認識する
*/
class CModelX : public CResource
{
	friend CResourceManager;
	friend CModelXFrame;
	friend CMesh;
	friend CAnimationSet;
	friend CAnimation;
	friend CModelX;
	friend CMyShader;
private:
	//コンストラクタ
	CModelX();
	//デストラクタ
	~CModelX();

	//ファイル読み込み
	bool Load(std::string path, bool dontDelete) override;

	std::vector<CMaterial*> mMaterial;	//マテリアル配列
	std::vector<CModelXFrame*> mFrame;  //フレームの配列
	std::vector<CAnimationSet*> mAnimationSet;	//アニメーションセット配列

	CMatrix* mpSkinningMatrix;	//シェーダー用スキンマトリックス
	CMyShader mShader;	//シェーダーのインスタンス

	char* mpPointer;	//読み込み位置
	char mToken[1024];	//取り出した単語の領域

	//cが区切り文字ならtrueを返す
	bool IsDelimiter(char c);
	//読み込み済みフラグ
	bool mLoaded;

	std::string mFilePath;	//モデルデータのファイルパス
	std::string mDirPath;	//モデルデータのディレクトリパス

	CColor mColor;		//モデルのカラー

public:
	//マテリアル配列の取得
	std::vector<CMaterial*>& Material();
	std::vector<CModelXFrame*>& Frames();
	std::vector<CAnimationSet*>& CModelX::AnimationSet();
	//フレーム名に該当するフレームのアドレスを返す
	CModelXFrame* FinedFrame(const char* name);
	//マテリアルの検索
	CMaterial* FindMaterial(char* name);

	//単語の取り出し
	char* GetToken();
	char* Token();

	//トークンがなくなったらtrue
	bool EOT();
	//読み込み済みか判定
	bool IsLoaded();

	//ファイルパスを取得
	std::string FilePath() const;
	//ディレクトリパスを取得
	std::string DirPath() const;

	//シェーダーを使った描画
	void RenderShader(CMatrix* m);
	bool IsAddAnimationSet(std::string path) const;
	//アニメーションセットの追加
	void AddAnimationSet(const char* file);
	//アニメーションを抜き出す
	void SeparateAnimationSet(int idx, int start, int end, char* name);
	void AnimateVertex(CMatrix* mat);
	//頂点にアニメーションを適用
	void AnimateVertex();
	//スキンウェイトのフレーム番号設定
	void SetSkinWeightFrameIndex();
	//フレームの変換行列をアニメーションデータで更新する
	void AnimateFrame();
	//ノードの読み飛ばし
	void SkipNode();

	// カラーを設定
	void SetColor(const CColor& color);
	// カラーを取得
	const CColor& GetColor() const;
	// アルファ値設定
	void SetAlpha(float alpha);
	// アルファ値取得
	float GetAlpha() const;

	//描画処理
	void Render();
};

//CModelXFrameクラスの定義
class CModelXFrame {
	friend CModelX;
	friend CAnimation;
	friend CAnimationSet;
	friend CMyShader;
private:
	std::vector<CModelXFrame*> mChild; //子フレームの配列
	CMesh* mpMesh;  //Meshデータ
	CMatrix mTransformMatrix;  //変換行列
	CMatrix mCombinedMatrix;   //合成行列
	char* mpName;  //フレーム名前
	int mIndex;  //フレーム番号

public:
	//デフォルトコンストラクタ
	CModelXFrame();
	//コンストラクタ
	CModelXFrame(CModelX* model, bool dontDelete);
	//デストラクタ
	~CModelXFrame();

	int Index();

	const CMatrix& CombinedMatrix();
	//合成行列の作成
	void AnimateCombined(const CMatrix* parent);
	//描画処理
	void Render(const CColor& color);
};

//Meshクラスの定義
class CMesh {
	friend CModelX;
	friend CModelXFrame;
	friend CSkinWeights;
	friend CMyShader;
private:
	int mFaceNum;          //面数
	int mVertexNum;	   	   //頂点数
	int mNormalNum;        //法線数
	int mMaterialNum;      //マテリアル数
	int mMaterialIndexNum; //マテリアル番号数(面数)
	int* mpMaterialIndex;  //マテリアル番号
	int* mpVertexIndex;    //面を構成する頂点インデックス
	float* mpTextureCoords; //テクスチャ座標データ

	std::vector<int> mMaterialVertexCount;		//マテリアル毎の面数
	std::vector<CSkinWeights*> mSkinWeights;	//スキンウェイト
	std::vector<CMaterial*> mMaterial; //マテリアルデータ
	CVector* mpVertex;			//頂点データ
	CVector* mpNormal;			//法線ベクトル
	CVector* mpAnimateVertex;	//アニメーション用頂点
	CVector* mpAnimateNormal;	//アニメーション用法線
	CSkinWeights* mpSkinWeghts;

	GLuint mMyVertexBufferId;	//頂点バッファ識別子

public:
	//コンストラクタ
	CMesh();
	//デストラクタ
	~CMesh();

	void CreateVertexBuffer();
	void AnimateVertex(CMatrix* mat);
	//頂点にアニメーション適用
	void AnimateVertex(CModelX* model);
	//スキンウェイトにフレーム番号を設定する
	void SetSkinWeightFrameIndex(CModelX* model);
	//読み込み処理
	void Init(CModelX* model, bool dontDelete);
	//描画処理
	void Render(const CColor& color);
};

/*
CSkinWeights
スキンウェイトクラス
*/
class CSkinWeights
{
	friend CModelX;
	friend CMesh;
	friend CMyShader;
private:
	char* mpFrameName;	//フレーム名
	int mFrameIndex;	//フレーム番号
	int mIndexNum;		//頂点番号数
	int* mpIndex;		//頂点番号配列
	float* mpWeight;	//頂点ウェイト配列
	CMatrix mOffset;	//オフセットマトリックス

public:
	CSkinWeights(CModelX* model);
	~CSkinWeights();
	const int& FrameIndex();
	const CMatrix& Offset();
};

/*
CAnimationSet
アニメーションセット
*/
class CAnimationSet
{
	friend CModelX;
	friend CXCharacter;
private:
	std::string mPath;
	char* mpName;	//アニメーションセット名
	float mTime;	//現在時間
	float mWeight;	//重み
	float mMaxTime;	//最大時間
	std::vector<CAnimation*> mAnimation;

public:
	//コンストラクタ
	CAnimationSet();
	CAnimationSet(CModelX* model);
	//デストラクタ
	~CAnimationSet();

	float Time();
	float MaxTime();
	std::vector<CAnimation*>& Animation();
	void AnimateMarix(CModelX* model);
	/// <summary>
	/// アニメーションの再生時間を設定
	/// </summary>
	/// <param name="time">設定する時間</param>
	void Time(float time);
	/// <summary>
	/// アニメーションの再生時間を進行度で設定
	/// </summary>
	/// <param name="progress">再生時間の進行度（0～1）</param>
	void TimeProgress(float progress);
	void Weight(float weight);	//重みの設定
};

/*
CAnimation
アニメーションクラス
*/
class CAnimation
{
	friend CModelX;
	friend CAnimationSet;
private:
	char* mpFrameName;		//フレーム名
	int mFrameIndex;		//フレーム番号
	int mKeyNum;			//キー数(時間数)
	CAnimationKey* mpKey;	//キー配列

public:
	//コンストラクタ
	CAnimation();
	CAnimation(CModelX* model);
	//デストラクタ
	~CAnimation();
};

/*
CAnimationKey
アニメーションキークラス
*/
class CAnimationKey
{
	friend CModelX;
	friend CAnimation;
	friend CAnimationSet;
private:
	float mTime;		//時間
	CMatrix mMatrix;	//行列
};
#endif