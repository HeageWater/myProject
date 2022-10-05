//#pragma once
//#include <wrl.h>
//
//using namespace Microsoft::WRL;
//
//#include <xaudio2.h>
//
//#pragma comment(lib,"xaudio2.lib")
//
//#include <fstream>
//
//struct ChunkHeader
//{
//	char id[4];
//	int32_t size;
//};
//
//struct RiffHeader
//{
//	ChunkHeader chunk;
//	char type[4];
//};
//
//struct FormatChunk
//{
//	ChunkHeader chunk;
//	WAVEFORMATEX fmt;
//};
//
//struct SoundData
//{
//	//波形フォーマット
//	WAVEFORMATEX wfex;
//	//バッファの先頭アドレス
//	BYTE* pBuffer;
//	//バッファのサイズ
//	unsigned int  bufferSize;
//};
//
//
//
//class Sound {
//
//	//サウンド読み込み
//	SoundData SoundLoadWave(const char* filename);
//
//	//サウンド削除
//	void SoundunLoad(SoundData* soundData);
//
//	//サウンド再生
//	void SoundPlayWave(IXAudio2* xaudio2, const SoundData& soundData);
//
//
//
//	//サウンド設定
//	ComPtr<IXAudio2> xAudio2;
//	IXAudio2MasteringVoice* masterVoice;
//
//	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
//
//	result = xAudio2->CreateMasteringVoice(&masterVoice);
//
//	SoundData soundData1 = SoundLoadWave("Resources/loop1.wav");
//
//	SoundPlayWave(xAudio2.Get(), soundData1);
//};