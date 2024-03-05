#pragma once

#include <mutex>
#include <string>
#include <condition_variable>
#include <iostream>
#include "../Utils.h"

namespace threads_learning {

/*
	Implementacja funkcji wait() sprawdza warunek (wywo�uj�c przekazan� funkcj�lambda), 
	po czym zwraca sterowanie, je�li ten warunek jest spe�niony (je�li funkcja
	lambda zwr�ci�a warto�� true). Je�li warunek nie jest spe�niony (je�li funkcja lambda
	zwr�ci�a warto�� false), funkcja wait() odblokowuje muteks i wprowadza bie��cy w�tek
	w stan blokady (oczekiwania). Kiedy zmienna warunkowa jest powiadamiana za pomoc�
	funkcji notify_one() wywo�anej przez w�tek przygotowuj�cy dane, w�tek oczekuj�cy jest
	budzony (odblokowywany), ponownie uzyskuje blokad� muteksu i jeszcze raz sprawdza
	warunek.
*/

/*

before wait for
wait_for
new value: JpaDP4w7gH
after notify
wait_for
correct result!
data after modification: JpaDP4w7gH
before wait for
wait_for
// czeka 3  po czym wychodzi bo tamten ma timeout na 4 sekundy
wait_for
timeout!
data after modification: JpaDP4w7gH
before wait for
wait_for
new value: PvpIRIPjLG
after notify
wait_for
correct result!
data after modification: PvpIRIPjLG
before wait for
wait_for

*/

/*
	Implementacja funkcji wait() sprawdza warunek (wywo�uj�c przekazan� funkcj�
	lambda), po czym zwraca sterowanie, je�li ten warunek jest spe�niony (je�li funkcja
	lambda zwr�ci�a warto�� true). Je�li warunek nie jest spe�niony (je�li funkcja lambda
	zwr�ci�a warto�� false), funkcja wait() odblokowuje muteks i wprowadza bie��cy w�tek
	w stan blokady (oczekiwania). Kiedy zmienna warunkowa jest powiadamiana za pomoc�
	funkcji notify_one() wywo�anej przez w�tek przygotowuj�cy dane, w�tek oczekuj�cy jest
	budzony (odblokowywany), ponownie uzyskuje blokad� muteksu i jeszcze raz sprawdza
	warunek. Je�li warunek dalszego przetwarzania jest spe�niony, funkcja wait() zwraca
	sterowanie z zachowaniem blokady muteksu. Je�li warunek nie jest spe�niony, w�tek
	odblokowuje muteks i ponownie przechodzi w stan oczekiwania.
*/

/*

	Je�li w�tek wywo�uj�cy w za�o�eniu ma oczekiwa� na dane zdarzenie tylko raz, czyli
	je�li po spe�nieniu warunku w�tek nie b�dzie ponownie czeka� na t� sam� zmienn�
	warunkow�, by� mo�e warto zastosowa� inny mechanizm synchronizacji ni� zmienna
	warunkowa. Zmienne warunkowe s� szczeg�lnie nieefektywne w sytuacji, gdy warunkiem,
	na kt�ry oczekuj� w�tki, jest dost�pno�� okre�lonego elementu danych. W takim
	przypadku lepszym rozwi�zaniem jest u�ycie mechanizmu przysz�o�ci. @!!!!!!

*/

class ThreadsWithWaiting {
public:
	ThreadsWithWaiting(std::string sharedData) :
		m_mutex{},
		m_cond{},
		m_sharedValue{ sharedData },
		isFinished{} {}

	void modifier() {
		while (!isFinished) {
			std::unique_lock scopedLock{ m_mutex };
			m_sharedValue = utils::getRandomString(10);
			std::cout << "new value: " << m_sharedValue << std::endl;
			// zalezy, mozliwe ze notify_all zwraca sterowanie do wszystkich, a one tylko do jednego watku - 
			m_cond.notify_all();
			std::cout << "after notify" << std::endl;
			scopedLock.unlock();

			std::this_thread::sleep_for(std::chrono::seconds(4));
		}
	}

	void reader() {
		while (!isFinished) {
			std::unique_lock uniqueLock{ m_mutex };
			std::string valueBefore = m_sharedValue;
			// Je�li warunek nie jest spe�niony (je�li funkcja lambda zwr�ci�a warto�� false), funkcja wait()
			// odblokowuje muteks i wprowadza bie��cy w�tek w stan blokady (oczekiwania)
			// moze dlatego unique_lock
			// result jest wynikiem tego, czy zakonczony przez czas, czy zakonczone przez notify_all, a potem poprawne zwrocenie przypadku

			// Dziala to tak:
			// 1. Sprawdza wynik
			// 2.1 Jezeli jest true, to po prostu przechodzi dalej.
			// 2.2 Jezeli jest false, to odblokowuje mutex i czeka na ponowne notify
			// 3. Jezeli medzie notify, to sprawdza warunek i przechodzi do punktu 2.
			// 4. Jezeli minie czas to po prostu znowu wchodzi chyba, ale tym razem z 

			// !WARUNEK JEST SPRAWDZANY OD RAZU, A JEZELI ZWROCI FALSE TO CZEKA
			std::cout << "before wait for " << std::endl;
			auto result = m_cond.wait_for(uniqueLock, std::chrono::seconds(3), [valueBefore, this]() {
				std::cout << "wait_for" << std::endl;
				return valueBefore.compare(m_sharedValue) != 0;
			});

			if (!result) {
				std::cout << "timeout!" << std::endl;
			}
			else {
				std::cout << "correct result!" << std::endl;
			}

			std::cout << "data after modification: " << m_sharedValue << std::endl;
			uniqueLock.unlock();

			// pozwole innemu watki zajac sie robota, zeby od razu nie zablokowac mutexu
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}

	void finishThreads() {
		isFinished = true;
	}

private:
	// dzieki mutable mozemy blokowan mutex w funkcjach z const
	mutable std::mutex m_mutex;
	std::condition_variable m_cond;

	std::string m_sharedValue;
	bool isFinished;
};

class ThreadConditionVeriable {
public:
	void test() {
		ThreadsWithWaiting threadsWithWaitingObj{"test"};

		std::thread t1(&ThreadsWithWaiting::modifier, &threadsWithWaitingObj);
		std::thread t2(&ThreadsWithWaiting::reader, &threadsWithWaitingObj);

		for (auto i = 0; i < 100; i++) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		t1.join();
		t2.join();
	}
};

}