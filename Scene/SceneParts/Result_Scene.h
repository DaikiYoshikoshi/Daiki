#pragma once
#include <iostream>
#include <map>
#include <string>
#include "./../SceneParts/Scene.h"
#include "./../SceneManager/SceneRoot.h"
#include "./../SceneParts/Title_Scene.h"
#include "./../SceneParts/Main_Scene.h"
#include "./../../Mesh/CD3DXSKINMESH.h"
#include "./../../Charactor/CharactorCreator.h"
#include "./../../Camera.h"
#include "./../../UI/D3D11_SPRITE.h"

//--------//
#define UI_TIME 12 //éûä‘ï\é¶Ç…égópÇ∑ÇÈUIÇÃêî
#define SEMICOLON 10
//--------//

class Result_Scene :public Scene
{
private:

	CD3DXSKINMESH* mesh_princess_;
	CD3DXMESH* mesh_stage_;

	D3D11_TEXT* debugText_;

	Camera* camera_;

	CD3DXSKINMESH* meshPlayer1_;
	CD3DXSKINMESH* meshPlayer2_;
	CD3DXSKINMESH* meshPlayer3_;
	CD3DXSKINMESH* meshPlayer4_;
	CharactorCreator* creator_;

	//--------//
	TD_Graphics* uiTime_[UI_TIME]; //0~9 ; ,
	TD_Graphics* uiResult_;
	unsigned int time_;
	//--------//

public:
	Result_Scene();
	virtual ~Result_Scene();

	static int m10_Time;
	static int m1_Time;
	static int s10_Time;
	static int s1_Time;
	static int c10_Time;
	static int c1_Time;

	void Init();
	SceneBase* Update(SceneRoot* root);
	void Render();
	void Destroy();
};