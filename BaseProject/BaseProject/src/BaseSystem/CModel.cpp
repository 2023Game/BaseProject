#include "CModel.h"
#include "CVector.h"
#include "Maths.h"
#include "glut.h"

std::vector<CTriangle> CModel::Triangles() const
{
	return mTriangles;
}

//モデル読み込み
bool CModel::Load(std::string path, bool dontDelete)
{
	//頂点データの保存(CVector型)
	std::vector<CVector> vertex;
	std::vector<CVector> normal;
	//テクスチャマッピングの保存(CVector型)
	std::vector<CVector> uv;

	std::string dirPath = path;
	int index = dirPath.find_last_of('\\');
	if (index >= 0)
	{
		dirPath = dirPath.substr(0, index + 1);
	}

	std::string objPath = RES_DIR;
	objPath += path;

	//ファイルポインタ変数の作成
	FILE* fp;
	//ファイルからデータを入力
	//入力エリアを作成する
	char buf[256];

	//ファイルのオープン
	//fopen(ファイル名,モード)
	//オープンできない時はNULLを返す
	fp = fopen(objPath.c_str(), "r");
	//ファイルオープンエラーの判定
	//fpがNULLの時はエラー
	if (fp == NULL) {
		//コンソールにエラー出力して戻る
		printf("%s file open error￥n", objPath.c_str());
		return false;
	}

	int matIdx = 0;

	//ファイルから1行入力
	//fgets(入力エリア,エリアサイズ,ファイルポインタ)
	//ファイルの最後になるとNULLを返す
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		//データを分割する
		char str[4][64] = { "", "", "", "" };
		//文字列からデータを4つ変数へ代入する
		//sscanf(文字列, 変換指定子, 変数)
		sscanf(buf, "%s %s %s %s", str[0], str[1], str[2], str[3]);
		//文字列の比較
		//strcmp(文字列1, 文字列2)
		//文字列1と文字列2が同じ時0、異なる時0以外を返す
		//先頭がvの時、頂点をvertexに追加する
		if (strcmp(str[0], "v") == 0) {
			//可変長配列vertexに追加
			//atof(文字列)　文字列からfloat型の値を返す
			vertex.push_back(CVector(atof(str[1]), atof(str[2]), atof(str[3])));
		}
		else if (strcmp(str[0], "vn") == 0) {
			//可変長配列vertexに追加
			//atof(文字列)　文字列からfloat型の値を返す
			normal.push_back(CVector(atof(str[1]), atof(str[2]), atof(str[3])));
		}
		//先頭がfの時、三角形を作成して追加する
		else if (strcmp(str[0], "f") == 0) {
			//頂点と法線の番号作成
			int v[3], n[3];
			//テクスチャマッピングの有無を判定
			if (strstr(str[1], "//")) {
				//頂点と法線の番号取得
				sscanf(str[1], "%d//%d", &v[0], &n[0]);
				sscanf(str[2], "%d//%d", &v[1], &n[1]);
				sscanf(str[3], "%d//%d", &v[2], &n[2]);
				//三角形作成
				CTriangle t;
				t.Vertex(vertex[v[0] - 1], vertex[v[1] - 1], vertex[v[2] - 1]);
				t.Normal(normal[n[0] - 1], normal[n[1] - 1], normal[n[2] - 1]);
				//マテリアル番号の設定
				t.MaterialIdx(matIdx);
				//可変長配列mTrianglesに三角形を追加
				mTriangles.push_back(t);
			}
			else {
				//テクスチャマッピング有り
				int u[3]; //テクスチャマッピングの番号
				//頂点と法線の番号取得とマッピングの番号取得
				sscanf(str[1], "%d/%d/%d", &v[0], &u[0], &n[0]);
				sscanf(str[2], "%d/%d/%d", &v[1], &u[1], &n[1]);
				sscanf(str[3], "%d/%d/%d", &v[2], &u[2], &n[2]);
				//三角形作成
				CTriangle t;
				t.Vertex(vertex[v[0] - 1], vertex[v[1] - 1], vertex[v[2] - 1]);
				t.Normal(normal[n[0] - 1], normal[n[1] - 1], normal[n[2] - 1]);
				//テクスチャマッピングの設定
				t.UV(uv[u[0] - 1], uv[u[1] - 1], uv[u[2] - 1]);
				//マテリアル番号の設定
				t.MaterialIdx(matIdx);
				//可変長配列mTrianglesに三角形を追加
				mTriangles.push_back(t);
			}
		}
		//先頭がmtllibの時、マテリアルを読み込む
		else if (strcmp(str[0], "mtllib") == 0) {
			std::string mtlPath = dirPath + "\\" + str[1];
			LoadMaterial(mtlPath, dontDelete);
		}
		//先頭がusemtlの時、マテリアルインデックスを取得する
		else if (strcmp(str[0], "usemtl") == 0) {
			//可変長配列を後から比較
			for (matIdx = mpMaterials.size() - 1; matIdx > 0; matIdx--) {
				//同じ名前のマテリアルがあればループ終了
				if (strcmp(mpMaterials[matIdx]->Name(), str[1]) == 0) {
					break; //ループから出る
				}
			}
		}
		//先頭がvtの時、uvに追加する
		else if (strcmp(str[0], "vt") == 0) {
			//可変長配列uvに追加
			//atof(文字列)　文字列からfloat型の値を返す
			uv.push_back(CVector(atof(str[1]), atof(str[2]), 0.0));
		}
	}

	//ファイルのクローズ
	fclose(fp);

	CreateVertexBuffer();

	return true;
}

