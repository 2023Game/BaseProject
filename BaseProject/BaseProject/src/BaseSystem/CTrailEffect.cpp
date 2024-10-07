#include "CTrailEffect.h"

// コンストラクタ
CTrailEffect::CTrailEffect(ETag tag,
	CObjectBase* owner, CMatrix* mtx, CVector offsetPos,
	float updateInterval, float updateDist,
	float startWidth, float endWidth, float showTime)
	: CLineEffect(tag)
	, mpOwner(owner)
	, mpMatrix(mtx)
	, mOffsetPos(offsetPos)
	, mUpdateInterval(updateInterval)
	, mUpdateDistance(updateDist)
	, mStartWidth(startWidth)
	, mEndWidth(endWidth)
	, mShowTime(showTime)
	, mLastPos(0.0f, 0.0f, 0.0f)
	, mElapsedTime(0.0f)
{
	CVector pos = GetCurrentPos();
	AddPoint(pos, mStartWidth, mEndWidth);
	AddPoint(pos, mStartWidth, mEndWidth);
	mLastPos = pos;
}

// デストラクタ
CTrailEffect::~CTrailEffect()
{
}

// 持ち主を設定
void CTrailEffect::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
	UpdateLastPoint();
}

// 追跡する行列を設定
void CTrailEffect::SetFollowMatrix(CMatrix* mtx)
{
	mpMatrix = mtx;
	UpdateLastPoint();
}

//// 設定された持ち主または行列の位置からのオフセット値を設定
//void CTrailEffect::SetOffsetPos(const CVector& pos)
//{
//	mOffsetPos = pos;
//	UpdateLastPoint();
//	if (mPoints.size() == 1)
//	{
//		mLastPos = mPoints[0]->pos;
//	}
//}

// 現在地を取得
CVector CTrailEffect::GetCurrentPos() const
{
	CVector pos = Position();

	// 行列が設定されていたら、行列の座標を返す
	if (mpMatrix != nullptr)
	{
		pos = mpMatrix->Position();
	}
	// 持ち主が設定されていたら、持ち主の座標を返す
	else if (mpOwner != nullptr)
	{
		pos = mpOwner->Position();
	}

	return pos + mOffsetPos;
}

// 線の最後のポイントの位置を更新
void CTrailEffect::UpdateLastPoint()
{
	if (mpOwner == nullptr && mpMatrix == nullptr) return;

	int index = mPoints.size() - 1;
	if (index < 0) return;

	SetPos(index, GetCurrentPos());
}

// 更新
void CTrailEffect::Update()
{
	if (IsKill()) return;

	CLineEffect::Update();

	// 行列か持ち主が設定されていれば、現在地を更新
	if (mpMatrix != nullptr || mpOwner != nullptr)
	{
		// 現在地を取得
		CVector pos = GetCurrentPos();
		// 前回のポイントから一定距離離れていたら、
		float dist = (pos - mLastPos).Length();

		// 更新間隔か更新距離を超えたら、
		if (mElapsedTime >= mUpdateInterval || dist >= mUpdateDistance)
		{
			// 新しいポイントを追加
			AddPoint(pos, mStartWidth, mEndWidth);
			mLastPos = pos;
			mElapsedTime = std::max(mElapsedTime - mUpdateInterval, 0.0f);
		}
		// 更新タイミングでなければ、最後のポイントの位置を更新
		else
		{
			UpdateLastPoint();
		}
		// 更新間隔用の経過時間を加算
		mElapsedTime += Time::DeltaTime();
	}

	// 線のポイントの数が1個以下かつ、
	// 持ち主も行列も設定されていない状態であれば、
	int size = mPoints.size();
	if (size <= 1 && mpOwner == nullptr && mpMatrix == nullptr)
	{
		// エフェクトを削除
		Kill();
		return;
	}

	std::list<int> removes;
	// 各ポイントの更新
	for (int i = 0; i < size; i++)
	{
		CLinePoint* point = mPoints[i];
		if (point == nullptr) continue;
		// 表示時間に合わせて、徐々に透過していく
		if (point->elapsed < mShowTime)
		{
			float alpha = point->elapsed / mShowTime;
			point->alpha = 1.0f - alpha;

			// 表示時間に経過時間を加算
			point->elapsed += Time::DeltaTime();
		}
		// 表示時間が終われば、取り除くリストに追加
		else
		{
			point->alpha = 0.0f;
			removes.push_back(i);
		}
	}

	// 取り除くリストに登録されたポイントを取り除く
	for (int i : removes)
	{
		RemovePoint(i);
	}
}
