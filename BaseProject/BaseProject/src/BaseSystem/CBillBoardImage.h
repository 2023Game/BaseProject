#pragma once
#include "CImage3D.h"

// 常にカメラの方向を向くイメージ
class CBillBoardImage : public CImage3D
{
public:
	// コンストラクタ
	CBillBoardImage(std::string path,
		ETag tag = ETag::eNone,
		ETaskPauseType pause = ETaskPauseType::eDefault);
	// デストラクタ
	virtual ~CBillBoardImage();

private:
};