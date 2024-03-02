#pragma once

#include <thread>
#include <memory>
#include <shared_mutex>
#include <iostream>
#include "../Utils.h"

namespace threads_learning {

class SharedMutexReadWrite {
public:
	SharedMutexReadWrite(std::string val) :
		m_value{ val },
		entry_mutex{} {}

	std::string getValue() {
		// Oznacza to, że funkcja find_entry() może być jednocześnie wywoływana przez wiele
		// wątków.
		std::cout << "before reading" << std::endl;

		std::shared_lock<std::shared_mutex> lk(entry_mutex);
		
		std::cout << "after lock reading" << std::endl;

		return m_value;
	}

	void setValue(std::string value) {
		/*
			Zupełnie inaczej jest w przypadku funkcji update_or_add_entry(), która używa
			obiektu klasy std::lock_guard<> do zapewniania wyłącznego dostępu wątkowi aktualizującemu
			tabelę ❷ — pozostałe wątki nie tylko nie mogą w tym czasie aktualizować
			chronionej struktury za pomocą funkcji update_or_add_entry(), ale też nie mogą wywoływać
			funkcji find_entry().
			

			Na końcu omówiłem alternatywne metody ochrony
			danych w konkretnych scenariuszach, w tym funkcję std::call_once() i klasę std::
			shared_mutex.
		*/

		// Chyba to dziala tak, ze jezeli jest zapisywnaie to zaden watek nei moze nigdzie wejsc gdzie jest ten shared_mutex, ale w normalnym przypadku moga wbijac
		// Cos na zasadzie:
		// mozesz pobierac dane kiedy chcesz,
		// jezeli ktos w danym momencei zapisuje, to nie mozesz odczytac, tylko musiszp oczekac az cie odblokuja.
		{
			std::lock_guard<std::shared_mutex> lk(entry_mutex);
			std::cout << "lock writting" << std::endl;
			m_value = value;

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));

			if (m_value != value) {
				std::cout << "soemthing modified my mutex!" << std::endl;
			}
			else {
				std::cout << "value is the same!" << std::endl;
			}

		}
		std::cout << "after lock writting" << std::endl;
	}

private:
	std::string m_value;
	mutable std::shared_mutex entry_mutex;
};

void readingThread(SharedMutexReadWrite& shared, bool& isFinished) {
	while (!isFinished) {
		auto value = shared.getValue();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void writtingThread(SharedMutexReadWrite& shared, bool& isFinished) {
	while (!isFinished) {
		shared.setValue(utils::getRandomString(5));
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}


class ThreadSharedMutexReadWrite {
public:
	void test() {
		SharedMutexReadWrite shareDataWithMutex{ "abc" };
		bool isFinished{};
		std::vector<std::thread> threads{};

		for (auto i = 0; i < 5; i++) {
			auto th = std::thread(readingThread,std::ref(shareDataWithMutex), std::ref(isFinished));
			threads.push_back(std::move(th));
		}	
		
		for (auto i = 0; i < 2; i++) {
			auto th = std::thread(writtingThread, std::ref(shareDataWithMutex), std::ref(isFinished));
			threads.push_back(std::move(th));
		}

		for (auto i = 0; i < 100; i++) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		isFinished = true;

		for (auto i = 0; i < threads.size(); i++) {
			if (threads[i].joinable()) {
				threads[i].join();
			}
		}
	}
};

}