#pragma once
#include <xaudio2.h>
#include <wrl.h>
#include <vector>

struct ChunkHeader {
	char id_[4];
	int32_t size_;
};
struct RiffHeader {
	ChunkHeader chunk_;
	char type_[4];
};
struct FormatChunk {
	ChunkHeader chunk_;
	WAVEFORMATEX fmt_;
};
struct SoundData {
	WAVEFORMATEX wfex_;
	BYTE* pBuffer_;
	size_t bufferSize_;
};

/// <summary>
/// サウンド
/// </summary>
class MyXAudio
{
private:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//主音量
	IXAudio2MasteringVoice* masterVoice_;

	//音データ
	std::vector<SoundData> soundData_;

	//サウンドポインタ
	std::vector<IXAudio2SourceVoice*> soundPtr_;

	//何番目か
	size_t handle_ = 0;

	//サウンド破棄
	void SoundUnload(SoundData* soundData);

private:

	MyXAudio() = default;
	~MyXAudio() = default;

	//コピーコンストラクタ・代入演算子削除
	MyXAudio& operator=(const MyXAudio&) = delete;
	MyXAudio(const MyXAudio&) = delete;
public:

	//シングルトン
	static MyXAudio* GetInstance();

	ComPtr<IXAudio2> xAudio2_;

	size_t SoundLoadWave(const char* filename);
	void SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData);
	void SoundPlayWave(size_t handle, bool stop = false);
	void SoundPlayLoopWave(size_t handle);
	void StopAllLoopSound();

	void Initialize();
	void Finalize();
};

