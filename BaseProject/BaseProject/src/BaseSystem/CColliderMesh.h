#ifndef CCOLLIDERMESH_H
#define CCOLLIDERMESH_H
#include <vector>
#include "CModel.h"
#include "CCollider.h"

/*
モデルデータから三角コライダの生成
*/
class CColliderMesh : public CCollider
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">コライダーの持ち主</param>
	/// <param name="layer">衝突判定用レイヤー</param>
	/// <param name="model">モデルデータ</param>
	/// <param name="isKinematic">trueならば、衝突時に押し戻しの影響を受けない</param>
	/// <param name="weight">コライダーの重量</param>
	/// <param name="divX">コライダーのX軸の分割数</param>
	/// <param name="divY">コライダーのY軸の分割数</param>
	/// <param name="divZ">コライダーのZ軸の分割数</param>
	CColliderMesh(CObjectBase* owner, ELayer layer, CModel* model,
		bool isKinematic = false, float weight = 1.0f,
		int divX = 1, int divY = 1, int divZ = 1);
	// デストラクタ
	~CColliderMesh();

	void Set(CModel* model);
	const std::vector<STVertexData>& Get() const;

	// コライダー描画
	void Render() override;

protected:
	// コライダーの情報を更新
	void UpdateCol() override;

private:
	//三角コライダの配列作成
	std::vector<STVertexData> mVertices;
	int mDivX;
	int mDivY;
	int mDivZ;
};

#endif
