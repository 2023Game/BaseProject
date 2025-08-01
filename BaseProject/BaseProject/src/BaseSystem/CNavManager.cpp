#include "CNavManager.h"
#include "CNavNode.h"
#include "CDebugInput.h"
#include "Primitive.h"
#include <assert.h>

// 1フレームで調べる接続ノードの数
#define FIND_CONNECT_NODE_COUNT 10

CNavManager* CNavManager::spInstance = nullptr;

// 経路管理クラスのインスタンスを取得
CNavManager* CNavManager::Instance()
{
	return spInstance;
}

// コンストラクタ
CNavManager::CNavManager()
	: CTask(ETaskPriority::eNavigation, 0, ETaskPauseType::eGame)
	, mIsRender(false)
	, mpUpdateNode(nullptr)
	, mFindNodeDistance(0.0f)
	, mNextFindNodeIndex(-1)
{
	assert(spInstance == nullptr);
	spInstance = this;
}

// デストラクタ
CNavManager::~CNavManager()
{
	spInstance = nullptr;

	auto itr = mNodes.begin();
	auto end = mNodes.end();
	while (itr != end)
	{
		CNavNode* del = *itr;
		itr++;
		delete del;
	}
}

// 経路単サック用のノードを追加
void CNavManager::AddNode(CNavNode* node)
{
	mNodes.push_back(node);
}

// 経路探索用のノードを取り除く
void CNavManager::RemoveNode(CNavNode* node)
{
	auto result = std::remove(mNodes.begin(), mNodes.end(), node);
	mNodes.erase(result, mNodes.end());
}

// 指定したノードを接続状態の更新待ちリストに追加
void CNavManager::AddUpdateConnectNavNode(CNavNode* node, float distance)
{
	// 既に更新待ちリストに登録されてなければ、リストに追加
	auto find = std::find_if
	(
		mUpdateConnectNodes.begin(), mUpdateConnectNodes.end(),
		[node](const UpdateConnectNodeData& x) { return x.node == node; }
	);
	if (find == mUpdateConnectNodes.end())
	{
		mUpdateConnectNodes.push_back({ node, distance });
	}
}

// 指定したノードを接続状態の更新待ちリストから取り除く
void CNavManager::RemoveUpdateConnectNavNode(CNavNode* node)
{
	// リスト内に見つかれば、削除
	auto result = std::remove_if
	(
		mUpdateConnectNodes.begin(), mUpdateConnectNodes.end(),
		[node](const UpdateConnectNodeData& x) { return x.node == node; }
	);
	if (result != mUpdateConnectNodes.end())
	{
		mUpdateConnectNodes.erase(result);
	}
}

// 指定したノードに接続できるノードを検索して設定
void CNavManager::FindConnectNavNodes(CNavNode* node, float distance)
{
	// 削除予定のノードであれば、処理しない
	if (node->IsKill()) return;

	// 現在更新中のノードが存在する場合は
	if (mpUpdateNode != nullptr)
	{
		// 更新待ちリストに追加
		AddUpdateConnectNavNode(node, distance);
		mpUpdateNode->StartUpdateConnectNode();
		return;
	}

	mpUpdateNode = node;
	mFindNodeDistance = distance;
	mNextFindNodeIndex = 0;

	mpUpdateNode->ClearConnects();
	mpUpdateNode->StartUpdateConnectNode();
}

// 接続できるノードかどうか
bool CNavManager::CanConnectNavNode(CNavNode* node, CNavNode* other, float distance) const
{
	// 自分自身であれば、スルー
	if (node == other) return false;
	// 接続相手のノードが削除予定であれば、スルー
	if (other->IsKill()) return false;
	// 強制的に接続するノードであれば、スルー
	if (node->IsForcedConnectNode(other)) return false;
	// 接続しないノードであれば、スルー
	if (node->IsBlockedNode(other)) return false;

	// 目的地専用ノードは距離を考慮しない
	//if (!node->mIsDestNode)
	{
		// 指定された距離の限界値を超える場合は、スルー
		float dist = (other->GetPos() - node->GetPos()).LengthSqr();
		if (dist > distance * distance) return false;
	}

	// 自身から接続先のノードまでの遮蔽物チェック
	CVector start = node->GetOffsetPos();
	CVector end = other->GetOffsetPos();
	CHitInfo hit;
	bool isHit = false;
	// 登録されているコライダー全てと判定
	for (CCollider* col : mColliders)
	{
		// ヒットしていたら、ヒットフラグをtrueにしてチェック終了
		if (CCollider::CollisionRay(col, start, end, &hit))
		{
			isHit = true;
			break;
		}
	}
	// 何かにヒットした場合は、遮蔽物があるので接続できない
	if (isHit) return false;

	return true;
}

