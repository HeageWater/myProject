#pragma once
#include <wrl.h>

using namespace Microsoft::WRL;

#include <xaudio2.h>

#pragma comment(lib,"xaudio2.lib")

#include <fstream>

struct ChunkHeader
{
	char id[4];
	int32_t size;
};

struct RiffHeader
{
	ChunkHeader chunk;
	char type[4];
};

struct FormatChunk
{
	ChunkHeader chunk;
	WAVEFORMATEX fmt;
};

struct SoundData
{
	//波形フォーマット
	WAVEFORMATEX wfex;
	//バッファの先頭アドレス
	BYTE* pBuffer;
	//バッファのサイズ
	unsigned int  bufferSize;
};

class Sound {
public:
	//サウンド設定
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;

	//サウンド読み込み
	SoundData SoundLoadWave(const char* filename);

	//サウンド削除
	void SoundunLoad(SoundData* soundData);

	//サウンド再生
	void SoundPlayWave(IXAudio2* xaudio2, const SoundData& soundData);
};