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
	// .wav 파일 포맷
	// 파일의 헤더 정보를 읽어 오디오 데이터를 로드한다
	// 다른 포맷의 경우 그 포맷에 맞는 구조체 생성 필요
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

	// 1차 버퍼와 2차 버퍼로 버퍼를 2개 사용한다
	// 1차 버퍼는 사운드 카드, USB 헤드셋과 같은 메인 사운드 버퍼
	// 2차 버퍼는 실제로 어플리케이션에 생성되고 소리가 로드되는 메모리 영역

	// 즉 2차 버퍼의 내용들을 조합해서 1차 버퍼로 사운드를 출력한다
	// 2차 버퍼는 사운드 갯수 만큼 생성됨
	IDirectSoundBuffer* m_primaryBuffer;
	IDirectSoundBuffer8* m_secondaryBuffer1;
};


#endif // !_SOUNDCLASS_H_
