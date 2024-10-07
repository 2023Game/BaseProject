#include "CCollider.h"
#include "CCollisionManager.h"
#include "CColliderLine.h"
#include "CColliderSphere.h"
#include "CColliderTriangle.h"
#include "CColliderCapsule.h"
#include "CColliderMesh.h"
#include "CObjectBase.h"
#include "Maths.h"

// コンストラクタ
CCollider::CCollider(CObjectBase* owner, ELayer layer, EColliderType type,
	bool isKinematic, float weight)
	: mLayer(layer)
	, mType(type)
	, mpOwner(owner)
	, mIsEnable(true)
	, mIsKinematic(isKinematic)
	, mWeight(weight)
	, mCollisionLayers(~0)
	, mCollisionTags(~0)
	, mpAttachMtx(nullptr)
{
	// コリジョンリストに追加
	CCollisionManager::Instance()->Add(this);
}

// デストラクタ
CCollider::~CCollider()
{
	// コリジョンリストから削除
	CCollisionManager::Instance()->Remove(this);
}

// 衝突判定レイヤーを取得
ELayer CCollider::Layer() const
{
	return mLayer;
}

// コライダーの種類を取得
EColliderType CCollider::Type() const
{
	return mType;
}

// コライダーの持ち主を返す
CObjectBase* CCollider::Owner() const
{
	return mpOwner;
}

// コライダーの持ち主のタグを取得
ETag CCollider::Tag() const
{
	// 持ち主が存在すれば、持ち主のタグを返す
	if (mpOwner == nullptr) return ETag::eNone;
	return mpOwner->Tag();
}

// コライダーが有効かどうかを設定
void CCollider::SetEnable(bool isEnable)
{
	mIsEnable = isEnable;
}

// コライダーが有効かどうかを取得
bool CCollider::IsEnable() const
{
	return mIsEnable;
}

// 衝突時の押し戻しの影響を受けるかどうかを設定
void CCollider::SetKinematic(bool iskinematic)
{
	mIsKinematic = iskinematic;
}

// 衝突時の押し戻しの影響を受けるかどうか
bool CCollider::IsKinematic() const
{
	return mIsKinematic;
}

// コライダーの重量を設定
void CCollider::SetWeight(float weight)
{
	mWeight = weight;
}

// コライダーの重量を取得
float CCollider::GetWeight() const
{
	return mWeight;
}

// 指定したコライダーと衝突判定を行うかどうかを取得
bool CCollider::IsCollision(CCollider* col) const
{
	// 自分自身もしくは相手のコライダーが有効でなければ、
	// 衝突判定を行わない
	if (col == nullptr) return false;
	if (!col->IsEnable()) return false;
	if (!mIsEnable) return false;

	// 同じコライダーもしくは、持ち主が同じであれば、
	// 衝突判定を行わない
	if (col == this) return false;
	if (col->Owner() != nullptr || mpOwner != nullptr)
	{
		if (col->Owner() == mpOwner) return false;
	}

	// 衝突判定を行うレイヤーか
	if (!IsCollisionLayer(col->Layer())) return false;
	// 衝突判定を行うオブジェクトタグか
	if (!IsCollisionTag(col->Tag())) return false;

	// 全ての条件を満たした場合は衝突判定を行う
	return true;
}

// 衝突判定を行うレイヤーを設定
void CCollider::SetCollisionLayers(Layers layers)
{
	mCollisionLayers = 0;
	for (auto&& layer : layers)
	{
		mCollisionLayers |= 1 << (int)layer;
	}
}

// 指定したレイヤーとの衝突判定を行うかどうかを設定
void CCollider::SetCollisionLayer(ELayer layer, bool isCollision)
{
	if (isCollision) mCollisionLayers |= 1 << (int)layer;
	else mCollisionLayers &= ~(1 << (int)layer);
}

// 指定したレイヤーと衝突判定を行うかどうかを取得
bool CCollider::IsCollisionLayer(ELayer layer) const
{
	return (mCollisionLayers & 1 << (int)layer) != 0;
}

// 衝突判定を行うオブジェクトタグを設定
void CCollider::SetCollisionTags(Tags tags)
{
	mCollisionTags = 0;
	for (auto&& tag : tags)
	{
		mCollisionTags |= 1 << (int)tag;
	}
}

// 指定したオブジェクトタグとの衝突判定を行うかどうかを設定
void CCollider::SetCollisionTag(ETag tag, bool isCollision)
{
	if (isCollision) mCollisionTags |= 1 << (int)tag;
	else mCollisionTags &= ~(1 << (int)tag);
}

// 指定したオブジェクトタグと衝突判定を行うかどうかを取得
bool CCollider::IsCollisionTag(ETag tag) const
{
	return (mCollisionTags & 1 << (int)tag) != 0;
}

// 指定した行列にコライダーを附属させる
void CCollider::SetAttachMtx(const CMatrix* mtx)
{
	mpAttachMtx = mtx;
}

// 行列を取得
CMatrix CCollider::Matrix() const
{
	CMatrix m = CTransform::Matrix();
	// 附属させる行列が設定されていれば
	if (mpAttachMtx != nullptr)
	{
		// その行列に附属する
		CMatrix sm;
		sm.Scale(100.0f, 100.0f, 100.0f);
		m = sm * mAttachMtx * m;
	}
	// 持ち主が設定されていれば
	else if (mpOwner != nullptr)
	{
		// 持ち主の行列に附属
		m = mpOwner->Matrix() * m;
	}
	return m;
}

// バウンディングボックスを取得
CBounds CCollider::Bounds() const
{
	return mBounds;
}

