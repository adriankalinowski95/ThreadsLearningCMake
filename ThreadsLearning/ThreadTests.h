#pragma once

#include "ThreadArgumentsPassing.h"
#include "mutex/ThreadAccessingToSharedData.h"
#include "mutex/ThreadSharedMutexReadWrite.h"
#include "condition_veriables/ThreadConditionVeriable.h"
#include "condition_veriables/ThreadFutureAsync.h"
#include "condition_veriables/ThreadPackageTask.h"
#include "condition_veriables/ThreadPromise.h"
#include "exercises/PrintInOrder.h"
#include "exercises/ZeroEvenOdd.h"

namespace threads_learning {

class ThreadTests {
public:
	void start() {
		if (Thread_Arguments_Passing_Test){
			ThreadArgumentsPassing threadArgumentsPassing{};
			threadArgumentsPassing.test();
		}

		if (Thread_Accessing_To_Shared_Data_Test){
			ThreadAccessingToSharedData threadAccessingToSharedData{};
			threadAccessingToSharedData.test();
		}

		if (Thread_Shared_Mutex_Read_Write) {
			ThreadSharedMutexReadWrite threadSharedMutexReadWrite{};
			threadSharedMutexReadWrite.test();
		}

		if (Thread_Condition_Veriable) {
			ThreadConditionVeriable threadConditionVeriable{};
			threadConditionVeriable.test();
		}

		if (Thread_Future_Async) {
			ThreadFutureAsync threadFutureAsync{};
			threadFutureAsync.test();
		}

		if (Thread_Package_Task) {
			ThreadPackageTask threadPackageTask{};
			threadPackageTask.test();
		}

		if (Thread_Promise_Task) {
			ThreadPromise threadPromise{};
			threadPromise.test();
		}

		if (false) {
			auto printFirst = []() {

			};

			PrintInOrder printInOrder{};

			std::thread first(&PrintInOrder::first, &printInOrder, []() {
				std::cout << "first" << std::endl;
			});

			std::thread second(&PrintInOrder::second, &printInOrder, []() {
				std::cout << "second" << std::endl;
			});

			std::thread third(&PrintInOrder::third, &printInOrder, []() {
				std::cout << "third" << std::endl;
			});

			first.join();
			second.join();
			third.join();
		}

		if (false) {
			ZeroEvenOdd zeroEvenOdd{1};

			std::thread t1([](ZeroEvenOdd& arg) {
				while (!arg.finished) {
					auto time = utils::getRandomNumber() % 3;
					arg.zero([](int number) {
						std::cout << std::to_string(number) << std::endl;
					});
					std::this_thread::sleep_for(std::chrono::milliseconds(500 * (1 + time)));
				}
				std::cout << "time end" << std::endl;
			}, std::ref(zeroEvenOdd));
			
			std::thread t2([](ZeroEvenOdd& arg) {
				while (!arg.finished) {
					auto time = utils::getRandomNumber() % 3;
					arg.even([](int number) {
						std::cout << std::to_string(number) << std::endl;
					});
					std::this_thread::sleep_for(std::chrono::milliseconds(500 * (1 + time)));
				}

				std::cout << "even end" << std::endl;
			}, std::ref(zeroEvenOdd));


			std::thread t3([](ZeroEvenOdd& arg) {
				while (!arg.finished) {
					auto time = utils::getRandomNumber() % 3;
					arg.odd([](int number) {
						std::cout << std::to_string(number) << std::endl;
					});
					std::this_thread::sleep_for(std::chrono::milliseconds(500 * (1 + time)));
				}

				std::cout << "odd end" << std::endl;
			}, std::ref(zeroEvenOdd));

			t1.join();
			t2.join();
			t3.join();
		}
	}

private:
	static constexpr bool Thread_Arguments_Passing_Test = false;
	static constexpr bool Thread_Accessing_To_Shared_Data_Test = false;
	static constexpr bool Thread_Shared_Mutex_Read_Write = false;
	static constexpr bool Thread_Condition_Veriable = false;
	static constexpr bool Thread_Future_Async = false;
	static constexpr bool Thread_Package_Task = true;
	static constexpr bool Thread_Promise_Task = false;
};

}