// 指定したノードに接続できるノードを検索して設定（即時検索版）
int CNavManager::FindConnectNavNodesImmediate(CNavNode* node, float distance)
{
	// 現在の接続先の情報を全てクリアしておく
	node->ClearConnects();

	// 全てのノードをチェック
	for (CNavNode* findNode : mNodes)
	{
		// 接続できるノードであれば、接続リストに追加
		if (CanConnectNavNode(node, findNode, distance))
		{
			node->AddConnect(findNode);
		}
	}

	return node->mConnectData.size();
}

// 最短経路計算用のデータをリセット
void CNavManager::ResetCalcData()
{
	for (CNavNode* node : mNodes)
	{
		node->ResetCalcData();
	}
}

// 最短経路計算用のデータをリセット
void CNavManager::CalcNextMoveCost(CNavNode* node, CNavNode* goal)
{
	// 指定されたノードまたは目的地ノードが空だった場合は、移動コスト計算不可
	if (node == nullptr || goal == nullptr) return;

	// 接続している全てのノードへの移動コストを求める
	for (CNavConnectData& connect : node->mConnectData)
	{
		// 接続先のノードが無効であれば、スルー
		if (!connect.node->IsEnable()) continue;
		// 接続先のノードが削除予定であれば、スルー
		if (connect.node->IsKill()) continue;
		// 接続解除されている状態であれば、スルー
		if (!connect.enabled) continue;

		// 接続しているノードが目的地専用ノードの場合は、
		// 今回の経路探索の目的地ノード以外は経由しないため、スルー
		if (connect.node->mIsDestNode && connect.node != goal) continue;

		// 移動コスト = ここまでの移動コスト + 接続先のノードまでの移動コスト
		float cost = node->mCalcMoveCost + connect.cost;
		// 接続先のノードまでの移動コストがまだ計算されていないか、
		// 求めた移動コストの方が現在の接続先のノードまでの移動コストより小さい場合
		if (connect.node->mCalcMoveCost < 0 || cost < connect.node->mCalcMoveCost)
		{
			// 接続先のノードの移動コストと1つ前のノードのポインタを更新
			connect.node->mCalcMoveCost = cost;
			connect.node->mpCalcFromNode = node;
			// 接続先のノードが目的地のノードでなければ、
			// 接続先のノードから接続されているノードへの移動コストを計算する
			if (connect.node != goal)
			{
				CalcNextMoveCost(connect.node, goal);
			}
		}
	}
}

// 指定した開始ノードから目的地ノードまでの最短経路を求める
bool CNavManager::Navigate(CNavNode* start, CNavNode* goal, std::vector<CNavNode*>& route)
{
	// 開始ノードまたは目的地ノードが空だった場合は、経路探索不可
	if (start == nullptr || goal == nullptr)return false;
	// 開始ノードまたは目的地ノードが無効だった場合は、経路探索不可
	if (!start->IsEnable() || !goal->IsEnable()) return false;
	// 開始ノードまたは目的地ノードが削除予定だった場合は、経路探索不可
	if (start->IsKill() || goal->IsKill()) return false;

	// 全てのノードの最短経路計算用のデータをクリア
	ResetCalcData();

	// 開始ノードへ移動コストを0に設定
	start->mCalcMoveCost = 0.0f;
	// 開始ノードから順番に、
	// 各ノードが接続しているノードまでの移動コストを計算
	CalcNextMoveCost(start, goal);

	// 最短経路のリストをクリアして最初に目的地ノードを登録
	route.clear();
	route.push_back(goal);

	// 目的地ノードから順番に、
	// 最短経路での1つ前のノードが存在する限りループして、
	// 最短経路のリストにノードを登録していく
	CNavNode* node = goal;
	while (node->mpCalcFromNode != nullptr)
	{
		route.insert(route.begin(), node->mpCalcFromNode);
		node = node->mpCalcFromNode;
	}

	// 求めた最短経路をデバッグ表示用の最短経路リストにコピー
	mLastCalcRoute = route;

	// 求め最短経路の1バン最初のノードが開始ノードであれば、
	// 開始ノードから目的地ノードまで経路が繋がっている
	return route[0] == start;
}

