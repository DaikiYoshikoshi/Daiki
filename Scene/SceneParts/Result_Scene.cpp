#include "./Result_Scene.h"

int Result_Scene::m10_Time;
int Result_Scene::m1_Time;
int Result_Scene::s10_Time;
int Result_Scene::s1_Time;
int Result_Scene::c10_Time;
int Result_Scene::c1_Time;

Result_Scene::Result_Scene()
{
	//new
	camera_ = new Camera;
	creator_ = new CharactorCreator;
	debugText_ = new D3D11_TEXT;

	//--------//
	uiResult_ = new TD_Graphics;
	for (int i = 0; i < UI_TIME; i++)
	{
		uiTime_[i] = new TD_Graphics;
	}
	//--------//
}

Result_Scene::~Result_Scene()
{
	//delete
	delete camera_;
	camera_ = nullptr;
	delete creator_;
	creator_ = nullptr;

	delete debugText_;
	debugText_ = nullptr;

	//--------//
	delete uiResult_;
	uiResult_ = nullptr;
	for (int i = 0; i < UI_TIME; i++)
	{
		delete uiTime_[i];
		uiTime_[i] = nullptr;
	}
	//--------//
}

void Result_Scene::Init()
{
	float scale = 5;
	float dist = 5;
	float speed = 1.0f / 40.0f;

	//“Ç‚Ýž‚Ý
	camera_->gazePoint_ = D3DXVECTOR3(0, 5.5, 0);
	camera_->movePow_ = D3DXVECTOR3(0, 7.5, -25);

	meshPlayer1_ = creator_->LoadChara(CharactorCreator::player1_);
	meshPlayer2_ = creator_->LoadChara(CharactorCreator::player2_);
	meshPlayer3_ = creator_->LoadChara(CharactorCreator::player3_);
	meshPlayer4_ = creator_->LoadChara(CharactorCreator::player4_);

	mesh_princess_ = creator_->LoadChara("•P");

	mesh_stage_ = creator_->LoadStage("ƒ^ƒCƒgƒ‹");

	//--------//
	D3DXVECTOR2 ui_scale(1623, 600);
	uiResult_->Init(L"./UI/UI_Tex/clear_font2.png", D3DXVECTOR2(0, 0), ui_scale, D3DXVECTOR4(1.0, 1.0, 1.0, 1.0), GrapRect(0.0f, 1.0f, 0.0f, 1.0f));

	D3DXVECTOR2 time_scale(128, 256);
	float rect = 0.083;
	for (int i = 0; i < UI_TIME; i++)
	{
		uiTime_[i]->Init(L"./UI/UI_Tex/number.png", D3DXVECTOR2(0, 0), time_scale, D3DXVECTOR4(1.0, 1.0, 1.0, 1.0), GrapRect(0.0f, 1.0f, 0.0f + i*rect, rect + i*rect));
	}
	//--------//
}

void Result_Scene::Destroy()
{
	SAFE_DELETE(mesh_princess_);
}

SceneBase* Result_Scene::Update(SceneRoot* root)
{
	SceneBase* next = this;

	static bool nextSceneFlg = false;

	for (int i = 0; i < 4; i++)
	{
		if (GamePad::checkInput(i, GamePad::InputName::B)/*GetKeyState(VK_SPACE) & 0x80*/)
		{
			nextSceneFlg = true;
		}
	}

	if (nextSceneFlg)
	{
		next = new Title_Scene;
		nextSceneFlg = false;
	}

	GamePad::update();
	return next;
}

void Result_Scene::Render()
{
	float scale = 0.5;
	float dist = 5;
	float speed = 1.0f / 40.0f;
	float sp = 0.001;

	meshPlayer1_->Render(D3DXVECTOR3(-dist, 0, 0), 0, D3DXVECTOR3(scale, scale, scale));
	meshPlayer1_->m_pD3dxMesh->m_pAnimController->AdvanceTime(speed, NULL);
	meshPlayer2_->Render(D3DXVECTOR3(dist, 0, 0), 0, D3DXVECTOR3(scale, scale, scale));
	meshPlayer2_->m_pD3dxMesh->m_pAnimController->AdvanceTime(speed, NULL);
	meshPlayer3_->Render(D3DXVECTOR3(-dist - 5, 0, 0), 0, D3DXVECTOR3(scale, scale, scale));
	meshPlayer3_->m_pD3dxMesh->m_pAnimController->AdvanceTime(speed, NULL);
	meshPlayer4_->Render(D3DXVECTOR3(dist + 5, 0, 0), 0, D3DXVECTOR3(scale, scale, scale));
	meshPlayer4_->m_pD3dxMesh->m_pAnimController->AdvanceTime(speed, NULL);


	mesh_princess_->Render(D3DXVECTOR3(0, 0, 0), 0, D3DXVECTOR3(scale, scale, scale), camera_->movePow_);
	mesh_princess_->m_pD3dxMesh->m_pAnimController->AdvanceTime(speed, NULL);

	camera_->movePow_.x += (GetKeyState(VK_LEFT) & 0x80)*sp + -(GetKeyState(VK_RIGHT) & 0x80)*sp;
	camera_->movePow_.y += (GetKeyState('W') & 0x80)*sp + -(GetKeyState('S') & 0x80)*sp;
	camera_->movePow_.z += (GetKeyState(VK_UP) & 0x80)*sp + -(GetKeyState(VK_DOWN) & 0x80)*sp;

	//ƒXƒe[ƒW‚Ì•`‰æ
	mesh_stage_->Render(D3DXVECTOR3(0, 0.8, 0), D3DXVECTOR3(0, 0, 0), 2);

	char str[256];
	sprintf(str, "pos.x:%f pos.y:%f pos.z:%f", camera_->movePow_.x, camera_->movePow_.y, camera_->movePow_.z);
	debugText_->Render(str, 0, 50);

	//---------------//
	//ŽžŠÔ•`‰æ
	float posX = 750;
	float posY = 270;
	float space = 80;
	D3DXVECTOR2 time_scale(0.5, 0.5);
	//•ª
	int minutes10 = time_ / (FPS*FPS * 10) % 6;// time_ / FPS % 6;
	int minutes1 = time_ / (FPS*FPS) % 10;// time_ / FPS;
	uiTime_[m10_Time]->Render(D3DXVECTOR2(posX, posY), time_scale, true);
	uiTime_[m1_Time]->Render(D3DXVECTOR2(posX + space * 0.8, posY), time_scale, true);
	uiTime_[SEMICOLON]->Render(D3DXVECTOR2(posX + space * 1.33, posY), time_scale, true);

	//•b
	int second10 = time_ / (FPS * 10) % 6;
	int second1 = time_ / FPS % 10;
	uiTime_[s10_Time]->Render(D3DXVECTOR2(posX + space * 2, posY), time_scale, true);
	uiTime_[s1_Time]->Render(D3DXVECTOR2(posX + space * 2.8, posY), time_scale, true);
	uiTime_[SEMICOLON]->Render(D3DXVECTOR2(posX + space * 3.3, posY), time_scale, true);

	//ƒ~ƒŠ•b
	int conma10 = time_ / 10 % 6;
	int conma1 = time_ % 10;
	uiTime_[c10_Time]->Render(D3DXVECTOR2(posX + space * 4, posY), time_scale, true);
	uiTime_[c1_Time]->Render(D3DXVECTOR2(posX + space * 4.8, posY), time_scale, true);

	posY = 30;
	posX = 180;
	uiResult_->Render(D3DXVECTOR2(posX, posY), D3DXVECTOR2(1, 1), true);
	//---------------//

	camera_->Render();
}