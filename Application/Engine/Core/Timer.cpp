#include "pch.h"
#include "Timer.h"

namespace sn {
	void Timer::Reset()
	{
		m_timePoint = clock::now();
	}

	Timer::clock::rep Timer::GetElapsedTicks()
	{
		clock_duration duration = (clock::now() - m_timePoint);

		return duration.count();
	}

	Timer::clock::rep Timer::TicksPerSecond()
	{
		return clock_duration::period::den;
	}

	double Timer::GetElapsedSeconds()
	{
		return GetElapsedTicks() / static_cast<double>(TicksPerSecond());
	}
	void FrameTimer::Tick()
	{
		m_dt = static_cast<float>(GetElapsedSeconds());
		m_timePoint = clock::now();
	}
}
