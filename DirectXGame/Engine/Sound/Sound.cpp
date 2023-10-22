#include "Sound.h"
#include <fstream>
#include <cassert>

#pragma comment(lib,"xaudio2.lib")

size_t MyXAudio::SoundLoadWave(const char* filename)
{
	std::ifstream file;
	file.open(filename, std::ios_base::binary);
	assert(file.is_open());

	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	FormatChunk format = {};

	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) { assert(0); }
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	if (strncmp(data.id, "JUNK", 4) == 0) {
		file.seekg(data.size, std::ios_base::cur);
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0) { assert(0); }
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);
	file.close();

	SoundData _soundData = {};

	_soundData.wfex = format.fmt;
	_soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	_soundData.bufferSize = data.size;

	soundData_.push_back(_soundData);

	return handle_++;
}

void MyXAudio::SoundUnload(SoundData* soundData)
{
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

void MyXAudio::SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData)
{
	HRESULT result;

	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = (int32_t)soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}

void MyXAudio::SoundPlayWave(size_t handle, bool stop)
{
	HRESULT result;

	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData_[handle].wfex);
	assert(SUCCEEDED(result));

	if (stop) soundPtr_.push_back(pSourceVoice);

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData_[handle].pBuffer;
	buf.AudioBytes = (int32_t)soundData_[handle].bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;


	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}

void MyXAudio::SoundPlayLoopWave(size_t handle)
{
	HRESULT result;

	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData_[handle].wfex);
	assert(SUCCEEDED(result));

	soundPtr_.push_back(pSourceVoice);

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData_[handle].pBuffer;
	buf.AudioBytes = (int32_t)soundData_[handle].bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.LoopCount = XAUDIO2_LOOP_INFINITE;

	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}

void MyXAudio::StopAllLoopSound()
{
	for (size_t i = 0; i < soundPtr_.size(); i++)
	{
		soundPtr_[i]->Stop();
	}
	soundPtr_.resize(0);
}

void MyXAudio::Initialize()
{
	handle_ = 0;
	soundData_.resize(handle_);

	HRESULT result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);

	result = xAudio2_->CreateMasteringVoice(&masterVoice_);
}

void MyXAudio::Finalize()
{
	xAudio2_.Reset();
	for (size_t i = 0; i < soundData_.size(); i++)
	{
		SoundUnload(&soundData_[i]);
	}
}

MyXAudio* MyXAudio::GetInstance()
{
	static MyXAudio xaudio;
	return &xaudio;
}