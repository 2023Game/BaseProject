#include "CResourceManager.h"
#include "CModel.h"
#include "CModelX.h"
#include "CTexture.h"
#include "CSound.h"

//テンプレートの前宣言
template CModel* CResourceManager::Load(std::string name, std::string path, bool dontDelete);
template CModelX* CResourceManager::Load(std::string name, std::string path, bool dontDelete);
template CTexture* CResourceManager::Load(std::string name, std::string path, bool dontDelete);
template CSound* CResourceManager::Load(std::string name, std::string path, bool dontDelete);
template CModel* CResourceManager::Get(std::string name);
template CModelX* CResourceManager::Get(std::string name);
template CTexture* CResourceManager::Get(std::string name);
template CSound* CResourceManager::Get(std::string name);

//CResourceManagerのインスタンス
CResourceManager* CResourceManager::mpInstance = nullptr;

//コンストラクタ
CResourceManager::CResourceManager()
{
}

//デストラクタ
CResourceManager::~CResourceManager()
{
	//読み込み済みのモデルデータを破棄
	for (auto& res : mResources)
	{
		delete res.second;
	}
	mResources.clear();

	mpInstance = nullptr;
}

//インスタンス取得
CResourceManager* CResourceManager::Instance()
{
	if (mpInstance == nullptr)
	{
		mpInstance = new CResourceManager();
	}
	return mpInstance;
}

//インスタンスを破棄
void CResourceManager::ClearInstance()
{
	SAFE_DELETE(mpInstance);
}

// リソース読み込み
template <class T>
T* CResourceManager::Load(std::string name, std::string path, bool dontDelete)
{
	// 既にリソースが読み込み済みであれば、
	// 読み込み済みのリソースを返す
	auto& list = Instance()->mResources;
	auto find = list.find(name);
	if (find != list.end())
	{
		return dynamic_cast<T*>(find->second);
	}

	// リソース読み込み
	CResource* res = new T();
	if (res == nullptr) return nullptr;
	if (!res->Load(path, dontDelete))
	{
		// 読み込み失敗
		delete res;
		return nullptr;
	}
	res->SetDontDelete(dontDelete);

	// リソースのリストに追加
	list.insert(make_pair(name, res));

	// 読み込んだリソースを返す
	return dynamic_cast<T*>(res);
}

// 読み込み済みのリソースを取得
template <class T>
T* CResourceManager::Get(std::string name)
{
	// リソースが読み込み済みでなければ、
	// nullptrを返す
	auto& list = Instance()->mResources;
	auto find = list.find(name);
	if (find == list.end()) return nullptr;

	// 読み込んでいるリソースを返す
	return dynamic_cast<T*>(find->second);
}

// リソース削除
void CResourceManager::Delete(std::string name)
{
	auto& list = Instance()->mResources;
	auto find = list.find(name);
	if (find == list.end()) return;

	list.erase(find);
	delete find->second;
}

// 指定したシーンに所属するリソースを全て削除
void CResourceManager::DeleteInScene(EScene scene)
{
	auto& list = Instance()->mResources;
	auto itr = list.begin();
	auto end = list.end();
	while (itr != end)
	{
		CResource* res = itr->second;
		if (res->mSceneType == scene)
		{
			itr = list.erase(itr);
			delete res;
		}
		else
		{
			itr++;
		}
	}
}
