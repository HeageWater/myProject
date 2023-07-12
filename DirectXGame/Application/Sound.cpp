//#include "Sound.h"
//
//#define DIRECTINPUT_VERSION 0x0800
//#include <dinput.h>
//#include <cassert>
//#include <DirectXMath.h>
//
//#pragma comment(lib,"dinput8.lib")
//#pragma comment(lib,"dxguid.lib")
//
////サウンド読み込み
//SoundData xAudio::SoundLoadWave(const char* filename)
//{
//	HRESULT result;
//
//	//ファイルオープン
//	std::ifstream file;
//	file.open(filename, std::ios_base::binary);
//	assert(file.is_open());
//
//	//データ読み込み(wav)
//	RiffHeader riff;
//	file.read((char*)&riff, sizeof(riff));
//
//	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
//	{
//		assert(0);
//	}
//
//	if (strncmp(riff.type, "WAVE", 4) != 0)
//	{
//		assert(0);
//	}
//
//	FormatChunk format = {};
//
//	file.read((char*)&format, sizeof(ChunkHeader));
//
//	/*if (strncmp(format.chunk.id, "fmt", 4) != 0)
//	{
//		assert(0);
//	}*/
//
//	assert(format.chunk.size <= sizeof(format.fmt));
//
//	file.read((char*)&format.fmt, format.chunk.size);
//
//	ChunkHeader data;
//
//	file.read((char*)&data, sizeof(data));
//
//	if (strncmp(data.id, "JUNK", 4) == 0)
//	{
//		file.seekg(data.size, std::ios_base::cur);
//		file.read((char*)&data, sizeof(data));
//	}
//
//	if (strncmp(data.id, "data", 4) != 0)
//	{
//		assert(0);
//	}
//
//	char* pBuffer = new char[data.size];
//	file.read(pBuffer, data.size);
//
//	//ファイルクローズ
//	file.close();
//
//	//データをreturn
//	SoundData soundData = {};
//
//	soundData.wfex = format.fmt;
//	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
//	soundData.bufferSize = data.size;
//
//	return soundData;
//}
//
////サウンド削除
//void xAudio::SoundunLoad(SoundData* soundData)
//{
//	delete[] soundData->pBuffer;
//	soundData->pBuffer = 0;
//	soundData->bufferSize = 0;
//	soundData->wfex = {};
//}
//
////サウンド再生
//void xAudio::SoundPlayWave(IXAudio2* xaudio2, const SoundData& soundData)
//{
//	HRESULT result;
//
//	IXAudio2SourceVoice* pSouceVoice = nullptr;
//	result = xaudio2->CreateSourceVoice(&pSouceVoice, &soundData.wfex);
//	assert(SUCCEEDED(result));
//
//	XAUDIO2_BUFFER buf{};
//	buf.pAudioData = soundData.pBuffer;
//	buf.AudioBytes = soundData.bufferSize;
//	buf.Flags = XAUDIO2_END_OF_STREAM;
//
//	result = pSouceVoice->SubmitSourceBuffer(&buf);
//	result = pSouceVoice->Start();
//}

#include "Sound.h"
#include <fstream>
#include <cassert>

MyXAudio::MyXAudio()
{
	handle = 0;
	soundData.resize(handle);

	HRESULT result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	result = xAudio2->CreateMasteringVoice(&masterVoice);
}

MyXAudio::~MyXAudio()
{
	xAudio2.Reset();
	for (size_t i = 0; i < soundData.size(); i++)
	{
		SoundUnload(&soundData[i]);
	}
}

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

	soundData.push_back(_soundData);

	return handle++;
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
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData[handle].wfex);
	assert(SUCCEEDED(result));

	if (stop) soundPtr.push_back(pSourceVoice);

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData[handle].pBuffer;
	buf.AudioBytes = (int32_t)soundData[handle].bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;


	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}

void MyXAudio::SoundPlayLoopWave(size_t handle)
{
	HRESULT result;

	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData[handle].wfex);
	assert(SUCCEEDED(result));

	soundPtr.push_back(pSourceVoice);

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData[handle].pBuffer;
	buf.AudioBytes = (int32_t)soundData[handle].bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.LoopCount = XAUDIO2_LOOP_INFINITE;

	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}

void MyXAudio::StopAllLoopSound()
{
	for (size_t i = 0; i < soundPtr.size(); i++)
	{
		soundPtr[i]->Stop();
	}
	soundPtr.resize(0);
}
