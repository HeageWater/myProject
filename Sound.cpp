//#include "Sound.h"
//
////サウンド読み込み
//SoundData SoundLoadWave(const char* filename)
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
//void SoundunLoad(SoundData* soundData)
//{
//	delete[] soundData->pBuffer;
//	soundData->pBuffer = 0;
//	soundData->bufferSize = 0;
//	soundData->wfex = {};
//}
//
////サウンド再生
//void SoundPlayWave(IXAudio2* xaudio2, const SoundData& soundData)
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