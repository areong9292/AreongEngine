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

	// DirectSound �������̽��� 1�� ���۸� �ʱ�ȭ
	result = InitializeDirectSound(hwnd);
	if (!result)
	{
		return false;
	}

	// .wav ������ �ε��ϰ� 2�� ���۸� �ʱ�ȭ
	result = LoadWaveFile("../Engine/data/sound01.wav", &m_secondaryBuffer1);
	if (!result)
	{
		return false;
	}

	// .wav ���� ���
	result = PlayWaveFile();
	if (!result)
	{
		return false;
	}

	return true;
}

void SoundClass::Shutdown()
{
	// ����� �����͸� ������ �ִ� 2�� ���۸� ����
	ShutdownWaveFile(&m_secondaryBuffer1);

	// 1�� ���ۿ� DirectSound �������̽��� ����
	ShutdownDirectSound();

	return;
}

bool SoundClass::InitializeDirectSound(HWND hwnd)
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	// DirectSound �������̽� ����
	result = DirectSoundCreate8(NULL, &m_DirectSound, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Cooperative ���� ����
	result = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (FAILED(result))
	{
		return false;
	}

	// 1�� ���� desc �ۼ�
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);

	// DSBCAPS_CTRLVOLUME - 1�� ������ ������ ������ �� �ֵ��� ����
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// ���� ���� ���� - �̷� ���� 1�� ���۷� ���� ��ġ ��Ʈ�� ����
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// ���̺� ���� ���� ����
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// ���� ��ġ�l ������ ����� ������ �������� ����
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

// wav������ �ε��Ͽ� �� �����͸� 2�� ���ۿ� �����Ѵ�
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

	// wav���� ���̳ʸ��� ����
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	// ���� ������ �о wave �������� üũ�Ѵ�
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
