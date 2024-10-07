#pragma once
#include <initializer_list>

// オブジェクト識別用のタグ
enum class ETag
{
	eNone = -1,

	eField,	// フィールドを構成するオブジェクト
	eRideableObject,	// 乗ることができるオブジェクト
	eItem,	// アイテムのオブジェクト

	ePlayer,// プレイヤー
	eEnemy,	// エネミー

	eBullet,// 弾丸
	eFlame,	// 炎
	eSlash,	// 斬撃

	eCamera,// カメラ

	Num
};
// オブジェクト識別用のタグの初期化リスト
using Tags = std::initializer_list<ETag>;
