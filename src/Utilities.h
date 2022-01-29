#pragma once
#include <random>


namespace Utilities
{
	inline float GenerateRandomFloat(float min, float max)
	{
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<float> dis(min, max);
		return dis(mt);
	}

	class Ticker
	{
	public:
		Ticker(std::function<void()> onTick, std::chrono::milliseconds interval) :
			m_OnTick(onTick),
			m_Interval(interval),
			m_Running(false)
		{}

		void Start() {
			if (m_Running) {
				return;
			}
			m_Running = true;
			std::thread tickerThread(&Ticker::RunLoop, this);
			tickerThread.detach();
		}

		void Stop() {
			m_Running = false;
		}

		void UpdateInterval(std::chrono::milliseconds newInterval) {
			m_IntervalMutex.lock();
			m_Interval = newInterval;
			m_IntervalMutex.unlock();
		}

	private:
		void RunLoop() {
			while (m_Running) {
				std::thread runnerThread(m_OnTick);
				runnerThread.detach();

				m_IntervalMutex.lock();
				std::chrono::milliseconds interval = m_Interval;
				m_IntervalMutex.unlock();
				std::this_thread::sleep_for(interval);
			}
		}

		std::function<void()> m_OnTick;
		std::chrono::milliseconds m_Interval;

		std::atomic<bool> m_Running;
		std::mutex m_IntervalMutex;
	};
}
