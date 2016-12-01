//
//	@file	MAIN.h
//	@brief	メインクラス

//ヘッダーファイルのインクルード
#include <io.h>
#include <fcntl.h>
#include "./Origin.h"
#include "./Main_Scene.h"
#include "./Camera.h"
//
//
//
class MAIN
{

	int hConsole = 0;

	long start;
public:
	//デバッグ用コンソールウィンドウ
	void CreateConsoleWindow();
	void CloseConsoleWindow();

	HRESULT InitWindow(HINSTANCE,INT,INT,INT,INT,LPCWSTR);	//ウィンドウ初期化
	HRESULT InitD3D();										//初期化
	LRESULT MsgProc(HWND,UINT,WPARAM,LPARAM);				//ウィンドウプロシージャ
	void Loop();		//アプリケーション処理入り口
	void App();			//アプリケーション処理
	void Update();		//更新処理
	void Render();		//描画処理
	void DestroyD3D();	//解放処理

	HWND m_hWnd;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext *m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pBackBuffer_TexRTV;
	ID3D11DepthStencilView* m_pBackBuffer_DSTexDSV;
	ID3D11Texture2D* m_pBackBuffer_DSTex;

	Main_Scene* mainScene_;		//ゲームメインシーンクラス
	//Stage* stageMesh_;
	//Camera* camera_;

};
