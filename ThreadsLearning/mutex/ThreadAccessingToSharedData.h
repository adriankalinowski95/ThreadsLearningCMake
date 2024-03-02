#pragma once

#include <iostream>
#include "../Utils.h"
#include "ThreadSharedDataWithMutex.h"

namespace threads_learning {

class WithoutAccessingMutex {
public:
	void accessToData(ThreadSharedDataWithMutex& threadSharedData, bool& isFinished) {
		while(!isFinished) {
			auto result = threadSharedData.getValue();
			if (!threadSharedData.compare(result)) {
				std::cout << "Dane sa rozne pomiedzy wywoalniem getValue a compare" << std::endl;
			} else {
				std::cout << "Dane sa takie same: " << result << std::endl;
			}
	
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
	
	void modifyData(ThreadSharedDataWithMutex& threadSharedData, bool& isFinished) {
		while (!isFinished) {
			threadSharedData.setValue(utils::getRandomString(5));
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
};

class WithAccessingMutex {
public:
	WithAccessingMutex() : 
		m_accessMutex{} {}
	
	void accessToData(ThreadSharedDataWithMutex& threadSharedData, bool& isFinished) {
		while (!isFinished) {
			{
				std::scoped_lock sl{ m_accessMutex };
				// Ten scoped lock laczy zarowne metode do odczytu jaki i porownywanie (niby jest wewnetrzy mutex, ale tez zewnetrzny
				auto result = threadSharedData.getValue();
				if (!threadSharedData.compare(result)) {
					std::cout << "Dane sa rozne pomiedzy wywoalniem getValue a compare" << std::endl;
				} else {
					std::cout << "Dane sa takie same: " << result << std::endl;
				}
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void modifyData(ThreadSharedDataWithMutex& threadSharedData, bool& isFinished) {
		while (!isFinished) { 
			
			/*
				std::adopt_lock przekazywanej za po�rednictwem drugiego argumentu konstruktora, kt�ra powoduje, �e tworzony obiekt zarz�dza blokad�
				muteksu, mo�emy przekaza� warto�� std::defer_lock (za po�rednictwem tego samego argumentu),
				aby okre�li�, �e podczas tworzenia obiektu muteks ma pozosta� odblokowany.

				Blokad� mo�na p�niej uzyska�, 
				wywo�uj�c funkcj� lock() dla obiektu klasy std::unique_lock (nie dla muteksu) lub przekazuj�c obiekt klasy std::unique_lock na
				wej�ciu funkcji std::lock().
			*/

			std::unique_lock ul{ m_accessMutex };

			threadSharedData.setValue(utils::getRandomString(5));

			ul.unlock();

			// najlepiej tutaj dac unlock

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

private:
	std::mutex m_accessMutex;
};

class ThreadAccessingToSharedData {
public:
	void test() {
		std::cout << "----------- without accessing mutex ----------" << std::endl;
		withoutAccessingMutex();

		std::cout << "----------- with accessing mutex ----------" << std::endl;
		withAccessingMutex();
	}

private:
	void withoutAccessingMutex() {
		ThreadSharedDataWithMutex shareDataWithMutex{ "abc" };
		bool isFinished{};

		WithoutAccessingMutex obj{};
		std::thread accessToDataThread(&WithoutAccessingMutex::accessToData,&obj, std::ref(shareDataWithMutex), std::ref(isFinished));
		std::thread modifyDataThread(&WithoutAccessingMutex::modifyData, &obj, std::ref(shareDataWithMutex), std::ref(isFinished));

		for (auto i = 0; i < 100; i++) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		isFinished = true;

		if (accessToDataThread.joinable()) {
			accessToDataThread.join();
		}

		if (modifyDataThread.joinable()) {
			modifyDataThread.join();
		}
	}

	void withAccessingMutex() {
		ThreadSharedDataWithMutex shareDataWithMutex{ "abc" };
		bool isFinished{};

		WithAccessingMutex obj{};
		std::thread accessToDataThread(&WithAccessingMutex::accessToData, &obj, std::ref(shareDataWithMutex), std::ref(isFinished));
		std::thread modifyDataThread(&WithAccessingMutex::modifyData, &obj, std::ref(shareDataWithMutex), std::ref(isFinished));

		for (auto i = 0; i < 100; i++) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		isFinished = true;

		if (accessToDataThread.joinable()) {
			accessToDataThread.join();
		}

		if (modifyDataThread.joinable()) {
			modifyDataThread.join();
		}
	}
};

}