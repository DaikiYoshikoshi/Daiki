//
//	@file	D3D11_SPRITE.cpp
//	@brief	スプライト

#include "D3D11_SPRITE.h"

D3DXMATRIX D3D11_SPRITE::m_View;
D3DXMATRIX D3D11_SPRITE::m_Proj;
DWORD D3D11_SPRITE::m_dwWindowWidth;
DWORD D3D11_SPRITE::m_dwWindowHeight;
//D3DXVECTOR4 D3D11_SPRITE::m_vColor;
//float D3D11_SPRITE::m_fAlpha;

ID3D11Device* D3D11_SPRITE::m_pDevice = nullptr;
ID3D11DeviceContext* D3D11_SPRITE::m_pDeviceContext = nullptr;
ID3D11SamplerState* D3D11_SPRITE::m_pSampleLinear = nullptr;
ID3D11VertexShader* D3D11_SPRITE::m_pVertexShader = nullptr;
ID3D11PixelShader* D3D11_SPRITE::m_pPixelShader = nullptr;
ID3D11InputLayout* D3D11_SPRITE::m_pVertexLayout = nullptr;
ID3D11Buffer* D3D11_SPRITE::m_pVertexBuffer = nullptr;
ID3D11Buffer* D3D11_SPRITE::m_pConstantBuffer = nullptr;
ID3D11BlendState* D3D11_SPRITE::m_pBlendState = nullptr;

//
//
//
D3D11_SPRITE::D3D11_SPRITE()
{
	ZeroMemory(this,sizeof(D3D11_SPRITE));
	m_fScale=1.0f;
	m_isActive = true;
	m_iAnimCount = 0;
	TexScroll = D3DXVECTOR4(0, 0, 0, 0);
	count = 0;
}
//
//
//
D3D11_SPRITE::~D3D11_SPRITE()
{
}
//
//
//
HRESULT D3D11_SPRITE::Init(ID3D11DeviceContext* pContext, DWORD width, DWORD height/*, D3DXVECTOR4 vColor*/)
{

	//m_vColor = vColor;
	//m_fAlpha = vColor.w;

	//デバイスとコンテキストをコピー
	m_pDeviceContext=pContext;
	m_pDeviceContext->GetDevice(&m_pDevice);
	//window サイズ
	m_dwWindowWidth=width;
	m_dwWindowHeight=height;


	

	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc,sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	m_pDevice->CreateSamplerState( &SamDesc, &m_pSampleLinear);
	
	////////////////////////////////////////////
	//移動
	////テクスチャーを作成
	//if (FAILED(D3DX11CreateShaderResourceViewFromFile(m_pDevice, filepass, NULL, NULL, &m_pAsciiTexture, NULL)))
	//{
	//	return E_FAIL;
	//}

	////////////////////////////////////////////////

	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob *pCompiledShader=NULL;
	ID3DBlob *pErrors=NULL;
	//ブロブからバーテックスシェーダー作成
	if(FAILED(D3DX11CompileFromFile(L"./Effect/D3D11_SPRITE.hlsl",NULL,NULL,"VS","vs_5_0",0,0,NULL,&pCompiledShader,&pErrors,NULL)))
	{
		MessageBox(0,L"hlsl読み込み失敗",NULL,MB_OK);
		return E_FAIL;
    }
	SAFE_RELEASE(pErrors);

	if(FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(),pCompiledShader->GetBufferSize(),NULL,&m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0,L"バーテックスシェーダー作成失敗",NULL,MB_OK);
		return E_FAIL;
	}
	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
	};
	UINT numElements = sizeof(layout)/sizeof(layout[0]);

	//頂点インプットレイアウトを作成
	if( FAILED( m_pDevice->CreateInputLayout( layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout ) ) )
		return FALSE;
	//ブロブからピクセルシェーダー作成
	if(FAILED(D3DX11CompileFromFile(L"./Effect/D3D11_SPRITE.hlsl",NULL,NULL,"PS","ps_5_0",0,0,NULL,&pCompiledShader,&pErrors,NULL)))
	{
		MessageBox(0,L"hlsl読み込み失敗",NULL,MB_OK);
		return E_FAIL;
    }
	SAFE_RELEASE(pErrors);
	if(FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(),pCompiledShader->GetBufferSize(),NULL,&m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0,L"ピクセルシェーダー作成失敗",NULL,MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);
	//コンスタントバッファー作成　ここでは変換行列渡し用
	D3D11_BUFFER_DESC cb;
	cb.BindFlags= D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth= sizeof( SPRITE_CONSTANT_BUFFER );
	cb.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags	=0;
	cb.StructureByteStride=0;
	cb.Usage=D3D11_USAGE_DYNAMIC;

	if( FAILED(m_pDevice->CreateBuffer( &cb,NULL,&m_pConstantBuffer)))
	{
		return E_FAIL;
	}

	

	return S_OK;
}

