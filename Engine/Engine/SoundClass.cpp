#include "SoundClass.h"

SoundClass::SoundClass()
{
	m_DirectSound = nullptr;
	m_primaryBuffer = nullptr;
	m_secondaryBuffer1 = nullptr;
}

SoundClass::SoundClass(const SoundClass & other)
{
}

SoundClass::~SoundClass()
{
}

bool SoundClass::Initialize(HWND hwnd)
{
	bool result;

	// DirectSound 인터페이스와 1차 버퍼를 초기화
	result = InitializeDirectSound(hwnd);
	if (!result)
	{
		return false;
	}

	// .wav 파일을 로드하고 2차 버퍼를 초기화
	result = LoadWaveFile("../Engine/data/sound01.wav", &m_secondaryBuffer1);
	if (!result)
	{
		return false;
	}

	// .wav 파일 재생
	result = PlayWaveFile();
	if (!result)
	{
		return false;
	}

	return true;
}

void SoundClass::Shutdown()
{
	// 오디오 데이터를 가지고 있는 2차 버퍼를 해제
	ShutdownWaveFile(&m_secondaryBuffer1);

	// 1차 버퍼와 DirectSound 인터페이스를 해제
	ShutdownDirectSound();

	return;
}

bool SoundClass::InitializeDirectSound(HWND hwnd)
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	// DirectSound 인터페이스 생성
	result = DirectSoundCreate8(NULL, &m_DirectSound, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Cooperative 수준 설정
	result = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (FAILED(result))
	{
		return false;
	}

	// 1차 버퍼 desc 작성
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);

	// DSBCAPS_CTRLVOLUME - 1차 버퍼의 볼륨을 조절할 수 있도록 셋팅
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// 사운드 버퍼 생성 - 이로 인해 1차 버퍼로 사운드 장치 컨트롤 가능
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// 웨이브 파일 포맷 셋팅
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// 사운드 장치릐 포맷을 오디오 파일의 포맷으로 지정
	result = m_primaryBuffer->SetFormat(&waveFormat);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void SoundClass::ShutdownDirectSound()
{
	if (m_primaryBuffer != nullptr)
	{
		m_primaryBuffer->Release();
		m_primaryBuffer = nullptr;
	}

	if (m_DirectSound != nullptr)
	{
		m_DirectSound->Release();
		m_DirectSound = nullptr;
	}
}

// wav파일을 로드하여 그 데이터를 2차 버퍼에 복사한다
bool SoundClass::LoadWaveFile(char* filename, IDirectSoundBuffer8** secondaryBuffer)
{
	int error;
	FILE* filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferSesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;

	// wav파일 바이너리로 오픈
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	// 파일 내용을 읽어서 wave 포맷인지 체크한다
	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	if ((waveFileHeader.chunkId[0] != 'R') ||
		(waveFileHeader.chunkId[1] != 'I') ||
		(waveFileHeader.chunkId[2] != 'F') ||
		(waveFileHeader.chunkId[3] != 'F'))
	{
		return false;
	}

	if ((waveFileHeader.format[0] != 'W') ||
		(waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') ||
		(waveFileHeader.format[3] != 'E'))
	{
		return false;
	}

	if ((waveFileHeader.subChunkId[0] != 'f') ||
		(waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') ||
		(waveFileHeader.subChunkId[3] != ' '))
	{
		return false;
	}

	// Check that the audio format is WAVE_FORMAT_PCM.
	if(waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}
	
	// Check that the wave file was recorded in stereo format.
	if(waveFileHeader.numChannels != 2)
	{
		return false;
	}
	
	// Check that the wave file was recorded at a sample rate of 44.1 KHz.
	if(waveFileHeader.sampleRate != 44100)
	{
		return false;
	}
	
	// Ensure that the wave file was recorded in 16 bit format.
	if(waveFileHeader.bitsPerSample != 16)
	{
		return false;
	}
	
	// Check for the data chunk header.
	if((waveFileHeader.dataChunkId[0] != 'd') ||
	   (waveFileHeader.dataChunkId[1] != 'a') ||
	   (waveFileHeader.dataChunkId[2] != 't') ||
	   (waveFileHeader.dataChunkId[3] != 'a'))
	{
		return false;
	}


	return false;
}

void SoundClass::ShutdownWaveFile(IDirectSoundBuffer8 **)
{
}

bool SoundClass::PlayWaveFile()
{
	return false;
}
