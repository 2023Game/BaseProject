#pragma once
#include "CTransform.h"
#include "CollisionLayer.h"
#include "ColliderType.h"
#include "CVertex.h"
#include "ObjectTag.h"
#include "CBounds.h"
#include "CRect.h"

class CObjectBase;
class CCollisionManager;
class CColliderLine;
class CColliderSphere;
class CColliderTriangle;
class CColliderCapsule;
class CColliderMesh;

struct STVertexData
{
	STVertex lv;
	STVertex wv;
	CBounds bounds;
};

// ヒット情報
class CHitInfo
{
public:
	CVector adjust;	// 押し戻しベクトル
	float weight;	// 押し戻し割合
	CVector cross;	// 衝突位置
	float dist;		// 衝突位置までの距離
	std::list<STVertex> tris;	// 衝突している三角形リスト
};

/// <summary>
/// コライダーのベースクラス
/// </summary>
class CCollider : public CTransform
{
	friend CCollisionManager;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">コライダーの持ち主</param>
	/// <param name="layer">衝突判定用のレイヤー</param>
	/// <param name="type">コライダーの種類</param>
	CCollider(CObjectBase* owner, ELayer layer, EColliderType type, bool isKinematic, float weight);
	// デストラクタ
	virtual ~CCollider();

	// 衝突判定レイヤーを取得
	ELayer Layer() const;
	// コライダーの種類を取得
	EColliderType Type() const;
	// コライダーの持ち主を取得
	CObjectBase* Owner() const;
	// コライダーの持ち主のタグを取得
	ETag Tag() const;

	/// <summary>
	/// コライダーが有効かどうかを設定
	/// </summary>
	/// <param name="isEnable">有効かどうか</param>
	void SetEnable(bool isEnable);
	/// <summary>
	/// コライダーが有効かどうかを取得
	/// </summary>
	/// <returns>trueならば有効</returns>
	bool IsEnable() const;

	/// <summary>
	/// 衝突時の押し戻しの影響を受けるかどうかを設定
	/// </summary>
	/// <param name="isKinematic">trueならば、影響を受けない</param>
	void SetKinematic(bool isKinematic);
	/// <summary>
	/// 衝突時の押し戻しの影響を受けるかどうか
	/// </summary>
	/// <returns>trueならば、影響を受けない</returns>
	bool IsKinematic() const;

	/// <summary>
	/// コライダーの重量を設定
	/// </summary>
	/// <param name="weight"></param>
	void SetWeight(float weight);
	/// <summary>
	/// コライダーの重量を取得
	/// </summary>
	/// <returns></returns>
	float GetWeight() const;

	/// <summary>
	/// 指定したコライダーと衝突判定を行うかどうかを取得
	/// </summary>
	/// <param name="col">衝突判定を行うか確認するコライダー</param>
	/// <returns>trueならば衝突判定を行う</returns>
	bool IsCollision(CCollider* col) const;

	/// <summary>
	/// 衝突判定を行うレイヤーを設定
	/// （設定前の状態はリセットする）
	/// </summary>
	/// <param name="layers">衝突判定を行うレイヤーのリスト</param>
	void SetCollisionLayers(Layers layers);
	/// <summary>
	/// 指定したレイヤーとの衝突判定を行うかどうかを設定
	/// </summary>
	/// <param name="layer">設定するレイヤー</param>
	/// <param name="isCollision">衝突判定を行うかどうか</param>
	void SetCollisionLayer(ELayer layer, bool isCollision);
	/// <summary>
	/// 指定したレイヤーと衝突判定を行うかどうかを取得
	/// </summary>
	/// <param name="layer">判定するレイヤー</param>
	/// <returns>trueならば衝突判定を行う</returns>
	bool IsCollisionLayer(ELayer layer) const;

	/// <summary>
	/// 衝突判定を行うオブジェクトタグを設定
	/// （設定前の状態はリセットする）
	/// </summary>
	/// <param name="tags">衝突判定を行うオブジェクトタグのリスト</param>
	void SetCollisionTags(Tags tags);
	/// <summary>
	/// 指定したオブジェクトタグとの衝突判定を行うかどうかを設定
	/// </summary>
	/// <param name="tag">設定するオブジェクトタグ</param>
	/// <param name="isCollision">衝突判定を行うかどうか</param>
	void SetCollisionTag(ETag tag, bool isCollision);
	/// <summary>
	/// 指定したオブジェクトタグと衝突判定を行うかどうかを取得
	/// </summary>
	/// <param name="tag">判定するオブジェクトタグ</param>
	/// <returns>trueならば衝突判定を行う</returns>
	bool IsCollisionTag(ETag tag) const;

