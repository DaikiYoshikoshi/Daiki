//
//	@file	MAIN.cpp
//	@brief	メインクラス

#include "MAIN.h"
//グローバル変数
MAIN* g_pMain = NULL;


//関数プロトタイプの宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//
//
//アプリケーションのエントリー関数 
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, INT)
{
	g_pMain = new MAIN;
	if (g_pMain != NULL)
	{
		if (SUCCEEDED(g_pMain->InitWindow(hInstance, 0, 0, WINDOW_WIDTH,
			WINDOW_HEIGHT, APP_NAME)))
		{
			g_pMain->CreateConsoleWindow();
			if (SUCCEEDED(g_pMain->InitD3D()))
			{
				g_pMain->Loop();
			}
		}
		//アプリ終了
		g_pMain->CloseConsoleWindow();
		g_pMain->DestroyD3D();
		delete g_pMain;
	}
	return 0;
}
//
//
//OSから見たウィンドウプロシージャー（実際の処理はMAINクラスのプロシージャーで処理）
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return g_pMain->MsgProc(hWnd, uMsg, wParam, lParam);
}
//
//
//ウィンドウ作成
HRESULT MAIN::InitWindow(HINSTANCE hInstance,
	INT iX, INT iY, INT iWidth, INT iHeight, LPCWSTR WindowName)
{
	// ウィンドウの定義
	WNDCLASSEX  wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = WindowName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc);

	//ウィンドウの作成
	m_hWnd = CreateWindow(WindowName, WindowName, WS_OVERLAPPEDWINDOW,
		0, 0, iWidth, iHeight, 0, 0, hInstance, 0);
	if (!m_hWnd)
	{
		return E_FAIL;
	}
	//ウインドウの表示
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return S_OK;
}
//
//
//ウィンドウプロシージャー
LRESULT MAIN::MsgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_KEYDOWN:
		switch ((char)wParam)
		{
		case VK_ESCAPE://ESCキーで修了
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}
//
//
//メッセージループとアプリケーション処理の入り口
void MAIN::Loop()
{
	//メッシュ作成
	//XFileRead* xfileRead = new XFileRead;
	//xfileRead->ReadXFilePath();
	//XFile* xfile = xfileRead->GetXFile("ステージ");
	/*camera_ = new Camera;*/
	//stageMesh_ = new Stage;
	//stageMesh_->Read(m_hWnd, m_pDevice, m_pDeviceContext,xfile->GetFileName());
	//メッシュ作成　終わり

	//初期化
	//CD3DXMESH::Init(m_hWnd, m_pDevice, m_pDeviceContext);
	CD3DXMESH::InitShader(m_hWnd, m_pDevice, m_pDeviceContext);
	CD3DXSKINMESH::Init(m_hWnd, m_pDevice, m_pDeviceContext);
	D3D11_SPRITE::Init(m_pDeviceContext, WINDOW_WIDTH, WINDOW_HEIGHT/*, D3DXVECTOR4(1, 1, 1, 1)*/);
	mainScene_->Init(m_hWnd, m_pDevice, m_pDeviceContext);

	Sound::getInstance().Run();

	// メッセージループ
	MSG msg = { 0 };
	start = timeGetTime();
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//アプリケーションの処理はここから飛ぶ。
			App();

			//FPS調整
			while (timeGetTime() - start < 1000 / FPS)
			{
				Sleep(1);
			}
			start = timeGetTime();
		}
	}
	//アプリケーションの終了
}
//
//
//アプリケーション処理。アプリのメイン関数。
void MAIN::App()
{
	Update();
	Render();
}
//
//
//
HRESULT MAIN::InitD3D()
{
	// デバイスとスワップチェーンの作成
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = WINDOW_WIDTH;
	sd.BufferDesc.Height = WINDOW_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		0, &pFeatureLevels, 1, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice,
		pFeatureLevel, &m_pDeviceContext)))
	{
		return FALSE;
	}
	//各種テクスチャーと、それに付帯する各種ビューを作成

	//バックバッファーテクスチャーを取得（既にあるので作成ではない）
	ID3D11Texture2D *pBackBuffer_Tex;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer_Tex);
	//そのテクスチャーに対しレンダーターゲットビュー(RTV)を作成
	m_pDevice->CreateRenderTargetView(pBackBuffer_Tex, NULL, &m_pBackBuffer_TexRTV);
	SAFE_RELEASE(pBackBuffer_Tex);

	//デプスステンシルビュー用のテクスチャーを作成
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = WINDOW_WIDTH;
	descDepth.Height = WINDOW_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pBackBuffer_DSTex);
	//そのテクスチャーに対しデプスステンシルビュー(DSV)を作成
	m_pDevice->CreateDepthStencilView(m_pBackBuffer_DSTex, NULL, &m_pBackBuffer_DSTexDSV);

	//レンダーターゲットビューと深度ステンシルビューをパイプラインにバインド
	m_pDeviceContext->OMSetRenderTargets(1, &m_pBackBuffer_TexRTV, m_pBackBuffer_DSTexDSV);
	//ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &vp);
	//ラスタライズ設定
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode = D3D11_CULL_NONE;
	rdc.FillMode = D3D11_FILL_SOLID;
	ID3D11RasterizerState* pIr = NULL;
	m_pDevice->CreateRasterizerState(&rdc, &pIr);
	m_pDeviceContext->RSSetState(pIr);
	SAFE_RELEASE(pIr);

	//初期化
	mainScene_ = new Main_Scene;
	mainScene_->DebugInit(m_pDeviceContext);
	return S_OK;
}
//
//
//
void MAIN::DestroyD3D()
{
	//delete camera_;
	//camera_ = nullptr;
	delete mainScene_;
	mainScene_ = nullptr;
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pBackBuffer_TexRTV);
	SAFE_RELEASE(m_pBackBuffer_DSTexDSV);
	SAFE_RELEASE(m_pBackBuffer_DSTex);
	SAFE_RELEASE(m_pDevice);
}

