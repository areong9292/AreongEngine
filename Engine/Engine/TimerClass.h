#ifndef _TIMERCLASS_H_
#define _TIMERCLASS_H_

#include <windows.h>

// 프레임 간의 시간을 계산하는 클래스
class TimerClass
{
public:
	TimerClass();
	TimerClass(const TimerClass&);
	~TimerClass();

	bool Initialize();
	void Frame();

	float GetTime();

private:
	INT64 m_frequency;
	float m_ticksPerMs;
	INT64 m_startTime;
	float m_frameTime;
};

#endif // !_TIMERCLASS_H_