#include "CMaterial.h"
//memset,strncpyのインクルード
#include <string.h>
#include "glut.h"
/*
* strncpy(char* str1, const char* str2, int len)
* コピー先str1にコピー元str2の文字をlen文字数までコピーする
*/
char* strncpy(char* str1, const char* str2, int len)
{
	int i = 0;
	//iがlenより小さく、かつ、コピー元が終わりでない間繰り返し
	while (i < len && *str2 != '\0')
	{
		*(str1 + i) = *str2; //コピー先にコピー元を代入
		str2++; //コピー元を次へ
		i++;
	}
	str1[i] = '\0'; //コピー先の文字列に終わり
	return str1; //コピー先の先頭アドレスを返却
}

bool CMaterial::LoadTexture(std::string name, std::string path, bool dontDelete)
{
	mpTexture = CResourceManager::Load<CTexture>(path, path, dontDelete);
	return mpTexture != nullptr;
}

CTexture* CMaterial::Texture()
{
	return mpTexture;
}

void CMaterial::Disabled()
{
	//ブレンド処理を無効
	Blend::DisableBlend();
	//テクスチャ有り
	if (mpTexture != nullptr && mpTexture->Id())
	{
		//テクスチャのバインドを解く
		glBindTexture(GL_TEXTURE_2D, 0);
		//テクスチャを無効にする
		glDisable(GL_TEXTURE_2D);
	}
}

//デフォルトコンストラクタ
CMaterial::CMaterial()
	: mVertexNum(0)
	, mPower(0)
	, mpTextureFilename(nullptr)
	, mpTexture(nullptr)
	, mBlendType(EBlend::eAlpha)
{
	//名前を0で埋め
	memset(mName, 0, sizeof(mName));
	//Deffuseカラーはデフォルトは(1, 1, 1, 1)
	int size = ARRAY_SIZE(mDiffuse);
	for (int i = 0; i < size; i++)
	{
		mDiffuse[i] = 1.0f;
	}
}

/*
Materialデータの読み込みと設定
*/
CMaterial::CMaterial(CModelX *model, bool dontDelete)
	: mpTextureFilename(nullptr)
	, mBlendType(EBlend::eAlpha)
{
	//CModelXにマテリアルを追加する
	model->Material().push_back(this);
	model->GetToken(); // { ? Name
	if (strcmp(model->Token(), "{") != 0)
	{
		// {でない時はマテリアル名
		strcpy(mName, model->Token());
		model->GetToken(); //{
	}

	mDiffuse[0] = atof(model->GetToken());
	mDiffuse[1] = atof(model->GetToken());
	mDiffuse[2] = atof(model->GetToken());
	mDiffuse[3] = atof(model->GetToken());

	mPower = atof(model->GetToken());

	mSpecular[0] = atof(model->GetToken());
	mSpecular[1] = atof(model->GetToken());
	mSpecular[2] = atof(model->GetToken());

	mEmissive[0] = atof(model->GetToken());
	mEmissive[1] = atof(model->GetToken());
	mEmissive[2] = atof(model->GetToken());

	model->GetToken(); //TextureFilename or }

	if (strcmp(model->Token(), "TextureFilename") == 0)
	{
		//テクスチャありの場合、テクスチャファイル名取得
		model->GetToken(); // {
		model->GetToken(); // filename
		mpTextureFilename = new char[strlen(model->Token()) + 1];
		strcpy(mpTextureFilename, model->Token());

		//テクスチャの読み込み
		std::string texPath = model->DirPath() + mpTextureFilename;
		LoadTexture(mpTextureFilename, texPath, dontDelete);

		model->GetToken(); // }
		model->GetToken(); // }
	}
}

CMaterial::~CMaterial()
{
	if (mpTextureFilename)
	{
		delete[] mpTextureFilename;
	}
	mpTextureFilename = nullptr;

	if (mpTexture != nullptr)
	{
	}
}

//マテリアルを有効にする
void CMaterial::Enabled(const CColor& color)
{
	glColor4fv((GLfloat*)&color);
	//拡散光の設定
	float diffuse[4];
	diffuse[0] = mDiffuse[0] * color.R();
	diffuse[1] = mDiffuse[1] * color.G();
	diffuse[2] = mDiffuse[2] * color.B();
	diffuse[3] = mDiffuse[3] * color.A();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);

	//ブレンド処理を有効にする
	Blend::EnableBlend(mBlendType);
	//テクスチャ有り
	if (mpTexture != nullptr && mpTexture->Id())
	{
		//テクスチャを使用可能にする
		glEnable(GL_TEXTURE_2D);
		//テクスチャをバインドする
		glBindTexture(GL_TEXTURE_2D, mpTexture->Id());
	}
}
//マテリアルの名前の取得
char* CMaterial::Name()
{
	return mName;
}

// マテリアルの名前を設定する
// Name(マテリアルの名前)
void CMaterial::Name(char* name)
{
	strncpy(mName, name, MATERIAL_NAME_LEN);
}
//mDiffuse配列の取得
float* CMaterial::Diffuse()
{
	return mDiffuse;
}

void CMaterial::VertexNum(int num)
{
	mVertexNum = num;
}

int CMaterial::VertexNum()
{
	return mVertexNum;
}

//ブレンド処理のタイプを設定
void CMaterial::SetBlendType(EBlend type)
{
	mBlendType = type;
}