//
//
//
void D3D11_SPRITE::SetCamera(D3DXMATRIX view, D3DXMATRIX proj)
{
	m_View = view;;
	m_Proj = proj;
}

//
//
//テクスチャの作成
HRESULT D3D11_SPRITE::CreateEffects(WCHAR *filepass, bool flg_alpha/*, float alpha*/, D3DXVECTOR2 size, int animSpeed, float scale)
{
	m_vSize = size;
	m_iAnimSpeed = animSpeed;
	//m_fAlpha = alpha;

	//画像表示位置指定　デフォルトは全域
	D3DXVECTOR4 rect = D3DXVECTOR4(0.0f, 0.0f, 1.0f / m_vSize.x, 1.0f / m_vSize.y);
	EffectVertex vertices[] =
	{
		D3DXVECTOR3(0, scale, 0), D3DXVECTOR2(rect.x, rect.w),//頂点1,
		D3DXVECTOR3(0, 0, 0), D3DXVECTOR2(rect.x, rect.y),//頂点2
		D3DXVECTOR3(scale, scale, 0), D3DXVECTOR2(rect.z, rect.w), //頂点3
		D3DXVECTOR3(scale, 0, 0), D3DXVECTOR2(rect.z, rect.y), //頂点4
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(EffectVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		return E_FAIL;
	}

	//テクスチャーを作成
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(m_pDevice, filepass, NULL, NULL, &m_pAsciiTexture, NULL)))
	{
		return E_FAIL;
	}

	if (flg_alpha)
	{
		//抜け色設定
		D3D11_BLEND_DESC bd2;
		ZeroMemory(&bd2, sizeof(D3D11_BLEND_DESC));
		bd2.IndependentBlendEnable = false;
		bd2.AlphaToCoverageEnable = false;
		bd2.RenderTarget[0].BlendEnable = true;
		bd2.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd2.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bd2.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		bd2.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bd2.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		bd2.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd2.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		if (FAILED(m_pDevice->CreateBlendState(&bd2, &m_pBlendState)))
		{
			return E_FAIL;
		}
	}
}

