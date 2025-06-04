#include "CNavNode.h"
#include "CNavManager.h"
#include "Primitive.h"

// ノードのY座標のオフセット値
#define NODE_OFFSET_Y 5.0f
// 探すノードの距離の限界値
#define FIND_NODE_DISTANCE 180.0f

// コンストラクタ
CNavNode::CNavNode(const CVector& pos, bool isDestNode)
	: mIsEnable(true)
	, mIsDestNode(isDestNode)
	, mPosition(pos)
	, mCalcMoveCost(-1.0f)
	, mpCalcFromNode(nullptr)
	, mColor(0.0f, 1.0f, 0.0f, 1.0f)
{
	// 管理クラスのリストに自身を追加
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		navMgr->AddNode(this);
	}

	// 座標を設定
	SetPos(mPosition);
}

// デストラクタ
CNavNode::~CNavNode()
{
	// 管理クラスのリストから自身を取り除く
	// 管理クラスのリストに自身を追加
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		navMgr->RemoveNode(this);
	}
}

// 有効状態を設定
void CNavNode::SetEnable(bool enable)
{
	mIsEnable = enable;
}

// 現在有効かどうか
bool CNavNode::IsEnabel() const
{
	return mIsEnable;
}

// 最短経路計算用のデータをリセット
void CNavNode::ResetCalcData()
{
	mCalcMoveCost = -1.0;
	mpCalcFromNode = nullptr;
}


// ノードの座標を取得
const CVector& CNavNode::GetPos() const
{
	return mPosition;
}

// 遮蔽物チェックや表示用に、少し上に上げたノードの座標を返す
CVector CNavNode::GetOffsetPos() const
{
	// 地面と重ならないように、ノードの位置から少し上げた位置を返す
	return mPosition + CVector(0.0f, NODE_OFFSET_Y, 0.0f);
}

// ノードの座標を設定
void CNavNode::SetPos(const CVector& pos)
{
	// ノードの座標を更新
	mPosition = pos;

	// 座標を変更したので、接続ノードを再検索
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		navMgr->FindConnectNavNodes(this, FIND_NODE_DISTANCE);
	}
}

// 接続するノード追加
void CNavNode::AddConnect(CNavNode* node)
{
	for (CNavConnectData& connect : mConnectData)
	{
		if (connect.node == node) return;
	}

	// 接続するノードまでの距離をコストとする
	float cost = (node->GetPos() - mPosition).Length();

	// 自身と相手それぞれの接続しているノードリストにお互いを設定
	mConnectData.push_back(CNavConnectData(node, cost));
	node->mConnectData.push_back(CNavConnectData(this, cost));
}

// 接続しているノードを取り除く
void CNavNode::RemoveConnect(CNavNode* node)
{
	auto result = std::remove_if
	(
		mConnectData.begin(), mConnectData.end(),
		[node](const CNavConnectData& x) { return x.node == node; }
	);
	mConnectData.erase(result, mConnectData.end());
}

// 接続している全てのノードを解除
void CNavNode::ClearConnects()
{
	// 接続相手の接続リストから自身を取り除く
	for (CNavConnectData& connect : mConnectData)
	{
		connect.node->RemoveConnect(this);
	}
	// 自身の接続リストをクリア
	mConnectData.clear();
}

// ノードの色設定（デバッグ用）
void CNavNode::SetColor(const CColor& color)
{
	mColor.Set(color.R(), color.G(), color.B());
}

// ノードを描画（デバッグ用）
void CNavNode::Render()
{
	// ノードが有効状態出なければ、描画しない
	if (!mIsEnable) return;

	// 接続先のノードまでのラインを描画
	for (CNavConnectData& connect : mConnectData)
	{
		// 接続先のノードが無効であれば、スルー
		if (!connect.node->IsEnabel()) continue;

		Primitive::DrawLine
		(
			GetOffsetPos(),
			connect.node->GetOffsetPos(),
			CColor(0.11f, 0.1f, 0.1f, 1.0f),
			2.0f
		);
	}

	// ノード座標に四角形を描画
	Primitive::DrawWireBox
	(
		GetOffsetPos(),
		CVector::one,
		mColor
	);
}