//
//
//更新処理
void MAIN::Update()
{
	//camera_->Update();
	mainScene_->Update();
}

//
//
//シーンを画面にレンダリング
void MAIN::Render()
{

	//画面クリア（実際は単色で画面を塗りつぶす処理）
	float ClearColor[4] = { 0,0,1,1 };// クリア色作成　RGBAの順
	m_pDeviceContext->ClearRenderTargetView(m_pBackBuffer_TexRTV, ClearColor);//画面クリア
	m_pDeviceContext->ClearDepthStencilView(m_pBackBuffer_DSTexDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//深度バッファクリア


	//// ３人称視点処理　ビュートランスフォーム カメラをキャラの後ろに配置するだけ
	//D3DXVECTOR3 vEyePt(0.0f, 2.0f, -4.0f); //カメラ（視点）位置
	//D3DXVECTOR3 vLookatPt(0.0f, 1.7f, 0.0f);//注視位置
	//D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//上方位置

	////視点と注視点の両方をキャラの姿勢（回転と位置）行列で曲げて移動すればいい
	//D3DXMATRIX OriMat, Tran, Yaw;
	//D3DXMatrixTranslation(&Tran, 0, 0, 0);
	//D3DXMatrixRotationY(&Yaw, 0);
	//OriMat = Yaw*Tran;

	//D3DXVec3TransformCoord(&vEyePt, &vEyePt, &OriMat);
	//D3DXVec3TransformCoord(&vLookatPt, &vLookatPt, &OriMat);

	//D3DXMatrixLookAtLH(&mView, &vEyePt, &vLookatPt, &vUpVec);
	//camera_->Render(mView, mProj);

	//// プロジェクショントランスフォーム（射影変換）
	//D3DXMatrixPerspectiveFovLH(&mProj, D3DX_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 1000.0f);
	//レンダリング
	//camera_->Render();
	/*D3DXMATRIX mView = camera_->GetView();
	D3DXMATRIX mProj = camera_->GetProj();*/

	CD3DXMESH::SetCamera(Camera::mView_, Camera::mProj_);
	CD3DXSKINMESH::SetCamera(Camera::mView_, Camera::mProj_);
	D3D11_SPRITE::SetCamera(Camera::mView_, Camera::mProj_);
	mainScene_->Render(/*mView, mProj*/);

	//画面更新（バックバッファをフロントバッファに）
	m_pSwapChain->Present(0, 0);

	/*char Str[512];
	sprintf(Str, "fps:%f", start);
	SetWindowTextA(m_hWnd, Str);*/

	//FPS計算表示
	static DWORD time = 0;
	static int frame = 0;
	frame++;
	char str[50];
	sprintf(str, "fps=%d", frame);
	if (timeGetTime() - time>1000)
	{
		time = timeGetTime();
		frame = 0;
		SetWindowTextA(m_hWnd, str);
	}
}

//
//	@brief	デバック用コンソールウィンドウ表示
void MAIN::CreateConsoleWindow()
{
	//AllocConsole();
	//hConsole = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
	//*stdout = *_fdopen(hConsole, "w");
	//setvbuf(stdout, NULL, _IONBF, 0);
}

//
//	@brief	デバッグ用コンソールウィンドウ終了
void MAIN::CloseConsoleWindow()
{
	//_close(hConsole);
}