#ifndef _SOUNDCLASS_H_
#define _SOUNDCLASS_H_

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

class SoundClass
{
private:
	// .wav ���� ����
	// ������ ��� ������ �о� ����� �����͸� �ε��Ѵ�
	// �ٸ� ������ ��� �� ���˿� �´� ����ü ���� �ʿ�
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

public:
	SoundClass();
	SoundClass(const SoundClass& other);
	~SoundClass();

	bool Initialize(HWND hwnd);
	void Shutdown();

private:
	bool InitializeDirectSound(HWND hwnd);
	void ShutdownDirectSound();

	bool LoadWaveFile(char* filename, IDirectSoundBuffer8** secondaryBuffer);
	void ShutdownWaveFile(IDirectSoundBuffer8**);

	bool PlayWaveFile();

private:
	IDirectSound8* m_DirectSound;

	// 1�� ���ۿ� 2�� ���۷� ���۸� 2�� ����Ѵ�
	// 1�� ���۴� ���� ī��, USB ���°� ���� ���� ���� ����
	// 2�� ���۴� ������ ���ø����̼ǿ� �����ǰ� �Ҹ��� �ε�Ǵ� �޸� ����

	// �� 2�� ������ ������� �����ؼ� 1�� ���۷� ���带 ����Ѵ�
	// 2�� ���۴� ���� ���� ��ŭ ������
	IDirectSoundBuffer* m_primaryBuffer;
	IDirectSoundBuffer8* m_secondaryBuffer1;
};


#endif // !_SOUNDCLASS_H_