	// 指定した行列にコライダーを附属させる
	void SetAttachMtx(const CMatrix* mtx);

	// 行列を取得
	CMatrix Matrix() const;

	// バウンディングボックスを取得
	CBounds Bounds() const;

	// コライダー更新
	void Update();
	// コライダー描画
	virtual void Render() = 0;

	/// <summary>
	/// 矩形同士の衝突判定
	/// </summary>
	/// <param name="rect1">矩形1</param>
	/// <param name="rect2">矩形2</param>
	/// <returns>trueならば、衝突している</returns>
	static bool CollisionRect(const CRect& rect1, const CRect& rect2);

	/// <summary>
	/// 矩形と点の衝突判定
	/// </summary>
	/// <param name="rect">矩形</param>
	/// <param name="point">点</param>
	/// <returns>trueならば、衝突している</returns>
	static bool CollisionRectPoint(const CRect& rect, const CVector2 point);

	/// <summary>
	/// 三角形と三角形の衝突判定
	/// </summary>
	/// <param name="t00">三角形1の頂点1</param>
	/// <param name="t01">三角形1の頂点2</param>
	/// <param name="t02">三角形1の頂点3</param>
	/// <param name="t10">三角形2の頂点1</param>
	/// <param name="t11">三角形2の頂点2</param>
	/// <param name="t12">三角形2の頂点3</param>
	/// <param name="hit">衝突した時の情報</param>
	/// <returns>trueならば、衝突している</returns>
	static bool CollisionTriangle(const CVector& t00, const CVector& t01, const CVector& t02,
		const CVector& t10, const CVector& t11, const CVector& t12,
		CHitInfo* hit);

	/// <summary>
	/// 三角形と線分の衝突判定
	/// </summary>
	/// <param name="t0">三角形の頂点1</param>
	/// <param name="t1">三角形の頂点2</param>
	/// <param name="t2">三角形の頂点3</param>
	/// <param name="ls">線分の始点</param>
	/// <param name="le">線分の終点</param>
	/// <param name="hit">衝突した時の情報</param>
	/// <returns>trueならば、衝突している</returns>
	static bool CollisionTriangleLine(const CVector& t0, const CVector& t1, const CVector& t2,
		const CVector& ls, const CVector& le,
		CHitInfo* hit, bool isLeftMain);

	/// <summary>
	/// 三角形とカプセルの衝突判定
	/// </summary>
	/// <param name="t0">三角形の頂点1</param>
	/// <param name="t1">三角形の頂点2</param>
	/// <param name="t2">三角形の頂点3</param>
	/// <param name="cs">カプセルを構成する線分の始点</param>
	/// <param name="ce">カプセルを構成する線分の終点</param>
	/// <param name="cr">カプセルの半径</param>
	/// <param name="hit">衝突した時の情報</param>
	/// <returns>trueならば、衝突している</returns>
	static bool CollisionTriangleCapsule(const CVector& t0, const CVector& t1, const CVector& t2,
		const CVector& cs, const CVector& ce, float cr,
		CHitInfo* hit, bool isLeftMain);

	/// <summary>
	/// 三角形と点の衝突判定
	/// </summary>
	/// <param name="t0">三角形の頂点1</param>
	/// <param name="t1">三角形の頂点2</param>
	/// <param name="t2">三角形の頂点3</param>
	/// <param name="tn">三角形の法線</param>
	/// <param name="p">点の座標</param>
	/// <returns>trueならば、衝突している</returns>
	static bool CollisionTrianglePoint(const CVector& t0, const CVector& t1, const CVector& t2, const CVector& tn, const CVector& p);
	/// <summary>
	/// 三角形と点の衝突判定
	/// </summary>
	/// <param name="t0">三角形の頂点1</param>
	/// <param name="t1">三角形の頂点2</param>
	/// <param name="t2">三角形の頂点3</param>
	/// <param name="p">点の座標</param>
	/// <returns>trueならば、衝突している</returns>
	static bool CollisionTrianglePoint(const CVector& t0, const CVector& t1, const CVector& t2, const CVector& p);

	/// <summary>
	/// 三角形と球の衝突判定
	/// </summary>
	/// <param name="t0">三角形の頂点1</param>
	/// <param name="t1">三角形の頂点2</param>
	/// <param name="t2">三角形の頂点3</param>
	/// <param name="sp">球の座標</param>
	/// <param name="sr">球の半径</param>
	/// <param name="hit">衝突した時の情報</param>
	/// <returns>trueならば、衝突している</returns>
	static bool CollisionTriangleSphere(const CVector& t0, const CVector& t1, const CVector& t2,
		const CVector& sp, const float sr,
		CHitInfo* hit, bool isLeftMain);

