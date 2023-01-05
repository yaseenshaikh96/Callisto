#pragma once

#include <vector>
#include <chrono>

namespace Callisto
{
	template<typename Fn>
	class Timer
	{
	public:
		Timer(const char* name, Fn&& func)
			:
			m_Name(name), 
			m_Stopped(false),
			m_Func(func)
		{
			m_StartingTime = std::chrono::high_resolution_clock::now();
		}
		~Timer()
		{
			if (!m_Stopped)
				Stop();
		}
		void Stop()
		{ 
			auto stopTime = std::chrono::high_resolution_clock::now();
			float duration = (stopTime - m_StartingTime).count() * 0.000001f;
			m_Stopped = true;
			CALLISTO_CORE_TRACE("{0} Timer: took {1}ms", m_Name, duration);
			m_Func({m_Name, duration});
		}
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartingTime;
		bool m_Stopped;
		Fn m_Func;
	};

/*
	class Profiler
	{
		class TimerData
		{
		public:
			TimerData(const Timer& time) :mTimer(time) {}
			Timer mTimer;
			bool Finished = false;
			float Duration;
		};
	public:
		virtual ~Profiler();
		void MakeTime(const std::string& name);
		void OnUpdate();
		void Clear();
		float Stop(const std::string& name);
	private:
		std::vector<TimerData*> m_Timers;
	};
*/
}