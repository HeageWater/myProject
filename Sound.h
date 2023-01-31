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
//class xAudio {
//public:
//	//サウンド設定
//	ComPtr<IXAudio2> xAudio2;
//	IXAudio2MasteringVoice* masterVoice;
//
//	//サウンド読み込み
//	SoundData SoundLoadWave(const char* filename);
//
//	//サウンド削除
//	void SoundunLoad(SoundData* soundData);
//
//	//サウンド再生
//	void SoundPlayWave(IXAudio2* xaudio2, const SoundData& soundData);
//};

#pragma once
#include <xaudio2.h>
#include <wrl.h>
#include <vector>

#pragma comment(lib,"xaudio2.lib")

struct ChunkHeader {
	char id[4];
	int32_t size;
};
struct RiffHeader {
	ChunkHeader chunk;
	char type[4];
};
struct FormatChunk {
	ChunkHeader chunk;
	WAVEFORMATEX fmt;
};
struct SoundData {
	WAVEFORMATEX wfex;
	BYTE* pBuffer;
	unsigned int bufferSize;
};

class MyXAudio
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	IXAudio2MasteringVoice* masterVoice;
	std::vector<SoundData> soundData;
	std::vector<IXAudio2SourceVoice*> soundPtr;
	int handle;
	void SoundUnload(SoundData* soundData);
public:
	ComPtr<IXAudio2> xAudio2;
	MyXAudio();
	~MyXAudio();
	int SoundLoadWave(const char* filename);
	void SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData);
	void SoundPlayWave(int handle, bool stop = false);
	void SoundPlayLoopWave(int handle);
	void StopAllLoopSound();
};

