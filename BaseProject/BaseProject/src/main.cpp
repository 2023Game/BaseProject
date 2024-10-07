#include <Windows.h>
#include "glew.h"
#include "glut.h"
#include "GLFW/glfw3.h"
#include "CApplication.h"
#include "CInput.h"
#include "CCamera.h"
#include "CGamePause.h"

// 1秒間に実行するフレーム数
int gFPS = 60;
// 前回のフレームの経過時間
float gDeltaTime = 0.0f;

CApplication gApplication;

/* display関数
1秒間に60回実行される
*/
void display() {
	//各バッファーをクリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//行列のモードをモデルビューにする
	glMatrixMode(GL_MODELVIEW);
	//モデルビューの行列を単位行列にする
	glLoadIdentity();

	CInput::Update();
	gApplication.Update();

}

/// <summary>
/// マウスホイール回転時のコールバック関数
/// </summary>
/// <param name="window"></param>
/// <param name="x"></param>
/// <param name="y"></param>
void wheel(GLFWwindow* window, double x, double y)
{
	CInput::AddMouseWheel((int)y);
}

/*ウィンドウサイズ変更時の処理
void reshape(int width, int height)
width:画面幅
height:画面高さ
*/
void reshape(GLFWwindow* window, int width, int height) {
	CCamera* cam = CCamera::CurrentCamera();
	if (cam == nullptr)
	{
		glViewport(0, 0, width, height);	//画面の描画エリアの指定

		glMatrixMode(GL_PROJECTION);	//行列をプロジェクションモードへ変更
		glLoadIdentity();				//行列を初期化
		//3Dの画面を設定
		gluPerspective
		(
			CAMERA_FOVY,
			(double)width / (double)height,
			CAMERA_ZNEAR,
			CAMERA_ZFAR
		);

		glMatrixMode(GL_MODELVIEW);		//行列をモデルビューモードへ変更
		glLoadIdentity();				//行列を初期化
	}
	else
	{
		cam->Reshape(width, height);
	}
}
//
LARGE_INTEGER last_time;	//前回のカウンタ値
//１秒間に６０回描画するように調節する
void idle() {
	LARGE_INTEGER freq;		//一秒当たりのカウンタ数
	LARGE_INTEGER time;		//今回のカウンタ値

	//一秒間のカウンタ数を取得
	QueryPerformanceFrequency(&freq);

	if (last_time.QuadPart == 0) {
		QueryPerformanceCounter(&last_time);
	}
	do{
		//現在のシステムのカウント数を取得
		QueryPerformanceCounter(&time);

		//今のカウント-前回のカウント < 1秒当たりのカウント数で割る(1/gFPS秒当たりのカウント数)
	} while (time.QuadPart - last_time.QuadPart < freq.QuadPart / gFPS);
	gDeltaTime = (float)(time.QuadPart - last_time.QuadPart) / freq.QuadPart;
	last_time = time;

	//描画する関数を呼ぶ
	display();
	// 処理時間の計測結果を描画
	CDebugProfiler::Print();
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
#ifndef FULL_SCREEN
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_TITLE, NULL, NULL);
#else
	//Full Screen
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_TITLE, glfwGetPrimaryMonitor(), NULL);
#endif
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	CInput::Window(window);
	CInput::ShowCursor(false);
	/* Make the window's context current */
	glfwMakeContextCurrent(window);


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// GLEW を初期化する
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		// GLEW の初期化に失敗した
//		std::cerr << "Can't initialize GLEW" << std::endl;
		return 1;
	}
	// 垂直同期のタイミングを待つ  
	//glfwSwapInterval(1); 

	// ウィンドウのサイズ変更時に呼び出す処理の登録
	glfwSetWindowSizeCallback(window, reshape);
	reshape(window, WINDOW_WIDTH, WINDOW_HEIGHT);

	// マウスホイール回転時のコールバック関数を登録
	glfwSetScrollCallback(window, wheel);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//ライトの設定
	//固定シェーダー用
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float lightPosition[] = {0.0f, 100.0f, 100.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_NORMALIZE);

	//初期処理
	gApplication.Start();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{

		idle();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		int state = glfwGetKey(window, GLFW_KEY_ESCAPE);
		if (System::IsExitGame() || state == GLFW_PRESS) {
			//ESCキーでループ終了
			break;
		}
	}

	//終了処理
	gApplication.End();

	glfwTerminate();
	return 0;
}

// 目標フレームレートを取得
int Time::TargetFPS()
{
	return gFPS;
}

// 計算上での1フレームの経過時間を取得
float Time::CalcDeltaTime()
{
	if (gFPS == 0) return 0.0f;
	return 1.0f / gFPS;
}

// 前回のフレームのFPSを取得
float Time::FPS()
{
	if (gDeltaTime == 0.0f) return 0.0f;
	return 1.0f / gDeltaTime;
}

// 前回のフレームの経過時間を取得
float Time::DeltaTime()
{
	return gDeltaTime;
}
