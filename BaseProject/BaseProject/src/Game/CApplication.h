#pragma once

class CApplication
{
private:
public:
	~CApplication();

	//最初に一度だけ実行するプログラム
	void Start();
	//終了時に実行するプログラム
	void End();
	//繰り返し実行するプログラム
	void Update();
};