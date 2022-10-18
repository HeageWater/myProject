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
	//�g�`�t�H�[�}�b�g
	WAVEFORMATEX wfex;
	//�o�b�t�@�̐擪�A�h���X
	BYTE* pBuffer;
	//�o�b�t�@�̃T�C�Y
	unsigned int  bufferSize;
};

class Sound {
public:
	//�T�E���h�ݒ�
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;

	//�T�E���h�ǂݍ���
	SoundData SoundLoadWave(const char* filename);

	//�T�E���h�폜
	void SoundunLoad(SoundData* soundData);

	//�T�E���h�Đ�
	void SoundPlayWave(IXAudio2* xaudio2, const SoundData& soundData);
};