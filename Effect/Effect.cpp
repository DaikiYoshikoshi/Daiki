//
//	@file	Effect.cpp
//	@brief	ƒGƒtƒFƒNƒg‚Ìˆ—
//	@date	2016/11/22
//	@outher	‹g‰z‘å‹R
//#include "Effect.h"



//Effect::Effect()
//{
//	effect_ = new D3D11_SPRITE;
//	effect2_ = new D3D11_SPRITE;
//}
//
//
//Effect::~Effect()
//{
//}
//
//HRESULT Effect::EffectInit(ID3D11DeviceContext * m_pDeviceContext)
//{
//	D3DXVECTOR4 vColor(1.0f, 1.0f, 1.0f, 1.0f);
//	if (FAILED(effect_->Init(m_pDeviceContext, WINDOW_WIDTH, WINDOW_HEIGHT, vColor, true, L"./Effect/Effect_Tex/explo.png",D3DXVECTOR2(8, 2), 8, 6.0f)))
//	{
//		return E_FAIL;
//	}
//
//	if (FAILED(effect2_->Init(m_pDeviceContext, WINDOW_WIDTH, WINDOW_HEIGHT, vColor, true, L"./Effect/Effect_Tex/explo2.png", D3DXVECTOR2(8, 2), 3, 4.0f)))
//	{
//		return E_FAIL;
//	}
//	return S_OK;
//}
//
//void Effect::Render(D3DXMATRIX mView, D3DXMATRIX mProj)
//{
//	if (effect_->m_isActive)
//	{
//		effect_->RenderSprite(mView, mProj, 0, 0);
//	}
//	
//	effect2_->RenderSprite(mView, mProj, 1.5, -2);
//}
//