//
//
//
void D3D11_SPRITE::Render(int x,int y)
{
	//// ビュートランスフォーム（視点座標変換）
	//D3DXVECTOR3 vEyePt( 0.0f, 0.0f,-1 ); //カメラ（視点）位置
	//D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );//注視位置
	//D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );//上方位置
	//D3DXMatrixLookAtLH( &m_mView, &vEyePt, &vLookatPt, &vUpVec );

	//// プロジェクショントランスフォーム（射影変換）
	//D3DMATRIX mOtho =
	//{
	//	2.0f /(float) ( m_dwWindowWidth ), 0.0f, 0.0f, 0.0f,
	//	0.0f, -2.0f /(float) ( m_dwWindowHeight ), 0.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f, 0.0f,
	//	-1.0f, 1.0f, 0.0f, 1.0f
	//};

	//m_mProj=mOtho;
	//トポロジー
	m_pDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	//頂点インプットレイアウトをセット
	m_pDeviceContext->IASetInputLayout( m_pVertexLayout );

	//使用するシェーダーの登録
	m_pDeviceContext->VSSetShader(m_pVertexShader,NULL,0);
	m_pDeviceContext->PSSetShader(m_pPixelShader,NULL,0);

	//このコンスタントバッファーを使うシェーダーの登録
	m_pDeviceContext->VSSetConstantBuffers(0,1,&m_pConstantBuffer );
	m_pDeviceContext->PSSetConstantBuffers(0,1,&m_pConstantBuffer );

	//テクスチャーをシェーダーに渡す
	m_pDeviceContext->PSSetSamplers(0,1,&m_pSampleLinear);
	m_pDeviceContext->PSSetShaderResources(0,1,&m_pAsciiTexture);
	
	//render
	//RenderSprite(x,y);
}
//
//
//
//void D3D11_SPRITE::RenderSprite(D3DXMATRIX mView, D3DXMATRIX mProj,float x, float y, float z)
//{
//	//トポロジー
//	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
//	//頂点インプットレイアウトをセット
//	m_pDeviceContext->IASetInputLayout(m_pVertexLayout);
//
//	//使用するシェーダーの登録
//	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
//	m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
//
//	//このコンスタントバッファーを使うシェーダーの登録
//	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
//	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
//
//	//テクスチャーをシェーダーに渡す
//	m_pDeviceContext->PSSetSamplers(0, 1, &m_pSampleLinear);
//	m_pDeviceContext->PSSetShaderResources(0, 1, &m_pAsciiTexture);
//
//	//ワールド変換
//	D3DXMATRIX mWorld, Rot, Scale;
//	D3DXMatrixIdentity(&mWorld);
//	D3DXMatrixTranslation(&mWorld,x,y,z);
//
//	//ワールドトランスフォーム（絶対座標変換）
//	D3DXMatrixRotationX(&Rot, m_fYaw);//単純にyaw回転させる
//	D3DXMatrixScaling(&Scale, m_fScale, m_fScale, m_fScale);
//	mWorld = Scale * mWorld * Rot;
//
//	//シェーダーのコンスタントバッファーに各種データを渡す	
//	D3D11_MAPPED_SUBRESOURCE pData;
//	SPRITE_CONSTANT_BUFFER cb;
//	if( SUCCEEDED( m_pDeviceContext->Map( m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData ) ) )
//	{
//		//ワールド、カメラ、射影行列を渡す
//		D3DXMATRIX m=mWorld*mView*mProj;
//		D3DXMatrixTranspose( &m, &m );
//		cb.mWVP=m;
//		//カラーを渡す
//		cb.vColor=m_vColor;
//		//透明度を渡す
//		cb.fAlpha.x=m_fAlpha;
//
//		//テクスチャースクロールの増分を渡す
//		static D3DXVECTOR4 TexScroll(0, 0, 0, 0);
//		static int count = 0;
//		if (count < m_iAnimSpeed) count++;
//		if (count >= m_iAnimSpeed)
//		{
//			TexScroll.x += 1.0f / m_vSize.x;
//			count = 0;
//			m_iAnimCount++;
//			if ((m_iAnimCount % (int)m_vSize.x) == 0)
//			{
//				TexScroll.y += 1.0f / m_vSize.y;
//				TexScroll.x = 0.0f;
//			}
//			if ((m_iAnimCount % (int)(m_vSize.x * m_vSize.y)) == 0)
//			{
//				m_isActive = false;
//				m_iAnimCount = 0;
//				TexScroll.x = 0.0f;
//				TexScroll.y = 0.0f;
//			}
//		}
////		TexScroll.y += 0.0001;
//		cb.TexScroll = D3DXVECTOR4(TexScroll.x, TexScroll.y, 0, 0);
//
//		memcpy_s( pData.pData, pData.RowPitch, (void*)( &cb), sizeof( cb ) );
//		m_pDeviceContext->Unmap( m_pConstantBuffer, 0 );
//	}
//	//バーテックスバッファーをセット
//	UINT stride = sizeof(EffectVertex);
//	UINT offset = 0;
//	m_pDeviceContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
//
//	//抜け色
//	UINT ColorKey=0xffffffff;
//	m_pDeviceContext->OMSetBlendState(m_pBlendState, NULL, ColorKey);
//	m_pDeviceContext->Draw( 4, 0 );
//	m_pDeviceContext->OMSetBlendState(0, NULL, ColorKey);
//}


