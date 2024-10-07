#pragma once

#define GAME_TITLE "GameTitle"	//ゲームタイトル

#define WINDOW_WIDTH 1280		//ウィンドウの幅
#define WINDOW_HEIGHT 720		//ウィンドウの高さ

#define CAMERA_FOVY 60.0f		//カメラの画角
#define CAMERA_ZNEAR 1.0f		//カメラの描画最短距離
#define CAMERA_ZFAR 1000.0f		//カメラの描画最長距離

//#define FULL_SCREEN	//フルスクリーンにする場合は定義する

//配列のサイズ取得をマクロ化
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

//リソースファイルフォルダ
#define RES_DIR "res\\"
//領域解放をマクロ化
#define SAFE_DELETE(del) {if(del != nullptr) delete del; del = nullptr;}
//動的配列の領域解放をマクロ化
#define SAFE_DELETE_ARRAY(ary) {if(ary != nullptr) delete[] ary; ary = nullptr;}



#include "System.h"
#include "Time.h"
#include "CDebugCamera.h"
#include "CDebugPrint.h"
#include "CDebugProfiler.h"
#include "CResourceManager.h"
