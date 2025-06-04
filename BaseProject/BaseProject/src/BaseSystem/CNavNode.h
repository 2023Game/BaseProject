#pragma once
#include <list>

class CNavManager;
class CNavNode;

// 経路探索ノードの接続情報
class CNavConnectData
{
public:
	CNavNode* node;		// 繋がっているノード
	float cost;			// 繋がっているノードまでの移動コスト
	CNavConnectData(CNavNode* n, float c)
		: node(n), cost(c) {}
};

// 経路探索ノードクラス
class CNavNode
{
	friend CNavManager;

public:
	// コンストラクタ
	CNavNode(const CVector& pos, bool isDestNode = false);
	// デストラクタ
	~CNavNode();

	// 有効状態を設定
	void SetEnable(bool enable);
	// 現在有効かどうか
	bool IsEnabel() const;

	// ノードの座標を取得
	const CVector& GetPos() const;
	// 遮蔽物チェックや表示用に、少し上に上げたノードの座標を返す
	CVector GetOffsetPos() const;
	// ノードの座標を設定
	void SetPos(const CVector& pos);

	// 接続するノード追加
	void AddConnect(CNavNode* node);
	// 接続しているノードを取り除く
	void RemoveConnect(CNavNode* node);
	// 接続している全てのノードを解除
	void ClearConnects();

	// ノードの色設定（デバッグ用）
	void SetColor(const CColor& color);
	// ノードを描画（デバッグ用）
	void Render();
	
private:
	// 最短経路計算用のデータをリセット
	void ResetCalcData();

	// ノードが有効かどうか
	bool mIsEnable;

	// 経路探索時に経由できない目的地専用のノードかどうか
	bool mIsDestNode;
	CVector mPosition;	// ノードの座標
	// 接続しているノードの情報リスト
	std::vector<CNavConnectData> mConnectData;

	// 最短経路計算時に記憶しておく情報
	float mCalcMoveCost;		// 開始ノードからこのノードまでに掛かった移動コスト
	CNavNode* mpCalcFromNode;	// 最短経路での前のノードのポインタ

	// デバッグ関連
	CColor mColor;		// ノードの色
};