// 指定した経路が繋がっているかどうか
bool CNavManager::IsRouteValid(const std::vector<CNavNode*>& route) const
{
	// 指定した経路に2つ以上ノードが登録されていないと
	// 経路と見なさないため、経路は繋がっていないと判定する
	int size = route.size();
	if (size <= 1) return false;

	// 指定した経路のノードを先頭から順番に繋がっているか確認
	for (int i = 0; i < size - 1; i++)
	{
		// 現在のノードと次のノードが繋がっていなければ、経路が繋がっていない
		if (!route[i]->IsConnectNode(route[i + 1])) return false;
	}

	// 経路が繋がっていた
	return true;
}

// 遮蔽物チェックに使用するコライダーを追加
void CNavManager::AddCollider(CCollider* col)
{
	mColliders.push_back(col);
}

// 遮蔽物チェックに使用するコライダーを取り除く
void CNavManager::RemoveCollider(CCollider* col)
{
	auto result = std::remove(mColliders.begin(), mColliders.end(), col);
	mColliders.erase(result, mColliders.end());
}

// 接続ノードを更新
void CNavManager::UpdateConnectNavNode()
{
	// 更新中のノードが存在しない場合は処理しない
	if (mpUpdateNode == nullptr) return;

	// 1フレームで一定数のノードと接続しているかを確認
	int nodeCount = mNodes.size();
	for (int i = 0; i < FIND_CONNECT_NODE_COUNT; i++)
	{
		CNavNode* findNode = mNodes[mNextFindNodeIndex];
		// 接続できるノードであれば、リストに追加
		if (CanConnectNavNode(mpUpdateNode, findNode, mFindNodeDistance))
		{
			mpUpdateNode->AddConnect(findNode);
		}

		// 次のノードを調べる
		mNextFindNodeIndex++;
		if (mNextFindNodeIndex >= nodeCount) break;
	}

	// 最後まで調べたか
	if (mNextFindNodeIndex >= nodeCount)
	{
		// 接続ノード更新処理が終わったことをノードに伝える
		mpUpdateNode->EndUpdateConnectNode();

		// 更新待ちリストから取り除く
		RemoveUpdateConnectNavNode(mpUpdateNode);

		// ノードの更新が終わったタイミングでも、
		// 削除フラグが立ったノードを削除する
		CheckKillNode();

		// 更新待ちリストに更新待ちのノードが存在したら
		if (mUpdateConnectNodes.size() > 0)
		{
			// 一番先頭のノードを更新対象に設定
			const UpdateConnectNodeData& data = mUpdateConnectNodes[0];
			mpUpdateNode = data.node;
			mFindNodeDistance = data.distance;
			mNextFindNodeIndex = 0;

			mpUpdateNode->ClearConnects();
			mpUpdateNode->StartUpdateConnectNode();
		}
		// 更新待ちノードが存在しない
		else
		{
			// 更新関連の変数をリセット
			mpUpdateNode = nullptr;
			mNextFindNodeIndex = -1;
		}
	}
}

// 削除するノードを調べる
void CNavManager::CheckKillNode()
{
	auto itr = mNodes.begin();
	while (itr != mNodes.end())
	{
		CNavNode* node = *itr;
		if (node->IsKill())
		{
			RemoveUpdateConnectNavNode(node);
			delete node;
			itr = mNodes.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

// 全てのノードを更新
void CNavManager::Update()
{
	// 更新中のノードがなければ、
	// 削除フラグが立っているノードを削除
	if (mpUpdateNode != nullptr)
	{
		CheckKillNode();
	}

	// リスト内のノード全て更新
	for (CNavNode* node : mNodes)
	{
		node->Update();
	}

	// 現在更新中のノードの接続状態を更新
	UpdateConnectNavNode();
}

// 全てのノードと経路を描画
void CNavManager::Render()
{
#if _DEBUG
	// [SPACE]キーで経路探索ノードの描画モードを切り替え
	if (CDebugInput::PushKey('N'))
	{
		mIsRender = !mIsRender;
	}

	// 経路探索ノードを描画しないならば、移行処理しない
	if (!mIsRender) return;

	// リスト内のノード全て描画
	for (CNavNode* node : mNodes)
	{
		node->Render();
	}

	// 最後に求めた最短経路にラインを引く
	//glDisable(GL_DEPTH_TEST);
	//int size = mLastCalcRoute.size();
	//for (int i = 0; i < size - 1; i++)
	//{
	//	CVector start = mLastCalcRoute[i]->GetOffsetPos();
	//	CVector end = mLastCalcRoute[i + 1]->GetOffsetPos();
	//	Primitive::DrawLine(start, end, CColor::cyan, 4.0f);
	//}
	//glEnable(GL_DEPTH_TEST);
#endif
}

