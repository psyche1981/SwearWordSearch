#pragma once
#include <chrono>
#include <iostream>


class Timer
{
public:
	Timer();
	~Timer();
	
	void Begin();
	float End();
	void Reset();

	float GetElapsed();

private:
	std::chrono::steady_clock::time_point _start;
	bool _ended = true;
};
