#ifndef CMATRIX_H
#define CMATRIX_H

class CVector;

/*
マトリクスクラス
4行4列の行列データを扱います
*/
class CMatrix {
private:
	//4×4の行列データを設定
	float mM[4][4];

public:
	// コンストラクタ
	CMatrix();
	// コンストラクタ
	CMatrix(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);

	//*演算子のオーバーロード
	//CMatrix * float の演算結果を返す
	CMatrix operator*(const float& x);
	//+演算子のオーバーロード
	//CMatrix1 + CMatrix2 の演算結果を返す
	CMatrix operator+(const CMatrix& m);
	//+=演算子のオーバーロード
	//CMatrix1 += CMatrix2 の演算を行う
	void operator+=(const CMatrix& m);
	//クオータニオンで回転行列を設定する
	CMatrix Quaternion(float x, float y, float z, float w);
	CVector Position() const;
	void Position(const CVector& pos);
	CVector VectorZ() const; //Z軸ベクトルの取得
	CVector VectorX() const; //X軸ベクトルの取得
	CVector VectorY() const; //Y軸ベクトルの取得
	//転置行列取得
	CMatrix Transpose() const;
	//逆行列取得
	CMatrix Inverse() const;
	//表示確認用
	//4×4の行列を画面出力
	void Print();
	//単位行列の作成
	CMatrix Identity();
	//拡大縮小行列の作成
	//Scale(倍率X, 倍率Y, 倍率Z)
	CMatrix Scale(float sx, float sy, float sz);
	CMatrix Scale(const CVector& scale);
	//行列値の取得
	//M(行, 列)
	//mM[行][列]を取得
	float M(int r, int c) const;
	//回転行列（Y軸）の作成
	//RotateY(角度)
	CMatrix RotateY(float degree);
	//回転行列（Z軸）の作成
	//RotateZ(角度)
	CMatrix RotateZ(float degree);
	//回転行列（X軸）の作成
	//RotateX(角度)
	CMatrix RotateX(float degree);
	//移動行列の作成
	//Translate(移動量X, 移動量Y, 移動量Z)
	CMatrix Translate(float mx, float my, float mz);
	CMatrix Translate(const CVector& translation);
	//*演算子のオーバーロード
	//CMatrix * CMatrix の演算結果を返す
	const CMatrix operator*(const CMatrix& m) const;
	//行列値の代入
	//M(行数, 列数, 値)
	void M(int row, int col, float value);
	float* M() const;
	//要素数の取得
	int Size();

	/// <summary>
	/// 正面ベクトルと上方向ベクトルから回転行列を取得
	/// </summary>
	/// <param name="forward">正面ベクトル</param>
	/// <param name="up">上方向ベクトル</param>
	/// <returns>回転行列</returns>
	static CMatrix LookRotation(const CVector& forward, const CVector& up);

	// 零行列
	// [0, 0, 0, 0]
	// [0, 0, 0, 0]
	// [0, 0, 0, 0]
	// [0, 0, 0, 0]
	const static CMatrix zero;

	// 単位行列
	// [1, 0, 0, 0]
	// [0, 1, 0, 0]
	// [0, 0, 1, 0]
	// [0, 0, 0, 1]
	const static CMatrix identity;
};
#endif
