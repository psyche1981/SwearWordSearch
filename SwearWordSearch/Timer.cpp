#include "Timer.h"

Timer::Timer()
{
}

Timer::~Timer()
{
}

void Timer::Begin()
{
	if (_ended)
	{
		_ended = false;
		_start = std::chrono::steady_clock::now();
	}
	else
	{
		std::cout << "Tried to start the timer while it was still running" << std::endl;
	}
}

float Timer::End()
{
	_ended = true;
	return GetElapsed();
}

void Timer::Reset()
{
	End();
	Begin();
}

float Timer::GetElapsed()
{	
	if (!_ended)
	{
		std::chrono::duration<float> elapsed = std::chrono::steady_clock::now() - _start;
		return elapsed.count();
	}
	return 0.0f;
}
