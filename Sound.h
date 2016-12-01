#pragma once
#include <xaudio2.h>
#include <map>
#include <vector>
#include <string>
#include "../Origin.h"
#define MAX_WAV 100 //WAVサウンド最大数


using namespace std;

class Sound
{
private:
	Sound();
	~Sound();

public:
	Sound(const Sound&) = delete;
	Sound& operator=(const Sound&) = delete;
	Sound(Sound&&) = delete;
	Sound& operator=(Sound&&) = delete;

	static Sound& getInstance()
	{
		static Sound inst;
		return inst;
	}

	HRESULT InitSound();
	int LoadSound(char* szFileName);
	void  PlaySound(int iSoundIndex);
	void Run();
	IXAudio2* m_pXAudio2;
	IXAudio2MasteringVoice* m_pMasteringVoice;
	IXAudio2SourceVoice* m_pSourceVoice[MAX_WAV];
	BYTE* m_pWavBuffer[MAX_WAV];					//波形データ（フォーマット等を含まない、純粋に波形データのみ）
	DWORD m_dwWavSize[MAX_WAV];						//波形データのサイズ
	int m1_iSoundIndex[10];							//今回読み込む音の番号
	int m_iSoundCurrentIndex;						//音の現在の再生番号
	bool bgm_flg;									//BGMがなってるか
	
	void SE_play(string SE_mode);		// SE再生
	void SE_stop(string SE_mode);	// SE停止
	void BGM_play(string BGM_mode);					// BGM再生
	void BGM_stop(string BGM_mode);					// BGM停止


};
