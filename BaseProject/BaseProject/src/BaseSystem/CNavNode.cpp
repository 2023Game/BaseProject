#include "CNavNode.h"
#include "CNavManager.h"
#include "Primitive.h"

// ノードのY座標のオフセット値
#define NODE_OFFSET_Y 5.0f
// 探すノードの距離の限界値
#define FIND_NODE_DISTANCE 180.0f
// ノードの位置を更新する距離
#define UPDATE_DISTANCE 1.0f

// コンストラクタ
CNavNode::CNavNode(const CVector& pos, bool isDestNode)
	: mIsEnable(true)
	, mIsKill(false)
	, mIsDestNode(isDestNode)
	, mPosition(pos)
	, mCalcMoveCost(-1.0f)
	, mpCalcFromNode(nullptr)
	, mIsUpdateConnectNode(false)
	, mIsUpdaingConnectNode(false)
	, mColor(0.0f, 1.0f, 0.0f, 1.0f)
{
	// 管理クラスのリストに自身を追加
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		navMgr->AddNode(this);
	}

	// 座標を設定
	SetPos(mPosition, true);
}

// デストラクタ
CNavNode::~CNavNode()
{
	// 探索ノードを削除する前に、自身と接続しているノードを取り除く
	ClearConnects(true);
}

// 有効状態を設定
void CNavNode::SetEnable(bool enable)
{
	mIsEnable = enable;
}

// 現在有効かどうか
bool CNavNode::IsEnable() const
{
	return mIsEnable;
}

// 削除する
void CNavNode::Kill()
{
	mIsKill = true;
}

