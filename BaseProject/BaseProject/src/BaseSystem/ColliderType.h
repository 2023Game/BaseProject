#pragma once

// コライダーの種類
enum class EColliderType
{
	eLine,		// 線分コライダー
	eSphere,	// 球コライダー
	eTriangle,	// 三角コライダー
	eCapsule,	// カプセルコライダー
	eMesh,		// メッシュコライダー
};
