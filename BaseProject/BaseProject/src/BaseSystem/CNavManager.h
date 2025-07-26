#pragma once
#include <vector>
#include "CTask.h"

class CNavNode;

// 経路探索処理の管理クラス
class CNavManager : public CTask
{
public:
	// 経路管理クラスのインスタンスを取得
	static CNavManager* Instance();

	// コンストラクタ
	CNavManager();
	// デストラクタ
	~CNavManager();

	// 経路単サック用のノードを追加
	void AddNode(CNavNode* node);
	// 経路探索用のノードを取り除く
	void RemoveNode(CNavNode* node);

	/// <summary>
	///	指定したノードに接続できるノードを検索して設定
	/// (間に遮蔽物があるノードには接続できない)
	/// </summary>
	/// <param name="node">接続できるノードを検索するノード</param>
	/// <param name="distance">接続できる距離の限界値</param>
	/// <returns>接続したノードの数</returns>
	void FindConnectNavNodes(CNavNode* node, float distance);

	/// <summary>
	/// 指定したノードに接続できるノードを検索して設定（即時検索版）
	/// </summary>
	/// <param name="node">接続できるノードを検索するノード</param>
	/// <param name="distance">接続できる距離の限界値</param>
	/// <returns>接続したノードの数</returns>
	int FindConnectNavNodesImmediate(CNavNode* node, float distance);

	/// <summary>
	/// 指定した開始ノードから目的地ノードまでの最短経路を求める
	/// </summary>
	/// <param name="start">開始ノード</param>
	/// <param name="goal">目的地ノード</param>
	/// <param name="route">最短経路返却用</param>
	/// <returns>経路が繋がっている場合は、trueを返す</returns>
	bool Navigate(CNavNode* start, CNavNode* goal, std::vector<CNavNode*>& route);

	/// <summary>
	/// 指定した経路が繋がっているかどうか
	/// </summary>
	/// <param name="route">繋がっているか確認する経路</param>
	/// <returns>trueならば、経路が繋がっている</returns>
	bool IsRouteValid(const std::vector<CNavNode*>& route) const;

	/// <summary>
	/// 遮蔽物チェックに使用するコライダーを追加
	/// </summary>
	/// <param name="col"></param>
	void AddCollider(CCollider* col);
	/// <summary>
	/// 遮蔽物チェックに使用するコライダーを取り除く
	/// </summary>
	/// <param name="col"></param>
	void RemoveCollider(CCollider* col);

	// 全てのノードを更新
	void Update() override;
	// 全てのノードと経路を描画
	void Render() override;

private:
	// 最短経路計算用のデータをリセット
	void ResetCalcData();
	/// <summary>
	/// 指定したノードから次のノードへの移動コストを計算
	/// </summary>
	/// <param name="node">移動コストを計算する開始ノード</param>
	/// <param name="goal">目的地のノード</param>
	void CalcNextMoveCost(CNavNode* node, CNavNode* goal);
	// 接続できるノードかどうか
	bool CanConnectNavNode(CNavNode* node, CNavNode* other, float distance) const;

	// 指定したノードを接続状態の更新待ちリストに追加
	void AddUpdateConnectNavNode(CNavNode* node, float distance);
	// 指定したノードを接続状態の更新待ちリストから取り除く
	void RemoveUpdateConnectNavNode(CNavNode* node);

	// 接続ノードを更新
	void UpdateConnectNavNode();

	// 削除するノードを調べる
	void CheckKillNode();

	// 経路管理クラスのインスタンスへのポインタ
	static CNavManager* spInstance;

	// 経路探索用のノードのリスト
	std::vector<CNavNode*> mNodes;

	// 経路探索のデバッグ表示を行うかどうか
	bool mIsRender;
	// 最後に計算した最短経路のデバッグ表示用
	std::vector<CNavNode*> mLastCalcRoute;

	// 遮蔽物チェックに使用するコライダーのリスト
	std::vector<CCollider*> mColliders;

	// 現在更新中のノード
	CNavNode* mpUpdateNode;
	float mFindNodeDistance;
	// 次の接続しているか調べるノードのインデックス値
	int mNextFindNodeIndex;

	// 接続状態更新ノードのデータ
	struct UpdateConnectNodeData
	{
		CNavNode* node;	// 更新するノード
		float distance;	// 判定するノード間の距離
	};
	// 接続状態の更新待ちリスト
	std::vector<UpdateConnectNodeData> mUpdateConnectNodes;
};