#pragma once

#include <thread>

namespace threads_learning {

// Jak ju¿ wspomniano, programista musi zagwarantowaæ wywo³anie metody join() lub
// detach() przed zniszczeniem obiektu std::thread.

class ThreadGuard {
public:
	explicit ThreadGuard(std::thread& thread) :
		m_thread{ thread }{}
	~ThreadGuard() {
		if (m_thread.joinable()) {
			m_thread.join();
		}
	}

	ThreadGuard(ThreadGuard const&) = delete;
	ThreadGuard& operator=(ThreadGuard const&) = delete;

private:
	std::thread& m_thread;
};

}