//マテリアル読み込み
bool CModel::LoadMaterial(std::string path, bool dontDelete)
{
	std::string mtlPath = RES_DIR + path;

	std::string dirPath = path;
	int index = dirPath.find_last_of('\\');
	if (index >= 0)
	{
		dirPath = dirPath.substr(0, index + 1);
	}

	//ファイルポインタ変数の作成
	FILE* fp;
	//ファイルからデータを入力
	//入力エリアを作成する
	char buf[256];

	//ファイルのオープン
	//fopen(ファイル名,モード)
	//オープンできない時はNULLを返す
	fp = fopen(mtlPath.c_str(), "r");
	//ファイルオープンエラーの判定
	//fpがNULLの時はエラー
	if (fp == NULL) {
		//コンソールにエラー出力して戻る
		printf("%s file open error￥n", mtlPath.c_str());
		return false;
	}

	//マテリアルインデックス
	int idx = 0;
	//ファイルから1行入力
	//fgets(入力エリア,エリアサイズ,ファイルポインタ)
	//ファイルの最後になるとNULLを返す
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		//データを分割する
		char str[4][64] = { "", "", "", "" };
		//文字列からデータを4つ変数へ代入する
		sscanf(buf, "%s %s %s %s", str[0], str[1], str[2], str[3]);
		//先頭がnewmtlの時、マテリアルを追加する
		if (strcmp(str[0], "newmtl") == 0) {
			CMaterial* pm = new CMaterial();
			//マテリアル名の設定
			pm->Name(str[1]);
			//マテリアルの可変長配列に追加
			mpMaterials.push_back(pm);
			//配列の長さを取得
			idx = mpMaterials.size() - 1;
		}
		//先頭がKdの時、Diffuseを設定する
		else if (strcmp(str[0], "Kd") == 0) {
			mpMaterials[idx]->Diffuse()[0] = atof(str[1]);
			mpMaterials[idx]->Diffuse()[1] = atof(str[2]);
			mpMaterials[idx]->Diffuse()[2] = atof(str[3]);
		}
		//先頭がdの時、α値を設定する
		else if (strcmp(str[0], "d") == 0) {
			mpMaterials[idx]->Diffuse()[3] = atof(str[1]);
		}
		//先頭がmap_Kdの時、テクスチャを入力する
		else if (strcmp(str[0], "map_Kd") == 0) {
			std::string texPath = dirPath + str[1];
			mpMaterials[idx]->LoadTexture(str[1], texPath.c_str(), dontDelete);
		}

	}

	//ファイルのクローズ
	fclose(fp);

	return true;
}

void CModel::Render()
{
	// 完全に透明な状態であれば、描画しない
	if (mColor.A() == 0.0f) return;

	//可変長配列の要素数だけ繰り返し
	for (int i = 0; i < mTriangles.size(); i++) {
		//マテリアルの適用
		mpMaterials[mTriangles[i].MaterialIdx()]->Enabled(mColor);
		//可変長配列に添え字でアクセスする
		mTriangles[i].Render();
		//マテリアルを無効
		mpMaterials[mTriangles[i].MaterialIdx()]->Disabled();
	}
}

CModel::CModel()
	: mpVertexes(nullptr)
	, mColor(CColor::white)
	, mIsLighting(true)
	, mIsDepthWrite(true)
	, mIsCullFace(true)
	, mBlendMode(EBlend::eAlpha)
{
}

CModel::~CModel()
{
	for (int i = 0; i < mpMaterials.size(); i++)
	{
		delete mpMaterials[i];
	}
	delete[] mpVertexes;
}

// カラーを設定
void CModel::SetColor(const CColor& color)
{
	mColor = color;
}

