#pragma once
#include <string>
#include <map>
#include "CResource.h"

// リソース管理クラス
class CResourceManager
{
public:
	// インスタンスを破棄
	static void ClearInstance();

	// リソース読み込み
	template <class T>
	static T* Load(std::string name, std::string path, bool dontDelete = false);
	// 読み込み済みのリソースを取得
	template <class T>
	static T* Get(std::string name);

	// リソース削除
	static void Delete(std::string name);
	// 指定したシーンに所属するリソースを全て削除
	static void DeleteInScene(EScene scene);

private:
	// コンストラクタ
	CResourceManager();
	// デストラクタ
	~CResourceManager();

	// インスタンスを取得
	static CResourceManager* Instance();

	// CResourceManagerのインスタンス
	static CResourceManager* mpInstance;
	// 読み込み済みのリソースのリスト
	std::map<std::string, CResource*> mResources;
};
