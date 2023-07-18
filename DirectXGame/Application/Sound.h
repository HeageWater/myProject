#pragma once
#include <xaudio2.h>
#include <wrl.h>
#include <vector>

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
	size_t bufferSize;
};

class MyXAudio
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	IXAudio2MasteringVoice* masterVoice;
	std::vector<SoundData> soundData;
	std::vector<IXAudio2SourceVoice*> soundPtr;
	size_t handle;
	void SoundUnload(SoundData* soundData);

	MyXAudio();
	~MyXAudio();
public:

	static MyXAudio* Get()
	{
		static MyXAudio xaudio;
		return &xaudio;
	}

	ComPtr<IXAudio2> xAudio2;

	size_t SoundLoadWave(const char* filename);
	void SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData);
	void SoundPlayWave(size_t handle, bool stop = false);
	void SoundPlayLoopWave(size_t handle);
	void StopAllLoopSound();
};

