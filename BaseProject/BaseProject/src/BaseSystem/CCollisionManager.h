#pragma once
#include <list>
#include "CCollider.h"

/// <summary>
/// 衝突処理の管理クラス
/// </summary>
class CCollisionManager
{
public:
	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static CCollisionManager* Instance();
	// インスタンスの破棄
	static void ClearInstance();

	/// <summary>
	/// 指定したコライダーをリストに追加
	/// </summary>
	/// <param name="col">追加するコライダー</param>
	void Add(CCollider* col);
	/// <summary>
	/// 指定したコライダーをリストから取り除く
	/// </summary>
	/// <param name="col">取り除くコライダー</param>
	void Remove(CCollider* col);

	/// <summary>
	/// 指定した2つのコライダーの衝突処理を行う
	/// </summary>
	/// <param name="col0">衝突処理を行うコライダー 1つ目</param>
	/// <param name="col1">衝突処理を行うコライダー 2つ目</param>
	void Collision(CCollider* col0, CCollider* col1);
	/// <summary>
	/// 指定したコライダーと他の全てのコライダーとの衝突処理を行う
	/// </summary>
	/// <param name="col">衝突処理を行うコライダー</param>
	void Collision(CCollider* col);
	/// <summary>
	/// 全てのコライダーの衝突処理を行う
	/// </summary>
	void CollisionAll();

	// 全コライダーを描画
	void Render();

private:
	// コンストラクタ
	CCollisionManager();
	// デストラクタ
	~CCollisionManager();

	// コリジョンマネージャのインスタンス
	static CCollisionManager* mpInstance;
	// コライダーのリスト
	std::list<CCollider*> mColliderList;
	// コライダーを描画するかどうか
	bool mIsShowCollider;
};