	/// <summary>
	/// 球と球の衝突判定
	/// </summary>
	/// <param name="sp0">球1の座標</param>
	/// <param name="sr0">球1の半径</param>
	/// <param name="sp1">球2の座標</param>
	/// <param name="sr1">球2の半径</param>
	/// <param name="hit">衝突した時の情報</param>
	/// <returns>trueならば、衝突している</returns>
	static bool CollisionSphere(const CVector& sp0, const float sr0,
		const CVector& sp1, const float sr1, CHitInfo* hit);

	/// <summary>
	/// 球と線分の衝突判定
	/// </summary>
	/// <param name="sp">球の座標</param>
	/// <param name="sr">球の半径</param>
	/// <param name="ls">線分の始点</param>
	/// <param name="le">線分の終点</param>
	/// <param name="hit">衝突した時の情報</param>
	/// <returns>trueならば、衝突している</returns>
	static bool CollisionSphereLine(const CVector& sp, const float sr,
		const CVector& ls, const CVector& le,
		CHitInfo* hit, bool isLeftMain);

	/// <summary>
	/// 球とカプセルの衝突判定
	/// </summary>
	/// <param name="sp">球の座標</param>
	/// <param name="sr">球の半径</param>
	/// <param name="cs">カプセルを構成する線分の始点</param>
	/// <param name="ce">カプセルを構成する線分の終点</param>
	/// <param name="cr">カプセルの半径</param>
	/// <param name="hit">衝突した時の情報</param>
	/// <returns>trueならば、衝突している</returns>
	static bool CollisionSphereCapsule(const CVector& sp, const float sr,
		const CVector& cs, const CVector& ce, float cr,
		CHitInfo* hit, bool isLeftMain);

	/// <summary>
	/// 線分と線分の衝突判定
	/// </summary>
	/// <param name="ls0">線分1の始点</param>
	/// <param name="le0">線分1の終点</param>
	/// <param name="ls1">線分2の始点</param>
	/// <param name="le1">線分2の終点</param>
	/// <param name="hit">衝突した時の情報</param>
	/// <returns>trueならば、衝突している</returns>
	static bool CollisionLine(const CVector& ls0, const CVector& le0,
		const CVector& ls1, const CVector& le1, CHitInfo* hit);

	/// <summary>
	/// カプセルとカプセルの衝突判定
	/// </summary>
	/// <param name="cs0">カプセル1を構成する線分の始点</param>
	/// <param name="ce0">カプセル1を構成する線分の終点</param>
	/// <param name="cr0">カプセル1の半径</param>
	/// <param name="cs1">カプセル2を構成する線分の始点</param>
	/// <param name="ce1">カプセル2を構成する線分の終点</param>
	/// <param name="cr1">カプセル2の半径</param>
	/// <param name="hit">衝突した時の情報</param>
	/// <returns>trueならば、衝突している</returns>
	static bool CollisionCapsule(const CVector& cs0, const CVector& ce0, float cr0,
		const CVector& cs1, const CVector& ce1, float cr1, 
		CHitInfo* hit);

	/// <summary>
	/// カプセルと線分の衝突判定
	/// </summary>
	/// <param name="cs">カプセルを構成する線分の始点</param>
	/// <param name="ce">カプセルを構成する線分の終点</param>
	/// <param name="cr">カプセルの半径</param>
	/// <param name="ls">線分の始点</param>
	/// <param name="le">線分の終点</param>
	/// <param name="hit">衝突した時の情報</param>
	/// <returns>trueならば、衝突している</returns>
	static bool CollisionCapsuleLine(const CVector& cs, const CVector& ce, float cr,
		const CVector& ls, const CVector& le,
		CHitInfo* hit, bool isLeftMain);

	/// <summary>
	/// メッシュと線分の衝突判定
	/// </summary>
	/// <param name="tris">メッシュを構成する三角形ポリゴンのリスト</param>
	/// <param name="ls">線分の始点</param>
	/// <param name="le">線分の終点</param>
	/// <param name="lb">線分のバウンディングボックス</param>
	/// <param name="hit">衝突した時の情報</param>
	/// <returns>trueならば、衝突している</returns>
	static bool CollisionMeshLine(const std::list<STVertexData>& tris,
		const CVector& ls, const CVector& le, const CBounds& lb,
		CHitInfo* hit, bool isLeftMain);

	/// <summary>
	/// メッシュと球の衝突判定
	/// </summary>
	/// <param name="tris">メッシュを構成する三角形ポリゴンのリスト</param>
	/// <param name="sphereCol">球コライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	/// <returns>trueならば、衝突している</returns>
	static bool CollisionMeshSpehre(const std::list<STVertexData>& tris,
		CColliderSphere* sphereCol, CHitInfo* hit, bool isLeftMain);

