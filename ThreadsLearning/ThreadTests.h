#pragma once

#include "ThreadArgumentsPassing.h"
#include "mutex/ThreadAccessingToSharedData.h"
#include "mutex/ThreadSharedMutexReadWrite.h"
#include "condition_veriables/ThreadConditionVeriable.h"
#include "condition_veriables/ThreadFutureAsync.h"
#include "condition_veriables/ThreadPackageTask.h"
#include "condition_veriables/ThreadPromise.h"

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
	}

private:
	static constexpr bool Thread_Arguments_Passing_Test = false;
	static constexpr bool Thread_Accessing_To_Shared_Data_Test = false;
	static constexpr bool Thread_Shared_Mutex_Read_Write = false;
	static constexpr bool Thread_Condition_Veriable = false;
	static constexpr bool Thread_Future_Async = false;
	static constexpr bool Thread_Package_Task = false;
	static constexpr bool Thread_Promise_Task = true;
};

}