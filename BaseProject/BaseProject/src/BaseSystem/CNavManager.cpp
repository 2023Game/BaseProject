#include "CNavManager.h"
#include "CNavNode.h"
#include "CInput.h"
#include "Primitive.h"
#include <assert.h>

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
	std::remove(mNodes.begin(), mNodes.end(), node);
}

// 指定したノードに接続できるノードを検索して設定
int CNavManager::FindConnectNavNodes(CNavNode* node, float distance)
{
	// 現在の接続先の情報を全てクリアしておく
	node->ClearConnects();

	for (CNavNode* findNode : mNodes)
	{
		//自分自身であれば、スルー
		if (findNode == node) continue;

		// 目的地専用ノードは距離を考慮しない
		if (!node->mIsDestNode)
		{
			// 指定された距離の限界値を超える場合は、スルー
			float dist = (findNode->GetPos() - node->GetPos()).Length();
			if (dist > distance) continue;
		}
		
		// 自身から接続先のノードまでの遮蔽物チェック
		CVector start = node->GetOffsetPos();
		CVector end = findNode->GetOffsetPos();
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
		if (isHit) continue;;

		// 両方の条件を満たしたノードを接続リストに追加
		node->AddConnect(findNode);

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
		if (!connect.node->IsEnabel()) continue;

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
	if (!start->IsEnabel() || !goal->IsEnabel()) return false;

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

// 遮蔽物チェックに使用するコライダーを追加
void CNavManager::AddCollider(CCollider* col)
{
	mColliders.push_back(col);
}

// 遮蔽物チェックに使用するコライダーを取り除く
void CNavManager::RemoveCollider(CCollider* col)
{
	std::remove(mColliders.begin(), mColliders.end(), col);
}

// 全てのノードと経路を描画
void CNavManager::Render()
{
#if _DEBUG
	// [SPACE]キーで経路探索ノードの描画モードを切り替え
	if (CInput::PushKey('N'))
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
	glDisable(GL_DEPTH_TEST);
	int size = mLastCalcRoute.size();
	for (int i = 0; i < size - 1; i++)
	{
		CVector start = mLastCalcRoute[i]->GetOffsetPos();
		CVector end = mLastCalcRoute[i + 1]->GetOffsetPos();
		Primitive::DrawLine(start, end, CColor::cyan, 4.0f);
	}
	glEnable(GL_DEPTH_TEST);
#endif
}