	/// <summary>
	/// メッシュと三角形の衝突判定
	/// </summary>
	/// <param name="tris">メッシュを構成する三角形ポリゴンのリスト</param>
	/// <param name="triCol">三角形コライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	/// <returns>trueならば、衝突している</returns>
	static bool CollisionMeshTriangle(const std::list<STVertexData>& tris,
		CColliderTriangle* triCol, CHitInfo* hit, bool isLeftMain);

	/// <summary>
	/// メッシュとカプセルの衝突判定
	/// </summary>
	/// <param name="tris">メッシュを構成する三角形ポリゴンのリスト</param>
	/// <param name="capsuleCol">カプセルコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	/// <returns>trueならば、衝突している</returns>
	static bool CollisionMeshCapsule(const std::list<STVertexData>& tris,
		CColliderCapsule* capsuleCol, CHitInfo* hit, bool isLeftMain);

	/// <summary>
	/// 点から線分までの最短距離を求める
	/// </summary>
	/// <param name="point">点の座標</param>
	/// <param name="lineS">線分の始点</param>
	/// <param name="lineE">線分の終点</param>
	/// <param name="nearest">最近点返却用の変数のポインタ</param>
	/// <returns>最短距離</returns>
	static float CalcDistancePointToLine(const CVector& point, const CVector& lineS, const CVector& lineE, CVector* nearest = nullptr);

	/// <summary>
	/// 線分と線分の最短距離を求める
	/// </summary>
	/// <param name="s1">線分1の始点</param>
	/// <param name="e1">線分1の終点</param>
	/// <param name="s2">線分2の始点</param>
	/// <param name="e2">線分2の終点</param>
	/// <returns></returns>
	static float CalcDistanceLine(const CVector& s1, const CVector& e1, const CVector& s2, const CVector& e2);

	/// <summary>
	/// 指定した点が三角形の内側にあるかどうか
	/// </summary>
	/// <param name="point">点の座標</param>
	/// <param name="t0">三角形の1つ目の頂点</param>
	/// <param name="t1">三角形の2つ目の頂点</param>
	/// <param name="t2">三角形の3つ目の頂点</param>
	/// <param name="n">三角形の法線</param>
	/// <returns>trueならば、三角形の内側にある</returns>
	static bool IsInsideTriangle(const CVector& p, const CVector& t0, const CVector& t1, const CVector& t2, const CVector& n);

	/// <summary>
	/// 衝突判定
	/// </summary>
	/// <param name="c0">コライダ1</param>
	/// <param name="c1">コライダ2</param>
	/// <param name="hit">ヒットした時の情報</param>
	/// <returns>trueならば、衝突している</returns>
	static bool Collision(CCollider* c0, CCollider* c1, CHitInfo* hit);

	/// <summary>
	/// レイとコライダーの衝突判定
	/// </summary>
	/// <param name="c">コライダー</param>
	/// <param name="start">レイの開始位置</param>
	/// <param name="end">レイの終了位置</param>
	/// <param name="hit">ヒットした時の情報</param>
	/// <returns>trueならば、衝突している</returns>
	static bool CollisionRay(CCollider* c, const CVector& start, const CVector& end, CHitInfo* hit);

	/// <summary>
	/// 衝突時の押し戻し割合を算出
	/// </summary>
	/// <param name="self">自身のコライダー</param>
	/// <param name="other">相手のコライダー</param>
	/// <returns>押し戻し割合（0.0f ～ 1.0f）</returns>
	static float CalcPushBackRatio(CCollider* self, CCollider* other);

protected:
	// コライダーの情報を更新
	virtual void UpdateCol() = 0;

	/// <summary>
	/// コライダーの設定
	/// </summary>
	/// <param name="owner">コライダーの持ち主</param>
	/// <param name="layer">衝突判定用レイヤー</param>
	void Set(CObjectBase* owner, ELayer layer);

	CBounds mBounds;		// バウンディングボックス

private:
	ELayer mLayer;			// 衝突判定レイヤー
	EColliderType mType;	// コライダーの種類
	CObjectBase* mpOwner;	// コライダーの持ち主
	bool mIsEnable;			// 有効かどうか
	bool mIsKinematic;		// 衝突時の押し戻しの影響を受けないかどうか
	float mWeight;			// コライダーの重量
	int mCollisionLayers;	// 衝突判定を行うレイヤーのビットフラグ
	int mCollisionTags;		// 衝突判定を行うオブジェクトタグのビットフラグ

	const CMatrix* mpAttachMtx;	// くっつける行列のポインター
	CMatrix mAttachMtx;			// くっつける行列の本体
};