// コライダー更新
void CCollider::Update()
{
	// くっつける行列が設定されていれば、
	if (mpAttachMtx != nullptr)
	{
		// くっつける行列をメンバ変数にコピー
		mAttachMtx = *mpAttachMtx;
	}
}

// 矩形同士の衝突判定
bool CCollider::CollisionRect(const CRect& rect1, const CRect& rect2)
{
	if (rect1.Left() <= rect2.Right() && rect1.Right() >= rect2.Left() &&
		rect1.Top() <= rect2.Bottom() && rect1.Bottom() >= rect2.Top())
	{
		return true;
	}
	return false;
}

// 矩形と点の衝突判定
bool CCollider::CollisionRectPoint(const CRect& rect, const CVector2 point)
{
	if (rect.Left() <= point.X() && rect.Right() >= point.X() &&
		rect.Top() <= point.Y() && rect.Bottom() >= point.Y())
	{
		return true;
	}
	return false;
}

// コライダーの設定
void CCollider::Set(CObjectBase* owner, ELayer layer)
{
	mpOwner = owner;
	mLayer = layer;
}

// 三角形と三角形の衝突判定
bool CCollider::CollisionTriangle(const CVector& t00, const CVector& t01, const CVector& t02,
	const CVector& t10, const CVector& t11, const CVector& t12,
	CHitInfo* hit)
{
	return false;
}

