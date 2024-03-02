#pragma once

#include <iostream>
#include <string>
#include <deque>
#include <vector>
#include <mutex>
#include <future>
#include "../Utils.h"

namespace threads_learning {

/*
	Co nale¿y zrobiæ z zadaniami, których nie mo¿na wyraziæ w formie prostych wywo³añ
	funkcji, i zadaniami, których wyniki mog¹ pochodziæ z wielu ró¿nych miejsc? Obs³uga
	takich przypadków wymaga jeszcze innego sposobu tworzenia przysz³oœci — bezpoœredniego
	ustawiania wartoœci za pomoc¹ szablonu std::promise.
*/


std::string simpleTask(std::string val) {
	std::cout << "[BEFORE] Sleep" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << "[AFTER] Sleep" << std::endl;

	return val;
}


class TasksManager {
public:
	TasksManager() : 
		m_tasks{},
		m_mainMutex{},
		isFinished{} {}

	void mainThread() {
		while (!isFinished) {
			auto future = doSomeThings();
			auto fromFuture = future.get();
			std::cout << "result from future: " << fromFuture << std::endl;
			
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}

	void loopTask() {
		while (!isFinished) {
			if (m_tasks.empty()) {
				continue;
			}

			std::scoped_lock sl(m_mainMutex);

			std::cout << "[BEFORE TASK CALLING]" << std::endl;
			auto& task = m_tasks.front();
			task(utils::getRandomString(5));
			std::cout << "[AFTER TASK CALLING]" << std::endl;
			
			m_tasks.pop_front();
		}
	}

	
	std::future<std::string> doSomeThings() {
		std::packaged_task<std::string(std::string)> package(simpleTask);
		
		auto future = package.get_future();
		std::scoped_lock sl(m_mainMutex);
		// std::Move !!!!
		m_tasks.push_back(std::move(package));
		
		return future;
	}
	
	void stop() {
		std::scoped_lock sl(m_mainMutex);
		isFinished = true;
	}

private:
	std::deque<std::packaged_task<std::string(std::string)>> m_tasks;
	std::mutex m_mainMutex;


	bool isFinished;
};

class ThreadPackageTask {
public:
	void test() {
		TasksManager taskManager{};

		std::thread mainThread(&TasksManager::mainThread, &taskManager);
		std::thread loopThread(&TasksManager::loopTask, &taskManager);

		for (auto i = 0; i < 100; i++) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}

		taskManager.stop();

		mainThread.join();
		loopThread.join();
	}
};

}