void D3D11_SPRITE::RenderSprite(D3DXMATRIX& WVP, float x, float y, float z)
{
	//トポロジー
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//頂点インプットレイアウトをセット
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout);

	//使用するシェーダーの登録
	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	//このコンスタントバッファーを使うシェーダーの登録
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	//テクスチャーをシェーダーに渡す
	m_pDeviceContext->PSSetSamplers(0, 1, &m_pSampleLinear);
	m_pDeviceContext->PSSetShaderResources(0, 1, &m_pAsciiTexture);

	//ワールド変換
	D3DXMATRIX mWorld, Rot, Scale;
	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixTranslation(&mWorld, x, y, z);

	//ワールドトランスフォーム（絶対座標変換）
	D3DXMatrixRotationX(&Rot, m_fYaw);//単純にyaw回転させる
	D3DXMatrixScaling(&Scale, m_fScale, m_fScale, m_fScale);
	mWorld = Scale * mWorld *Rot;

	//シェーダーのコンスタントバッファーに各種データを渡す	
	D3D11_MAPPED_SUBRESOURCE pData;
	SPRITE_CONSTANT_BUFFER cb;
	if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド、カメラ、射影行列を渡す
		D3DXMATRIX m = mWorld *m_View*m_Proj;
		//D3DXMatrixTranspose(&m, &m);
		cb.mWVP = WVP * m;
		
		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);
		
		//カラーを渡す
		cb.vColor = D3DXVECTOR4(1,1,1,1);
		//透明度を渡す
		cb.fAlpha.x = 1;

		//テクスチャースクロールの増分を渡す
		
		if (count < m_iAnimSpeed) count++;
		if (count >= m_iAnimSpeed)
		{
			TexScroll.x += 1.0f / m_vSize.x;
			count = 0;
			m_iAnimCount++;
			if ((m_iAnimCount % (int)m_vSize.x) == 0)
			{
				TexScroll.y += 1.0f / m_vSize.y;
				TexScroll.x = 0.0f;
			}
			if ((m_iAnimCount % (int)(m_vSize.x * m_vSize.y)) == 0)
			{
				m_isActive = false;
				m_iAnimCount = 0;
				TexScroll.x = 0.0f;
				TexScroll.y = 0.0f;
			}
		}
		//		TexScroll.y += 0.0001;
		cb.TexScroll = D3DXVECTOR4(TexScroll.x, TexScroll.y, 0, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pDeviceContext->Unmap(m_pConstantBuffer, 0);
	}
	//バーテックスバッファーをセット
	UINT stride = sizeof(EffectVertex);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//抜け色
	UINT ColorKey = 0xffffffff;
	m_pDeviceContext->OMSetBlendState(m_pBlendState, NULL, ColorKey);
	m_pDeviceContext->Draw(4, 0);
	m_pDeviceContext->OMSetBlendState(0, NULL, ColorKey);
}

//void D3D11_SPRITE::RenderSprite(D3DXMATRIX& WVP)
//{
//	//使用するシェーダーのセット
//	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
//	m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
//	//シェーダーのコンスタントバッファーに各種データを渡す
//	D3D11_MAPPED_SUBRESOURCE pData;
//	SPRITE_CONSTANT_BUFFER cb;
//	if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
//	{
//		//ワールド、カメラ、射影行列を渡す
//		cb.mWVP = WVP;
//		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);
//
//		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
//		m_pDeviceContext->Unmap(m_pConstantBuffer, 0);
//	}
//	//このコンスタントバッファーをどのシェーダーで使うか
//	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
//	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
//	//頂点インプットレイアウトをセット
//	m_pDeviceContext->IASetInputLayout(m_pVertexLayout);
//	//プリミティブ・トポロジーをセット
//	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
//	//テクスチャーをシェーダーに渡す
//	m_pDeviceContext->PSSetSamplers(0, 1, &m_pSampleLinear);
//	m_pDeviceContext->PSSetShaderResources(0, 1, &m_pAsciiTexture);
//	//プリミティブをレンダリング
//	m_pDeviceContext->Draw(4, 0);
//}