// 三角形と線分の衝突判定
bool CCollider::CollisionTriangleLine(
	const CVector& t0, const CVector& t1, const CVector& t2,
	const CVector& ls, const CVector& le,
	CHitInfo* h, bool isLeftMain)
{
	//面の法線を、外積を正規化して求める
	CVector normal = CVector::Cross(t1 - t0, t2 - t0).Normalized();
	//三角の頂点から線分始点へのベクトルを求める
	CVector v0sv = ls - t0;
	//三角の頂点から線分終点へのベクトルを求める
	CVector v0ev = le - t0;
	//線分が面と交差しているか内積で確認する
	float dots = v0sv.Dot(normal);
	float dote = v0ev.Dot(normal);
	//プラスは交差してない
	if (dots * dote >= 0.0f) {
		//衝突してない（調整不要）
		h->adjust = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//線分は面と交差している
	//面と線分の交点を求める
	//交点の計算
	CVector cross = ls + (le - ls) * (abs(dots) / (abs(dots) + abs(dote)));

	//交点が三角形内なら衝突している
	if (!IsInsideTriangle(cross, t0, t1, t2, normal))
	{
		//三角形外なので、衝突してない
		h->adjust = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//ヒット情報に交点を設定
	h->cross = cross;

	//調整値計算（衝突しない位置まで戻す）
	if (dots < 0.0f) {
		//始点が裏面
		h->adjust = normal * dots;
	}
	else {
		//終点が裏面
		h->adjust = normal * dote;
	}
	if (!isLeftMain) h->adjust = -h->adjust;
	return true;
}

// 三角形とカプセルの衝突判定
bool CCollider::CollisionTriangleCapsule(
	const CVector& t0, const CVector& t1, const CVector& t2,
	const CVector& cs, const CVector& ce, float cr,
	CHitInfo* h, bool isLeftMain)
{
	// 三角形の法線を算出
	CVector n = CVector::Cross(t1 - t0, t2 - t0).Normalized();

	// カプセルの始点から三角形の頂点までのベクトルを求める
	CVector vs = cs - t0;
	// カプセルの終点から三角形の頂点までのベクトルを求める
	CVector ve = ce - t0;

	// 各ベクトルと三角形の法線との内積を求める
	float d1 = CVector::Dot(vs, n);
	float d2 = CVector::Dot(ve, n);

	// パターン①
	// カプセルを構成する三角形が交差していたら、線分と三角形の衝突判定を行う
	// （各内積の結果を乗算してマイナスの場合は、
	// 　カプセルを構成する線分が三角形と交差している）
	if (d1 * d2 < 0.0f)
	{
		// 線分と三角形の衝突判定を行う
		if (CollisionTriangleLine(t0, t1, t2, cs, ce, h, isLeftMain))
		{
			// 衝突していた場合は、押し戻しベクトルに
			// カプセルの半径分の長さを追加して返す
			h->adjust += h->adjust.Normalized() * cr;
			return true;
		}
	}


	// 始点と終点を垂直に下ろした最近点を求める
	CVector c1 = cs - n * d1;
	CVector c2 = ce - n * d2;
	// 始点と終点の押し戻し量を求める
	CVector v1, v2;
	if (fabsf(d1) <= cr) v1 = (c1 + n * cr) - cs;
	if (fabsf(d2) <= cr) v2 = (c2 + n * cr) - ce;

	// パターン②
	// カプセルを構成する線分の始点と終点から三角形への距離を求め、
	// カプセルの半径より近い（めり込んでいる）場合は、押し戻す
	{
		float l1 = FLT_MAX, l2 = FLT_MAX;
		// 求めた最近点が三角形の内側かつ、距離が半径以下であれば、
		// 距離と押し戻しベクトルを求める
		if (IsInsideTriangle(c1, t0, t1, t2, n) && fabsf(d1) <= cr) l1 = v1.LengthSqr();
		if (IsInsideTriangle(c2, t0, t1, t2, n) && fabsf(d2) <= cr) l2 = v2.LengthSqr();

		if (l1 < FLT_MAX || l2 < FLT_MAX)
		{
			if (l1 < FLT_MAX && l2 < FLT_MAX) h->adjust = l1 >= l2 ? v1 : v2;
			else h->adjust = l1 < FLT_MAX ? v1 : v2;
			if (isLeftMain) h->adjust = -h->adjust;
			return true;
		}
	}

	// パターン③
	// カプセルを構成する線分と三角形の各辺の距離を求め、
	// カプセルの半径より近い場合は押し戻す
	{
		float e1 = CalcDistanceLine(t0, t1, cs, ce);
		float e2 = CalcDistanceLine(t1, t2, cs, ce);
		float e3 = CalcDistanceLine(t2, t0, cs, ce);
		if (std::min(std::min(e1, e2), e3) <= cr)
		{
			if (e1 <= e2 && e1 <= e3)
			{
				CVector cv = CVector::Cross(t1 - t0, ce - cs).Normalized();
				if (cv.LengthSqr() == 0.0f)
				{
					CVector nv = (t1 - t0).Normalized();
					cv = (t0 + nv * CVector::Dot(cs - t0, nv) - cs).Normalized();
				}
				h->adjust = cv * (cr - e1);
			}
			else if (e2 <= e1 && e2 <= e3)
			{
				CVector cv = CVector::Cross(t2 - t1, ce - cs).Normalized();
				if (cv.LengthSqr() == 0.0f)
				{
					CVector nv = (t2 - t1).Normalized();
					cv = (t1 + nv * CVector::Dot(cs - t1, nv) - cs).Normalized();
				}
				h->adjust = cv * (cr - e2);
			}
			else
			{
				CVector cv = CVector::Cross(t0 - t2, ce - cs).Normalized();
				if (cv.LengthSqr() == 0.0f)
				{
					CVector nv = (t0 - t2).Normalized();
					cv = (t2 + nv * CVector::Dot(cs - t2, nv) - cs).Normalized();
				}
				h->adjust = cv * (cr - e3);
			}
			if (!isLeftMain) h->adjust = -h->adjust;
			return true;
		}
	}

	h->adjust = CVector::zero;
	return false;
}

// 三角形と点の衝突判定
bool CCollider::CollisionTrianglePoint(const CVector& t0, const CVector& t1, const CVector& t2, const CVector& tn, const CVector& p)
{
	if (CVector::Dot(CVector::Cross(t1 - t0, p - t0), tn) < 0.0f) return false;
	if (CVector::Dot(CVector::Cross(t2 - t1, p - t1), tn) < 0.0f) return false;
	if (CVector::Dot(CVector::Cross(t0 - t2, p - t2), tn) < 0.0f) return false;
	return true;
}

// 三角形と点の衝突判定
bool CCollider::CollisionTrianglePoint(const CVector& t0, const CVector& t1, const CVector& t2, const CVector& p)
{
	CVector n = CVector::Cross(t1 - t0, t2 - t0).Normalized();
	return CollisionTrianglePoint(t0, t1, t2, n, p);
}

// 三角形と球の衝突判定
bool CCollider::CollisionTriangleSphere(
	const CVector& t0, const CVector& t1, const CVector& t2,
	const CVector& sp, const float sr,
	CHitInfo* h, bool isLeftMain)
{
	// 三角形の法線を求める
	CVector n = CVector::Cross(t1 - t0, t2 - t0).Normalized();
	// 法線の長さが0であれば、無効な三角形なため、衝突していない
	if (n.LengthSqr() <= 0.0f) return false;

	// 三角形から球までの距離を求めて、
	// 距離が球の半径より離れていたら、衝突していない
	CVector v = sp - t0;
	float dist = CVector::Dot(v, n);
	if (fabsf(dist) > sr) return false;

	// 球の中心点から三角形へ垂直に下ろした点を求める
	CVector point = sp + (-n * dist);
	// 求めた点が三角形の範囲内か調べる
	if (CollisionTrianglePoint(t0, t1, t2, n, point))
	{
		// 範囲内であれば、衝突しているので、
		// 押し戻し量を計算
		float l = (sr - fabsf(dist)) * (dist < 0.0f ? -1.0f : 1.0f);
		h->adjust = n * l * (isLeftMain ? -1.0f : 1.0f);
		return true;
	}

	// 垂直に下ろした点が三角形の範囲外の場合、
	// 各辺と球が衝突していないか確認

	// 三角形の各辺との最短距離を求め、
	// 半径より小さい場合は衝突しているため、
	// その辺との押し戻し量を計算して返す

	// 三角形の頂点0から頂点1までの辺
	dist = CalcDistancePointToLine(sp, t0, t1);
	if (dist <= sr)
	{
		float l = sr - dist;
		h->adjust = n * l * (isLeftMain ? -1.0f : 1.0f);
		return true;
	}
	// 三角形の頂点1から頂点2までの辺
	dist = CalcDistancePointToLine(sp, t1, t2);
	if (dist <= sr)
	{
		float l = sr - dist;
		h->adjust = n * l * (isLeftMain ? -1.0f : 1.0f);
		return true;
	}
	// 三角形の頂点2から頂点0までの辺
	dist = CalcDistancePointToLine(sp, t2, t0);
	if (dist <= sr)
	{
		float l = sr - dist;
		h->adjust = n * l * (isLeftMain ? -1.0f : 1.0f);
		return true;
	}

	// 三角形の範囲外かつ、各辺とも衝突していない場合は、
	// 完全に衝突していない
	return false;
}

// 球と球の衝突判定
bool CCollider::CollisionSphere(const CVector& sp0, const float sr0,
	const CVector& sp1, const float sr1, CHitInfo* hit)
{
	//中心から中心へのベクトルを求める
	CVector vec = sp0 - sp1;
	float length = vec.Length();
	//中心の距離が半径の合計より小さいと衝突
	float sum = sr0 + sr1;
	if (sum > length) {
		hit->adjust = vec.Normalized() * (sum - length);
		//衝突している
		return  true;
	}

	hit->adjust = CVector(0.0f, 0.0f, 0.0f);
	//衝突していない
	return false;
}

// 球と線分の衝突判定
bool CCollider::CollisionSphereLine(const CVector& sp, const float sr,
	const CVector& ls, const CVector& le,
	CHitInfo* hit, bool isLeftMain)
{
	CVector nearest;
	float length = CalcDistancePointToLine(sp, ls, le, &nearest);
	if (length < sr)
	{
		CVector n = (sp - nearest).Normalized() * (isLeftMain ? 1.0f : -1.0f);
		hit->adjust = n * (sr - length);

		CVector v = (le - ls).Normalized();
		CVector p = (sp - ls).Normalized();
		float a = CVector::Dot(v, v);
		float b = CVector::Dot(v, p);
		float c = CVector::Dot(p, p) - sr * sr;
		if (a != 0.0f) hit->cross = ls + v * ((b - sqrtf(b * b - a * c)) / a);
		else hit->cross = CVector::zero;
		return true;
	}

	hit->adjust = CVector(0.0f, 0.0f, 0.0f);
	return false;
}

// 球とカプセルの衝突判定
bool CCollider::CollisionSphereCapsule(
	const CVector& sp, const float sr,
	const CVector& cs, const CVector& ce, float cr,
	CHitInfo* hit, bool isLeftMain)
{
	CVector nearest;
	float length = CalcDistancePointToLine(sp, cs, ce, &nearest);
	if (length < sr + cr)
	{
		CVector n = (sp - nearest).Normalized() * (isLeftMain ? 1.0f : -1.0f);
		hit->adjust = n * ((sr + cr) - length);

		CVector v = (ce - cs).Normalized();
		CVector p = (sp - cs).Normalized();
		float a = CVector::Dot(v, v);
		float b = CVector::Dot(v, p);
		float c = CVector::Dot(p, p) - sr * sr;
		if (a != 0.0f) hit->cross = cs + v * ((b - sqrtf(b * b - a * c)) / a);
		else hit->cross = CVector::zero;
		return true;
	}

	hit->adjust = CVector(0.0f, 0.0f, 0.0f);
	return false;
}

// 線分と線分の衝突判定
bool CCollider::CollisionLine(const CVector& ls0, const CVector& le0,
	const CVector& ls1, const CVector& le1, CHitInfo* hit)
{
	//TODO:調整値の対応
	hit->adjust = CVector(0.0f, 0.0f, 0.0f);

	CVector V0 = le0 - ls0;
	CVector V1 = le1 - ls1;

	CVector S1E1 = le0 - ls0;
	CVector S2E2 = le1 - ls1;
	CVector CD = CVector::Cross(V0, V1).Normalized();

	CVector S1S2 = ls1 - ls0;
	CVector S1E2 = le1 - ls0;
	CVector S2S1 = ls0 - ls1;
	CVector S2E1 = le0 - ls1;

	float length = 0.0f;
	float d1 = S1E1.Cross(S1S2).Dot(S1E1.Cross(S1E2));
	float d2 = S2E2.Cross(S2S1).Dot(S2E2.Cross(S2E1));
	if (d1 < 0 && d2 < 0)
	{
		length = abs(S1S2.Dot(CD));
	}
	else
	{
		float length1 = CalcDistancePointToLine(ls0, ls1, le1);
		float length2 = CalcDistancePointToLine(le0, ls1, le1);
		float length3 = CalcDistancePointToLine(ls1, ls0, le0);
		float length4 = CalcDistancePointToLine(le1, ls0, le0);
		length = fminf(fminf(length1, length2), fminf(length3, length4));
	}

	if (length < 0.1f + 0.1f)
	{
		return true;
	}

	return false;
}

// カプセルと線分の衝突判定
bool CCollider::CollisionCapsuleLine(
	const CVector& cs, const CVector& ce, float cr,
	const CVector& ls, const CVector& le,
	CHitInfo* hit, bool isLeftMain)
{
	//TODO:調整値の対応
	hit->adjust = CVector(0.0f, 0.0f, 0.0f);

	CVector V0 = le - ls;
	CVector V1 = ce - cs;

	CVector S1E1 = le - ls;
	CVector S2E2 = ce - cs;
	CVector CD = CVector::Cross(V0, V1).Normalized();

	CVector S1S2 = cs - ls;
	CVector S1E2 = ce - ls;
	CVector S2S1 = ls - cs;
	CVector S2E1 = le - cs;

	float length = 0.0f;
	float d1 = S1E1.Cross(S1S2).Dot(S1E1.Cross(S1E2));
	float d2 = S2E2.Cross(S2S1).Dot(S2E2.Cross(S2E1));
	if (d1 < 0 && d2 < 0)
	{
		length = abs(S1S2.Dot(CD));
	}
	else
	{
		float length1 = CalcDistancePointToLine(ls, cs, ce);
		float length2 = CalcDistancePointToLine(le, cs, ce);
		float length3 = CalcDistancePointToLine(cs, ls, le);
		float length4 = CalcDistancePointToLine(ce, ls, le);
		length = fminf(fminf(length1, length2), fminf(length3, length4));
	}

	if (length < cr)
	{
		CVector n = CVector::zero;
		if (CD.LengthSqr() == 0.0f)
		{
			n = CVector::Cross(V0, CVector::forward);
			if (n.LengthSqr() == 0.0f)
			{
				n = CVector::Cross(V0, CVector::up);
			}
		}
		else
		{
			n = CD;
		}

		n = (n.Normalized() * S1S2.Dot(n)).Normalized();
		hit->adjust = n * (cr - length);
		return true;
	}

	return false;
}

// カプセルとカプセルの衝突判定
bool CCollider::CollisionCapsule(const CVector& cs0, const CVector& ce0, float cr0, const CVector& cs1, const CVector& ce1, float cr1, CHitInfo* hit)
{
	hit->adjust = CVector(0.0f, 0.0f, 0.0f);

	CVector V0 = ce0 - cs0;
	CVector V1 = ce1 - cs1;
	CVector VN0 = V0.Normalized();
	CVector VN1 = V1.Normalized();

	CVector S1E1 = ce0 - cs0;
	CVector S2E2 = ce1 - cs1;
	CVector CD = CVector::Cross(V0, V1).Normalized();

	CVector S1S2 = cs1 - cs0;
	CVector S1E2 = ce1 - cs0;
	CVector S2S1 = cs0 - cs1;
	CVector S2E1 = ce0 - cs1;

	CVector cross = CVector::zero;

	float length = 0.0f;
	float d1 = S1E1.Cross(S1S2).Dot(S1E1.Cross(S1E2));
	float d2 = S2E2.Cross(S2S1).Dot(S2E2.Cross(S2E1));
	if (d1 < 0 && d2 < 0)
	{
		length = abs(S1S2.Dot(CD));
		float d = CVector::Dot(VN0, VN1);
		float dn0 = CVector::Dot(S1S2, VN0);
		float dn1 = CVector::Dot(S1S2, VN1);
		float r = 1.0f - d * d;
		if (r != 0.0f)
		{
			float t1 = (dn0 - d * dn1) / r;
			float t2 = (d * dn0 - dn1) / r;
			CVector p1 = cs0 + VN0 * t1;
			CVector p2 = cs1 + VN1 * t2;
			cross = (p1 + p2) * 0.5f;
		}
	}
	else
	{
		CVector n1, n2, n3, n4;
		float length1 = CalcDistancePointToLine(cs0, cs1, ce1, &n1);
		float length2 = CalcDistancePointToLine(ce0, cs1, ce1, &n2);
		float length3 = CalcDistancePointToLine(cs1, cs0, ce0, &n3);
		float length4 = CalcDistancePointToLine(ce1, cs0, ce0, &n4);
		length = fminf(fminf(length1, length2), fminf(length3, length4));

		if (length == length1) cross = (cs0 + n1) * 0.5f;
		else if (length == length2) cross = (ce0 + n2) * 0.5f;
		else if (length == length3) cross = (cs1 + n3) * 0.5f;
		else cross = (ce1 + n4) * 0.5f;
	}

	if (length < cr0 + cr1)
	{
		CVector n = CVector::zero;
		if (CD.LengthSqr() == 0.0f)
		{
			n = CVector::Cross(V0, CVector::forward);
			if (n.LengthSqr() == 0.0f)
			{
				n = CVector::Cross(V0, CVector::up);
			}
		}
		else
		{
			n = CD;
		}

		n = -(n.Normalized() * S1S2.Dot(n)).Normalized();
		hit->adjust = n * ((cr0 + cr1) - length);
		hit->cross = cross;
		return true;
	}

	return false;
}

// メッシュと線分の衝突判定
bool CCollider::CollisionMeshLine(const std::list<STVertexData>& tris,
	const CVector& ls, const CVector& le, const CBounds& lb,
	CHitInfo* hit, bool isLeftMain)
{
	bool ret = false;
	CVector adjust = CVector::zero;
	CVector cross = CVector::zero;
	float nearDist = 0.0f;
	bool isFirst = true;
	for (auto& v : tris)
	{
		if (!CBounds::Intersect(v.bounds, lb)) continue;
		if (CollisionTriangleLine(v.wv.V[0], v.wv.V[1], v.wv.V[2], ls, le, hit, isLeftMain))
		{
			hit->tris.push_back(v.wv);

			CVector adj = hit->adjust;
			adjust.X(abs(adjust.X()) > abs(adj.X()) ? adjust.X() : adj.X());
			adjust.Y(abs(adjust.Y()) > abs(adj.Y()) ? adjust.Y() : adj.Y());
			adjust.Z(abs(adjust.Z()) > abs(adj.Z()) ? adjust.Z() : adj.Z());

			if (isFirst)
			{
				cross = hit->cross;
				nearDist = (cross - ls).Length();
				isFirst = false;
			}
			else
			{
				float dist = (hit->cross - ls).Length();
				if (dist < nearDist)
				{
					cross = hit->cross;
					nearDist = dist;
				}
			}

			ret = true;
		}
	}
	hit->adjust = adjust;
	hit->cross = cross;
	hit->dist = nearDist;
	return ret;
}

// メッシュと球の衝突判定
bool CCollider::CollisionMeshSpehre(const std::list<STVertexData>& tris,
	CColliderSphere* sphereCol, CHitInfo* hit, bool isLeftMain)
{
	CVector sp;
	float sr;
	sphereCol->Get(&sp, &sr);

	bool ret = false;
	CVector adjust = CVector::zero;
	for (auto& v : tris)
	{
		if (!CBounds::Intersect(v.bounds, sphereCol->Bounds())) continue;
		if (CollisionTriangleSphere(v.wv.V[0], v.wv.V[1], v.wv.V[2], sp, sr, hit, isLeftMain))
		{
			adjust.Y(fabsf(adjust.Y()) > fabsf(hit->adjust.Y()) ? adjust.Y() : hit->adjust.Y());
			adjust.X(fabsf(adjust.X()) > fabsf(hit->adjust.X()) ? adjust.X() : hit->adjust.X());
			adjust.Z(fabsf(adjust.Z()) > fabsf(hit->adjust.Z()) ? adjust.Z() : hit->adjust.Z());
			hit->tris.push_back(v.wv);
			ret = true;
		}
	}
	if (ret)
	{
		hit->adjust = adjust;
	}
	return ret;
}

// メッシュと三角形の衝突判定
bool CCollider::CollisionMeshTriangle(const std::list<STVertexData>& tris,
	CColliderTriangle* triCol, CHitInfo* hit, bool isLeftMain)
{
	CVector t0, t1, t2;
	triCol->Get(&t0, &t1, &t2);

	bool ret = false;
	for (auto& v : tris)
	{
		if (!CBounds::Intersect(v.bounds, triCol->Bounds())) continue;
		CVector v00 = isLeftMain ? v.wv.V[0] : t0;
		CVector v01 = isLeftMain ? v.wv.V[1] : t1;
		CVector v02 = isLeftMain ? v.wv.V[2] : t2;
		CVector v10 = isLeftMain ? t0 : v.wv.V[0];
		CVector v11 = isLeftMain ? t1 : v.wv.V[1];
		CVector v12 = isLeftMain ? t2 : v.wv.V[2];
		if (CollisionTriangle(v00, v01, v02, v10, v11, v12, hit))
		{
			hit->tris.push_back(v.wv);
			ret = true;
		}
	}
	return ret;
}

// メッシュとカプセルの衝突判定
bool CCollider::CollisionMeshCapsule(const std::list<STVertexData>& tris,
	CColliderCapsule* capsuleCol, CHitInfo* hit, bool isLeftMain)
{
	CVector cs, ce;
	capsuleCol->Get(&cs, &ce);
	float cr = capsuleCol->Radius();

	bool ret = false;
	CVector adjust = CVector::zero;
	CVector cross = CVector::zero;
	float nearDist = 0.0f;
	bool isFirst = true;
	for (auto& v : tris)
	{
		if (!CBounds::Intersect(v.bounds, capsuleCol->Bounds())) continue;
		if (CollisionTriangleCapsule(v.wv.V[0], v.wv.V[1], v.wv.V[2], cs, ce, cr, hit, isLeftMain))
		{
			hit->tris.push_back(v.wv);

			CVector adj = hit->adjust;
			adjust.X(abs(adjust.X()) > abs(adj.X()) ? adjust.X() : adj.X());
			adjust.Y(abs(adjust.Y()) > abs(adj.Y()) ? adjust.Y() : adj.Y());
			adjust.Z(abs(adjust.Z()) > abs(adj.Z()) ? adjust.Z() : adj.Z());

			if (isFirst)
			{
				cross = hit->cross;
				nearDist = (cross - cs).Length();
				isFirst = false;
			}
			else
			{
				float dist = (hit->cross - cs).Length();
				if (dist < nearDist)
				{
					cross = hit->cross;
					nearDist = dist;
				}
			}

			ret = true;
		}
	}
	hit->adjust = adjust;
	hit->cross = cross;
	hit->dist = nearDist;
	return ret;
}

float CCollider::CalcDistancePointToLine(const CVector& point, const CVector& lineS, const CVector& lineE, CVector* nearest)
{
	CVector SE = lineE - lineS;
	float t = (point - lineS).Dot(SE.Normalized());

	CVector C;
	if (t < 0) C = lineS;
	else if (t > SE.Length()) C = lineE;
	else
	{
		C = lineS + SE.Normalized() * t;
	}

	if (nearest != nullptr) *nearest = C;

	return (C - point).Length();
}

float CCollider::CalcDistanceLine(const CVector& s1, const CVector& e1, const CVector& s2, const CVector& e2)
{
	// 線分同士が交差するかどうかで最短距離の判定を変える
	CVector SE1 = e1 - s1;
	CVector SE2 = e2 - s2;
	float d1 = CVector::Dot(CVector::Cross(SE1, s2 - s1), CVector::Cross(SE1, e2 - s1));
	float d2 = CVector::Dot(CVector::Cross(SE2, s1 - s2), CVector::Cross(SE2, e1 - s2));
	// 線分同士が交差している場合
	if (d1 < 0.0f && d2 < 0.0f)
	{
		CVector cv = CVector::Cross(SE1, SE2).Normalized();
		return fabsf(CVector::Dot(cv, s2 - s1));
	}
	// 線分同士が交差していない場合
	else
	{
		float l1 = CalcDistancePointToLine(s1, s2, e2);
		float l2 = CalcDistancePointToLine(e1, s2, e2);
		float l3 = CalcDistancePointToLine(s2, s1, e1);
		float l4 = CalcDistancePointToLine(e2, s1, e1);
		return std::min(std::min(l1, l2), std::min(l3, l4));
	}
}

bool CCollider::IsInsideTriangle(const CVector& p, const CVector& t0, const CVector& t1, const CVector& t2, const CVector& n)
{
	// 三角形の各辺と三角形の各頂点から点までのベクトルに
	// 垂直なベクトルを外積（CVector::Cross）で求める。

	// 三角形の法線と求めたベクトルの内積（CVector::Dot）で
	// 両ベクトルの向きを調べて同じ方向を向いているかを調べる。

	// これを三角形の三辺分行い、全て条件を満たした場合は、
	// 三角形の内側と判断する。
	if (CVector::Dot(CVector::Cross(t1 - t0, p - t0), n) < 0.0f) return false;
	if (CVector::Dot(CVector::Cross(t2 - t1, p - t1), n) < 0.0f) return false;
	if (CVector::Dot(CVector::Cross(t0 - t2, p - t2), n) < 0.0f) return false;
	return true;
}

bool CCollider::Collision(CCollider* c0, CCollider* c1, CHitInfo* hit)
{
	switch (c0->Type())
	{
	case EColliderType::eLine:
	{
		CColliderLine* line0 = dynamic_cast<CColliderLine*>(c0);
		CVector ls0, le0;
		line0->Get(&ls0, &le0);

		switch (c1->Type())
		{
		case EColliderType::eLine:
		{
			CColliderLine* line1 = dynamic_cast<CColliderLine*>(c1);
			CVector ls1, le1;
			line1->Get(&ls1, &le1);
			return CollisionLine(ls0, le0, ls1, le1, hit);
		}
		case EColliderType::eSphere:
		{
			CColliderSphere* sphere = dynamic_cast<CColliderSphere*>(c1);
			CVector sp;
			float sr;
			sphere->Get(&sp, &sr);
			return CollisionSphereLine(sp, sr, ls0, le0, hit, false);
		}
		case EColliderType::eTriangle:
		{
			CColliderTriangle* triangle = dynamic_cast<CColliderTriangle*>(c1);
			CVector t0, t1, t2;
			triangle->Get(&t0, &t1, &t2);
			return CollisionTriangleLine(t0, t1, t2, ls0, le0, hit, false);
		}
		case EColliderType::eCapsule:
		{
			CColliderCapsule* capsule = dynamic_cast<CColliderCapsule*>(c1);
			CVector cs, ce;
			capsule->Get(&cs, &ce);
			float cr = capsule->Radius();
			return CollisionCapsuleLine(cs, ce, cr, ls0, le0, hit, false);
		}
		case EColliderType::eMesh:
		{
			CColliderMesh* mesh = dynamic_cast<CColliderMesh*>(c1);
			auto tris = mesh->Get();
			return CollisionMeshLine(tris, ls0, le0, line0->Bounds(), hit, false);
		}
		}
		break;
	}
	case EColliderType::eSphere:
	{
		CColliderSphere* sphere0 = dynamic_cast<CColliderSphere*>(c0);
		CVector sp0;
		float sr0;
		sphere0->Get(&sp0, &sr0);

		switch (c1->Type())
		{
		case EColliderType::eLine:
		{
			CColliderLine* line = dynamic_cast<CColliderLine*>(c1);
			CVector ls, le;
			line->Get(&ls, &le);
			return CollisionSphereLine(sp0, sr0, ls, le, hit, true);
		}
		case EColliderType::eSphere:
		{
			CColliderSphere* sphere1 = dynamic_cast<CColliderSphere*>(c1);
			CVector sp1;
			float sr1;
			sphere1->Get(&sp1, &sr1);
			return CollisionSphere(sp0, sr0, sp1, sr1, hit);
		}
		case EColliderType::eTriangle:
		{
			CColliderTriangle* triangle = dynamic_cast<CColliderTriangle*>(c1);
			CVector t0, t1, t2;
			triangle->Get(&t0, &t1, &t2);
			return CollisionTriangleSphere(t0, t1, t2, sp0, sr0, hit, false);
		}
		case EColliderType::eCapsule:
		{
			CColliderCapsule* capsule = dynamic_cast<CColliderCapsule*>(c1);
			CVector cs, ce;
			capsule->Get(&cs, &ce);
			float cr = capsule->Radius();
			return CollisionSphereCapsule(sp0, sr0, cs, ce, cr, hit, true);
		}
		break;
		case EColliderType::eMesh:
		{
			CColliderMesh* mesh = dynamic_cast<CColliderMesh*>(c1);
			auto tris = mesh->Get();
			return CollisionMeshSpehre(tris, sphere0, hit, false);
		}
		}
		break;
	}
	case EColliderType::eTriangle:
	{
		CColliderTriangle* triangle0 = dynamic_cast<CColliderTriangle*>(c0);
		CVector t00, t01, t02;
		triangle0->Get(&t00, &t01, &t02);
		switch (c1->Type())
		{
		case EColliderType::eLine:
		{
			CColliderLine* line = dynamic_cast<CColliderLine*>(c1);
			CVector ls, le;
			line->Get(&ls, &le);
			return CollisionTriangleLine(t00, t01, t02, ls, le, hit, true);
		}
		case EColliderType::eSphere:
		{
			CColliderSphere* sphere = dynamic_cast<CColliderSphere*>(c1);
			CVector sp;
			float sr;
			sphere->Get(&sp, &sr);
			return CollisionTriangleSphere(t00, t01, t02, sp, sr, hit, true);
		}
		case EColliderType::eTriangle:
		{
			CColliderTriangle* triangle1 = dynamic_cast<CColliderTriangle*>(c1);
			CVector t10, t11, t12;
			triangle1->Get(&t10, &t11, &t12);
			return CollisionTriangle(t00, t01, t02, t10, t11, t12, hit);
		}
		case EColliderType::eCapsule:
		{
			CColliderCapsule* capsule = dynamic_cast<CColliderCapsule*>(c1);
			CVector cs, ce;
			capsule->Get(&cs, &ce);
			float cr = capsule->Radius();
			return CollisionTriangleCapsule(t00, t01, t02, cs, ce, cr, hit, true);
		}
		case EColliderType::eMesh:
		{
			CColliderMesh* mesh = dynamic_cast<CColliderMesh*>(c1);
			auto tris = mesh->Get();
			return CollisionMeshTriangle(tris, triangle0, hit, false);
		}
		}
		break;
	}
	case EColliderType::eCapsule:
	{
		CColliderCapsule* capsule0 = dynamic_cast<CColliderCapsule*>(c0);
		CVector cs0, ce0;
		capsule0->Get(&cs0, &ce0);
		float cr0 = capsule0->Radius();

		switch (c1->Type())
		{
		case EColliderType::eLine:
		{
			CColliderLine* line1 = dynamic_cast<CColliderLine*>(c1);
			CVector ls1, le1;
			line1->Get(&ls1, &le1);
			return CollisionCapsuleLine(cs0, ce0, cr0, ls1, le1, hit, false);
		}
		case EColliderType::eSphere:
		{
			CColliderSphere* sphere = dynamic_cast<CColliderSphere*>(c1);
			CVector sp;
			float sr;
			sphere->Get(&sp, &sr);
			return CollisionSphereCapsule(sp, sr, cs0, ce0, cr0, hit, false);
		}
		case EColliderType::eTriangle:
		{
			CColliderTriangle* triangle = dynamic_cast<CColliderTriangle*>(c1);
			CVector t0, t1, t2;
			triangle->Get(&t0, &t1, &t2);
			return CollisionTriangleCapsule(t0, t1, t2, cs0, ce0, cr0, hit, false);
		}
		case EColliderType::eCapsule:
		{
			CColliderCapsule* capsule1 = dynamic_cast<CColliderCapsule*>(c1);
			CVector cs1, ce1;
			capsule1->Get(&cs1, &ce1);
			float cr1 = capsule1->Radius();
			return CollisionCapsule(cs0, ce0, cr0, cs1, ce1, cr1, hit);
		}
		case EColliderType::eMesh:
		{
			CColliderMesh* mesh = dynamic_cast<CColliderMesh*>(c1);
			auto tris = mesh->Get();
			return CollisionMeshCapsule(tris, capsule0, hit, false);
		}
		}
		break;
	}
	case EColliderType::eMesh:
	{
		CColliderMesh* mesh = dynamic_cast<CColliderMesh*>(c0);
		auto tris = mesh->Get();
		switch (c1->Type())
		{
		case EColliderType::eLine:
		{
			CColliderLine* line = dynamic_cast<CColliderLine*>(c1);
			CVector ls, le;
			line->Get(&ls, &le);
			return CollisionMeshLine(tris, ls, le, line->Bounds(), hit, true);
		}
		case EColliderType::eSphere:
		{
			CColliderSphere* sphere = dynamic_cast<CColliderSphere*>(c1);
			return CollisionMeshSpehre(tris, sphere, hit, true);
		}
		case EColliderType::eTriangle:
		{
			CColliderTriangle* triangle = dynamic_cast<CColliderTriangle*>(c1);
			return CollisionMeshTriangle(tris, triangle, hit, true);
		}
		case EColliderType::eCapsule:
		{
			CColliderCapsule* capsule = dynamic_cast<CColliderCapsule*>(c1);
			return CollisionMeshCapsule(tris, capsule, hit, true);
		}
		case EColliderType::eMesh:
		{
			// メッシュとメッシュは衝突判定を行わない
			return false;
		}
		}
		break;
	}
	}
	return false;
}

// レイとコライダーの衝突判定
bool CCollider::CollisionRay(CCollider* c, const CVector& start, const CVector& end, CHitInfo* hit)
{
	// コライダーがnullならば、衝突していない
	if (c == nullptr) return false;
	// レイの長さが0ならば、衝突していない
	CVector v = end - start;
	if (v.LengthSqr() == 0.0f) return false;

	// コライダーの種類によって衝突判定を切り替える
	switch(c->Type())
	{
		// 線コライダーとの衝突
		case EColliderType::eLine:
		{
			CColliderLine* line = dynamic_cast<CColliderLine*>(c);
			CVector ls, le;
			line->Get(&ls, &le);
			return CollisionLine(start, end, ls, le, hit);
		}
		// 球コライダーとの衝突
		case EColliderType::eSphere:
		{
			CColliderSphere* sphere = dynamic_cast<CColliderSphere*>(c);
			CVector sp;
			float sr;
			sphere->Get(&sp, &sr);
			return CollisionSphereLine(sp, sr, start, end, hit, false);
		}
		// 三角形コライダーとの衝突
		case EColliderType::eTriangle:
		{
			CColliderTriangle* triangle = dynamic_cast<CColliderTriangle*>(c);
			CVector t0, t1, t2;
			triangle->Get(&t0, &t1, &t2);
			return CollisionTriangleLine(t0, t1, t2, start, end, hit, false);
		}
		// メッシュコライダーとの衝突
		case EColliderType::eMesh:
		{
			CColliderMesh* mesh = dynamic_cast<CColliderMesh*>(c);
			auto tris = mesh->Get();
			CBounds bounds = CBounds::GetLineBounds(start, end);
			return CollisionMeshLine(tris, start, end, bounds, hit, false);
		}
	}

	// それ以外は失敗
	return false;
}

// 衝突時の押し戻し割合を算出
float CCollider::CalcPushBackRatio(CCollider* self, CCollider* other)
{
	// 自身のコライダーが押し戻しの影響を受けない
	if (self->IsKinematic()) return 0.0f;
	// 相手のコライダーが押し戻しの影響を受けない
	if (other->IsKinematic()) return 1.0f;

	// 両方のコライダーが押し戻しの影響を受ける場合は、
	// 両方のコライダーの重量で押し戻し割合を算出
	float sw = self->GetWeight();
	float ow = other->GetWeight();
	// 同じ重量ならば、50%の影響を受ける
	if (sw == ow) return 0.5f;
	// 自身の重量が0ならば、100%の影響を受ける
	if (sw <= 0.0f) return 1.0f;
	// 相手の重量が0ならば、影響は受けない
	if (ow <= 0.0f) return 0.0f;

	// 重量の割合を計算して返す
	return 1.0f - Math::Clamp01(sw / (sw + ow));
}
