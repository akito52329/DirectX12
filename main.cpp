#include <Windows.h>
#ifdef _DEBUG
#include <iostream>
#include<tchar.h>
#endif

using namespace std;

// @brief コンソールがめんにフォーマット付き文字列を表示
// @param format フォーマット (%dとか%fとかの)
// @param 可変長引数
// @remarks この関数はデバック用です。デバック時にしか動作しません
//
void DebugOutputFormatString(const char* format, ...)
{
#ifdef _DEBUG
	va_list valist;
	va_start(valist, format);
	printf(format, valist);
	va_end(valist);
#endif

}

LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// ウィンドウが破棄されたら呼ばれる
	if (msg == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

const unsigned int window_width = 1280;
const unsigned int window_height = 720;

#ifdef _DEBUG
int main()
{
#else
int WINAPI WinMain(HINSTANCE, HINSSTANCE, LPSTR, int)
{
#endif 
	DebugOutputFormatString("Show window test.");
	HINSTANCE hInst = GetModuleHandle(nullptr);

	//ウィンドウクラスの生成＆登録
	WNDCLASSEX w = {};

	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProcedure; //コールバック関数の指定
	w.lpszClassName = _T("DX12Sample");//アプリケーションクラス名（適当でよい）
	w.hInstance = GetModuleHandle(nullptr); //ハンドルの取得

	RegisterClassEx(&w);//アプリケーションクラス（ウィンドウクラスの指定をOSに伝える）

	RECT wrc = { 0, 0, window_width, window_height };//ウィンドウサイズを決める

	//関数を使ってウィンドウのサイズを補正する
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウオブジェクトの作成
	HWND hwnd = CreateWindow(
	    w.lpszClassName,//クラス名指定
		_T("DX12テスト"),//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,//タイトルバーと境界線があるウィンドウ
		CW_USEDEFAULT,//表示ｘ座標はOSにお任せ
		CW_USEDEFAULT,//表示ｙ座標はOSにお任せ
		wrc.right - wrc.left,//ウィンドウ幅
		wrc.bottom - wrc.top,//ウィンドウ高
		nullptr,//親ウィンドウハンドル
		nullptr,//メニューハンドル
		w.hInstance,//呼び出しアプリケーションハンドル
		nullptr//追加パラメーター
		);

	//ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);

	MSG msg = {};

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//アプリケーションが終わるときにmessageがWM_QUITになる
		if (msg.message == WM_QUIT)
		{
			break;
		}
	}

	//もうクラスは使わないので登録解除する
	UnregisterClass(w.lpszClassName, w.hInstance);

	return 0;
}