// 削除するかどうか
bool CNavNode::IsKill() const
{
	return mIsKill;
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
void CNavNode::SetPos(const CVector& pos, bool isInit)
{
	// 初期化時でなければ
	if (!isInit)
	{
		// 位置が変わってなければ、更新しない
		if (pos == mPosition) return;

		// ノードの位置の変化が更新する距離より短い場合は、更新しない
		float dist = (pos - mPosition).LengthSqr();
		if (dist < UPDATE_DISTANCE * UPDATE_DISTANCE) return;
	}

	// ノードの座標を更新
	mPosition = pos;

	// 初期化時は、即時接続ノードを更新
	if (isInit)
	{
		UpdateConnectNode(true);
	}
	// それ以降は、更新できるタイミングに更新
	else
	{
		// 接続ノード更新フラグを立てる
		mIsUpdateConnectNode = true;
	}
}

// 指定したノードと接続しているか
bool CNavNode::IsConnectNode(CNavNode* node) const
{
	// 自身か、指定したノードが無効な状態であれば、接続していない
	if (!IsEnable() || !node->IsEnable()) return false;

	// 指定したノードが接続されているか確認
	auto find = std::find_if
	(
		mConnectData.begin(), mConnectData.end(),
		[node](const CNavConnectData& x) { return x.node == node; }
	);
	if (find != mConnectData.end())
	{
		// 接続されているかつ、接続状態が有効であれば、接続している
		if (find->enabled) return true;
	}

	// 接続されていなかった
	return false;
}

// 指定したノードとの接続状態を設定
void CNavNode::SetEnableConnect(CNavNode* node, bool enable)
{
	// 指定したノードが接続されているか確認
	auto find = std::find_if
	(
		mConnectData.begin(), mConnectData.end(),
		[node](const CNavConnectData& x) { return x.node == node; }
	);
	// 接続されていたら、接続状態を変更
	if (find != mConnectData.end())
	{
		find->enabled = enable;
	}
}

// 指定したノードとの接続状態を返す
bool CNavNode::IsEnableConnect(CNavNode* node) const
{
	// 指定したノードが接続されているか確認
	auto find = std::find_if
	(
		mConnectData.begin(), mConnectData.end(),
		[node](const CNavConnectData& x) { return x.node == node; }
	);
	if (find == mConnectData.end()) return false;
	return find->enabled;
}

// 接続するノード追加
void CNavNode::AddConnect(CNavNode* node, bool forced)
{
	for (CNavConnectData& connect : mConnectData)
	{
		if (connect.node == node) return;
	}

	// 接続するノードまでの距離をコストとする
	float cost = (node->GetPos() - mPosition).Length();

	// 自身と相手それぞれの接続しているノードリストにお互いを設定
	mConnectData.push_back(CNavConnectData(node, cost, forced));
	node->mConnectData.push_back(CNavConnectData(this, cost, forced));
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
void CNavNode::ClearConnects(bool forced)
{
	// 接続相手の接続リストから自身を取り除く
	for (CNavConnectData& connect : mConnectData)
	{
		if (!forced && connect.forced) continue;
		connect.node->RemoveConnect(this);
	}

	// 強制的に接続を解除するのであれば
	if (forced)
	{
		// 自身の接続リストをクリア
		mConnectData.clear();
	}
	// 強制でなければ
	else
	{
		// 強制的に接続するノード以外の接続状態を解除
		auto result = std::remove_if
		(
			mConnectData.begin(), mConnectData.end(),
			[](const CNavConnectData& x) { return !x.forced; }
		);
		mConnectData.erase(result, mConnectData.end());
	}
}

// 強制的に接続するノードを追加
void CNavNode::AddForcedConnectNode(CNavNode* node)
{
	mForcedConnectNodes.push_back(node);
	AddConnect(node, true);

	node->mForcedConnectNodes.push_back(this);
	node->AddConnect(this, true);
}

// 強制的に接続するノードを取り除く
void CNavNode::RemoveForcedConnectNode(CNavNode* node)
{
	auto result = std::remove(mForcedConnectNodes.begin(), mForcedConnectNodes.end(), node);
	mForcedConnectNodes.erase(result, mForcedConnectNodes.end());
	RemoveConnect(node);

	result = std::remove(node->mForcedConnectNodes.begin(), node->mForcedConnectNodes.end(), this);
	node->mForcedConnectNodes.erase(result, node->mForcedConnectNodes.end());
	node->RemoveConnect(this);
}

// 指定したノードが強制的に接続するノードかどうか
bool CNavNode::IsForcedConnectNode(CNavNode* node) const
{
	auto find = std::find(mForcedConnectNodes.begin(), mForcedConnectNodes.end(), node);
	return find != mForcedConnectNodes.end();
}

// 接続しないノードを追加
void CNavNode::AddBlockedNode(CNavNode* node)
{
	mBlockedNodes.push_back(node);
	node->mBlockedNodes.push_back(this);
}

// 接続しないノードを取り除く
void CNavNode::RemoveBlockedNode(CNavNode* node)
{
	auto result = std::remove(mBlockedNodes.begin(), mBlockedNodes.end(), node);
	mBlockedNodes.erase(result, mBlockedNodes.end());

	result = std::remove(node->mBlockedNodes.begin(), node->mBlockedNodes.end(), this);
	node->mBlockedNodes.erase(result, node->mBlockedNodes.end());
}

// 接続しないノードかどうか
bool CNavNode::IsBlockedNode(CNavNode* node) const
{
	auto find = std::find(mBlockedNodes.begin(), mBlockedNodes.end(), node);
	return find != mBlockedNodes.end();
}

// 接続しているノードを更新
void CNavNode::UpdateConnectNode(bool immediate)
{
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr == nullptr) return;

	if (immediate)
	{
		navMgr->FindConnectNavNodesImmediate(this, FIND_NODE_DISTANCE);
	}
	else
	{
		if (mIsUpdaingConnectNode) return;
		navMgr->FindConnectNavNodes(this, FIND_NODE_DISTANCE);
	}

	mIsUpdateConnectNode = false;
}


// 接続ノードの更新開始時の呼び出す
void CNavNode::StartUpdateConnectNode()
{
	mIsUpdaingConnectNode = true;
}

// 接続ノードの更新終了時の呼び出す
void CNavNode::EndUpdateConnectNode()
{
	mIsUpdaingConnectNode = false;
}

// ノード更新中か
bool CNavNode::IsUpdating() const
{
	if (mIsUpdateConnectNode) return true;
	if (mIsUpdaingConnectNode) return true;
	return false;
}

// ノードの色設定（デバッグ用）
void CNavNode::SetColor(const CColor& color)
{
	mColor.Set(color.R(), color.G(), color.B());
}

// ノードを更新
void CNavNode::Update()
{
	// 接続ノード更新フラグが立っていたら、更新
	if (mIsUpdateConnectNode)
	{
		UpdateConnectNode();
	}
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
		if (!connect.node->IsEnable()) continue;

		CColor color = CColor(0.11f, 0.1f, 0.1f, 1.0f);
		// 接続しているが、現在接続状態を解除されている場合は、赤色のラインに変更
		if (!connect.enabled) color = CColor(1.0f, 0.0f, 0.0f, 1.0f);
		Primitive::DrawLine
		(
			GetOffsetPos(),
			connect.node->GetOffsetPos(),
			color,
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

