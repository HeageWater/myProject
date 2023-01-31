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

class Sound
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
	Sound();
	~Sound();
	int SoundLoadWave(const char* filename);
	void SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData);
	void SoundPlayWave(int handle, bool stop = false);
	void SoundPlayLoopWave(int handle);
	void StopAllLoopSound();
};

