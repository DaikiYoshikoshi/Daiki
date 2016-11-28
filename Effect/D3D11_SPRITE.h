//
//	@file	D3D11_SPRITE.h
//	@brief	スプライト
#pragma once

#include <windows.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")


#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}
#define DIMENSION 12.0f
#define WDIMENSION 10.0f
#define TEX_DIMENSION 4.0f

//
//
struct SPRITE_CONSTANT_BUFFER
{
	D3DXMATRIX mWVP;
	D3DXVECTOR4 vColor;
	D3DXVECTOR4 fAlpha;
	D3DXVECTOR4 TexScroll;
};
//
//
struct EffectVertex
{
	D3DXVECTOR3 Pos;  //位置
	D3DXVECTOR2 vTex; //テクスチャー座標
};
//
//
class D3D11_SPRITE
{
private:
	static DWORD m_dwWindowWidth;
	static DWORD m_dwWindowHeight;

	static ID3D11Device* m_pDevice;
	static ID3D11DeviceContext* m_pDeviceContext;
	static ID3D11SamplerState* m_pSampleLinear;
	static ID3D11VertexShader* m_pVertexShader;
	static ID3D11PixelShader* m_pPixelShader;
	static ID3D11InputLayout* m_pVertexLayout;
	static ID3D11Buffer* m_pVertexBuffer;
	static ID3D11Buffer* m_pConstantBuffer;
	static ID3D11BlendState* m_pBlendState;

	D3DXVECTOR4 m_vColor;
	float m_fAlpha;

public:
	D3D11_SPRITE();
	~D3D11_SPRITE();
	
	static HRESULT Init(ID3D11DeviceContext* pContext, DWORD width, DWORD height/*, D3DXVECTOR4 vColor, bool flg_alpha, WCHAR *filepass, D3DXVECTOR2 size, int animSpeed, float scale*/);
	static void SetCamera(D3DXMATRIX view, D3DXMATRIX proj);
	HRESULT CreateEffects(WCHAR *filepass, bool flg_alpha,/*float alpha,*/ D3DXVECTOR2 size, int animSpeed, float scale);
	void Render(int x,int y);
	//void D3D11_SPRITE::RenderSprite(D3DXMATRIX mView, D3DXMATRIX mProj, float x, float y, float z);
	void D3D11_SPRITE::RenderSprite(D3DXMATRIX& WVP ,float x, float y, float z);
	//void D3D11_SPRITE::RenderSprite(D3DXMATRIX& WVP);




	static D3DXMATRIX m_View;
	static D3DXMATRIX m_Proj;
	ID3D11ShaderResourceView* m_pAsciiTexture;
	float m_fScale;//25pixelを基準 25pixel=1.0f
	float m_fYaw, m_fPitch, m_fRoll;
	D3DXVECTOR2 m_vSize;	//アニメーションテクスチャの横、縦サイズ
	int m_iAnimSpeed;		//アニメーションのスピード
	int m_iAnimCount;		//アニメーションカウント
	bool m_isActive;			//表示フラグ
	D3DXVECTOR4 TexScroll;
	int count;
};