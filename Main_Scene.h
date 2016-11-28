//
//	@file	Main_Scene.h
//	@brief	���C���Q�[���V�[���Ǘ��N���X
//	@date	2016/11/08
//	@outher	�m�ȍ��c
#pragma once
#include <vector>
#include <time.h>
#include "./Origin.h"
#include "./Read/XFileRead.h"
#include "./Read/ParameterRead.h"
#include "./Read/MotionRead.h"
#include "./Stage/Stage.h"
#include "./Charactor/Player/PlayerCommon.h"
#include "./Charactor/CharactorCreator.h"
#include "./Charactor/Enemy/EnemyJobManager.h"
#include "./Stage/Spawn.h"
#include "./Charactor/Princess.h"
#include "./Collision.h"
#include "./Camera.h"

class Main_Scene
{
	enum scene
	{
		StartS=0,
		MainS,
		EndS,
	};
	scene scene_;

	XFileRead* xfileRead;
	XFile* xfile;
	ParameterRead* parameter;
	MotionRead* motionRead_;

	HWND wnd_;
	ID3D11Device* device_;
	ID3D11DeviceContext* deviceContext_;

	Stage* stage_;				//�X�e�[�W���
	Collision* ray_;
	JobManager** virChar_;	//�m�F�p���L�����N�^�[
	Princess* princess_;	//�P
	Camera* camera_;

	int spawnAmount_;	//�X�|�[���Q�[�g�̐�
	//Spawn** spawn_;
	EnemyJobManager* virEnemy_;	//�m�p���L�����N�^�[
	CD3DXSKINMESH* slime_;
	/*EnemyJobManager* virEnemy_;*/

	void CollisionControl();	//�Փ˔���Ǘ�

	std::vector<CharactorManager*> charList_;	//�X�e�[�W��ɑ��݂���L�����N�^�[
	std::vector<EnemyJobManager*> enemyList_;	//�X�e�[�W��ɑ��݂���G�l�~�[
	std::vector<EnemyJobManager*> killList_;	//���S�G�l�~�[���X�g
	unsigned int time_;

	void GameStart();	//�Q�[���J�n��������
	void GameMain();	//�Q�[�����C��
	void GameEnd();		//�Q�[���I��

	void EnemyDestroy();	//�G�l�~�[���S����

	//�f�o�b�N
	D3D11_TEXT* debugText_;
	
	//�G�t�F�N�g
	D3D11_SPRITE* uisword_;
	D3D11_SPRITE* uiseeld_;
	D3D11_SPRITE* uimagic_;
	D3D11_SPRITE* uibom_;
	
	double insTime_,pushTime_;

	void PlayerDebug();
	void EnemyDebug();
public:
	Main_Scene();
	~Main_Scene();

	void Init(HWND m_hWnd,ID3D11Device* m_pDevice,ID3D11DeviceContext* m_pDeviceContext);	//������
	
	HRESULT DebugInit(ID3D11DeviceContext* m_pDeviceContext);								//�f�o�b�O�`�揉����
	HRESULT EffectInit(ID3D11DeviceContext* m_pDeviceContext);

	void Update();								//�X�V
	void EffectRender();						//�`��
	void Render();
};
