#pragma once
#include <list>

class CNavManager;
class CNavNode;

// 経路探索ノードの接続情報
class CNavConnectData
{
public:
	bool enabled;		// 有効かどうか
	CNavNode* node;		// 繋がっているノード
	float cost;			// 繋がっているノードまでの移動コスト
	bool forced;		// 強制的に繋ぐノードかどうか
	CNavConnectData(CNavNode* n, float c, bool f)
		: enabled(true), node(n), cost(c), forced(f) {}
};

// 経路探索ノードクラス
class CNavNode
{
	friend CNavManager;

public:
	// コンストラクタ
	CNavNode(const CVector& pos, bool isDestNode = false);

	// 有効状態を設定
	void SetEnable(bool enable);
	// 現在有効かどうか
	bool IsEnable() const;

	// 削除する
	void Kill();
	// 削除するかどうか
	bool IsKill() const;

	// ノードの座標を取得
	const CVector& GetPos() const;
	// 遮蔽物チェックや表示用に、少し上に上げたノードの座標を返す
	CVector GetOffsetPos() const;
	// ノードの座標を設定
	void SetPos(const CVector& pos, bool isInit = false);

	// 指定したノードと接続しているか
	bool IsConnectNode(CNavNode* node) const;

	// 指定したノードとの接続状態を設定
	void SetEnableConnect(CNavNode* node, bool enable);
	// 指定したノードとの接続状態を返す
	bool IsEnableConnect(CNavNode* node) const;

	// 接続するノード追加
	void AddConnect(CNavNode* node, bool forced = false);
	// 接続しているノードを取り除く
	void RemoveConnect(CNavNode* node);
	// 接続している全てのノードを解除
	void ClearConnects(bool forced = false);

	// 強制的に接続するノードを追加
	void AddForcedConnectNode(CNavNode* node);
	// 強制的に接続するノードを取り除く
	void RemoveForcedConnectNode(CNavNode* node);
	// 指定したノードが強制的に接続するノードかどうか
	bool IsForcedConnectNode(CNavNode* node) const;

	// 接続しないノードを追加
	void AddBlockedNode(CNavNode* node);
	// 接続しないノードを取り除く
	void RemoveBlockedNode(CNavNode* node);
	// 指定したノードが接続しないノードかどうか
	bool IsBlockedNode(CNavNode* node) const;

	// 接続しているノードを更新
	void UpdateConnectNode(bool immediate = false);
	// ノード更新中か
	bool IsUpdating() const;

	// ノードの色設定（デバッグ用）
	void SetColor(const CColor& color);
	// ノードを更新
	void Update();
	// ノードを描画（デバッグ用）
	void Render();

private:
	// デストラクタ
	~CNavNode();

	// 最短経路計算用のデータをリセット
	void ResetCalcData();

	// 接続ノードの更新開始時の呼び出す
	void StartUpdateConnectNode();
	// 接続ノードの更新終了時の呼び出す
	void EndUpdateConnectNode();

	// ノードが有効かどうか
	bool mIsEnable;
	// 削除するかどうか
	bool mIsKill;

	// 経路探索時に経由できない目的地専用のノードかどうか
	bool mIsDestNode;
	CVector mPosition;	// ノードの座標
	// 接続しているノードの情報リスト
	std::vector<CNavConnectData> mConnectData;
	// 遮蔽物チェックに関係なく接続するノードのリスト
	std::vector<CNavNode*> mForcedConnectNodes;
	// 接続しないノードのリスト
	std::vector<CNavNode*> mBlockedNodes;

	// 最短経路計算時に記憶しておく情報
	float mCalcMoveCost;		// 開始ノードからこのノードまでに掛かった移動コスト
	CNavNode* mpCalcFromNode;	// 最短経路での前のノードのポインタ
	bool mIsUpdateConnectNode;	// 接続ノードを更新する必要があるか
	bool mIsUpdaingConnectNode;	// 接続ノードを更新中か

	// デバッグ関連
	CColor mColor;		// ノードの色
};