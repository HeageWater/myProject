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
	if (strncmp(riff.chunk_.id_, "RIFF", 4) != 0) {
		assert(0);
	}
	if (strncmp(riff.type_, "WAVE", 4) != 0) {
		assert(0);
	}

	FormatChunk format = {};

	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk_.id_, "fmt ", 4) != 0) { assert(0); }
	assert(format.chunk_.size_ <= sizeof(format.fmt_));
	file.read((char*)&format.fmt_, format.chunk_.size_);

	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	if (strncmp(data.id_, "JUNK", 4) == 0) {
		file.seekg(data.size_, std::ios_base::cur);
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id_, "data", 4) != 0) { assert(0); }
	char* pBuffer = new char[data.size_];
	file.read(pBuffer, data.size_);
	file.close();

	SoundData _soundData = {};

	_soundData.wfex_ = format.fmt_;
	_soundData.pBuffer_ = reinterpret_cast<BYTE*>(pBuffer);
	_soundData.bufferSize_ = data.size_;

	soundData_.push_back(_soundData);

	return handle_++;
}

void MyXAudio::SoundUnload(SoundData* soundData)
{
	delete[] soundData->pBuffer_;

	soundData->pBuffer_ = 0;
	soundData->bufferSize_ = 0;
	soundData->wfex_ = {};
}

void MyXAudio::SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData)
{
	HRESULT result;

	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex_);
	assert(SUCCEEDED(result));

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer_;
	buf.AudioBytes = (int32_t)soundData.bufferSize_;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}

void MyXAudio::SoundPlayWave(size_t handle, bool stop)
{
	HRESULT result;

	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData_[handle].wfex_);
	assert(SUCCEEDED(result));

	if (stop) soundPtr_.push_back(pSourceVoice);

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData_[handle].pBuffer_;
	buf.AudioBytes = (int32_t)soundData_[handle].bufferSize_;
	buf.Flags = XAUDIO2_END_OF_STREAM;


	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}

void MyXAudio::SoundPlayLoopWave(size_t handle)
{
	HRESULT result;

	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData_[handle].wfex_);
	assert(SUCCEEDED(result));

	soundPtr_.push_back(pSourceVoice);

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData_[handle].pBuffer_;
	buf.AudioBytes = (int32_t)soundData_[handle].bufferSize_;
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