// カラーを取得
const CColor& CModel::GetColor() const
{
	return mColor;
}

// アルファ値設定
void CModel::SetAlpha(float alpha)
{
	mColor.A(Math::Clamp01(alpha));
}

// アルファ値取得
float CModel::GetAlpha() const
{
	return mColor.A();
}

// ライティングの有効状態を設定
void CModel::SetLighting(bool enable)
{
	mIsLighting = enable;
}

// ライティングの有効状態を取得
bool CModel::IsLighting() const
{
	return mIsLighting;
}

// デプス値の書き込みを行うか設定
void CModel::SetDepthWrite(bool enable)
{
	mIsDepthWrite = enable;
}

// デプス値の書き込みを行うかどうか
bool CModel::IsDepthWrite() const
{
	return mIsDepthWrite;
}

// カリングを行うかどうかを設定
void CModel::SetCullFace(bool enable)
{
	mIsCullFace = enable;
}

// カリングを行うかどうか
bool CModel::IsCullFace() const
{
	return mIsCullFace;
}

// 描画時のブレンド処理を設定
void CModel::SetBlend(EBlend mode)
{
	mBlendMode = mode;
	for (CMaterial* mat : mpMaterials)
	{
		mat->SetBlendType(mBlendMode);
	}
}

// 描画時のブレンド処理を取得
EBlend CModel::GetBlend() const
{
	return mBlendMode;
}

// エフェクト用の設定
void CModel::SetupEffectSettings()
{
	SetLighting(false);
	SetDepthWrite(false);
	SetCullFace(false);
	SetBlend(EBlend::eAdd);
}

//描画
//Render(行列)
void CModel::Render(const CMatrix& m)
{
	// 完全に透明な状態であれば、描画しない
	if (mColor.A() == 0.0f) return;

	//デプス値の書き込み設定
	glDepthMask(mIsDepthWrite);
	//ライティング設定
	if (!mIsLighting) glDisable(GL_LIGHTING);
	//カリング設定
	if (!mIsCullFace) glDisable(GL_CULL_FACE);

	//行列の退避
	glPushMatrix();
	//合成行列を掛ける
	glMultMatrixf(m.M());

	//頂点座標の位置を設定
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(CVertex), (void*)&mpVertexes[0].mPosition);
	//法線ベクトルの位置を設定
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, sizeof(CVertex), (void*)&mpVertexes[0].mNormal);
	//テクスチャマッピングの位置を設定
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(CVertex), (void*)&mpVertexes[0].mTextureCoords);

	int first = 0; //描画位置
	//マテリアル毎に描画する
	for (size_t i = 0; i < mpMaterials.size(); i++) {
		//マテリアルを適用する
		mpMaterials[i]->Enabled(mColor);
		//描画位置からのデータで三角形を描画します
		glDrawArrays(GL_TRIANGLES, first, mpMaterials[i]->VertexNum());
		//マテリアルを無効にする
		mpMaterials[i]->Disabled();
		//描画位置を移動
		first += mpMaterials[i]->VertexNum();
	}
	//行列を戻す
	glPopMatrix();

	//頂点座標の配列を無効にする
	glDisableClientState(GL_VERTEX_ARRAY);
	//法線の配列を無効にする
	glDisableClientState(GL_NORMAL_ARRAY);
	//テクスチャマッピングの配列を無効にする
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	//デプス値の書き込み状態を戻す
	glDepthMask(true);
	//ライティング設定
	glEnable(GL_LIGHTING);
	//カリング設定
	glEnable(GL_CULL_FACE);
}

void CModel::CreateVertexBuffer()
{
	mpVertexes = new CVertex[mTriangles.size() * 3];
	int idx = 0;
	for (int i = 0; i < mpMaterials.size(); i++)
	{
		for (int j = 0; j < mTriangles.size(); j++)
		{
			if (i == mTriangles[j].MaterialIdx())
			{
				mpMaterials[i]->VertexNum(mpMaterials[i]->VertexNum() + 3);
				mpVertexes[idx].mPosition = mTriangles[j].V0();
				mpVertexes[idx].mNormal = mTriangles[j].N0();
				mpVertexes[idx++].mTextureCoords = mTriangles[j].U0();
				mpVertexes[idx].mPosition = mTriangles[j].V1();
				mpVertexes[idx].mNormal = mTriangles[j].N1();
				mpVertexes[idx++].mTextureCoords = mTriangles[j].U1();
				mpVertexes[idx].mPosition = mTriangles[j].V2();
				mpVertexes[idx].mNormal = mTriangles[j].N2();
				mpVertexes[idx++].mTextureCoords = mTriangles[j].U2();
			}
		}
	}
}

