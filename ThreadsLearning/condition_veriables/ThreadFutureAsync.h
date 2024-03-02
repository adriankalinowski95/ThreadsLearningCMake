#pragma once

#include <thread>
#include <future>
#include <iostream>

namespace threads_learning {

/*
	Sluzy do jednorazowych zdazen, a nie czegos cyklicznego.
	Tw�rcy biblioteki standardowej j�zyka C++ rozwi�zali problem jednorazowych
	zdarze� za pomoc� mechanizmu nazwanego przysz�o�ci� (ang. future). W�tek, kt�ry
	musi czeka� na okre�lone jednorazowe zdarzenie, powinien uzyska� przysz�o�� reprezentuj�c�
	to zdarzenie. W�tek oczekuj�cy na t� przysz�o�� mo�e nast�pnie okresowo
	sprawdza�, czy odpowiednie zdarzenie nie nast�pi�o (tak jak pasa�erowie co jaki� czas
	zerkaj� na tablic� odlot�w), i jednocze�nie pomi�dzy tymi testami wykonywa� inne
	zadanie (spo�ywa� drogi deser w lotniskowej kawiarni).
*/

/*
	Istnieja shared_future i unique_future
	Je�li wiele w�tk�w potrzebuje dost�pu do 
	jednego obiektu przysz�o�ci, nale�y chroni� ten dost�p za pomoc� muteksu lub innego
	mechanizmu synchronizacji (patrz rozdzia� 3.).
*/

/*
	Asynchroniczne zadanie, kt�rego wynik nie jest potrzebny na bie��cym etapie dzia�ania
	programu, mo�na rozpocz�� za pomoc� funkcji std::async. Zamiast zwracania
	obiektu klasy std::thread, kt�ry umo�liwi oczekiwanie na zako�czenie danego w�tku,
	funkcja std::async zwraca obiekt klasy std::future, kt�ry w przysz�o�ci b�dzie zawiera�
	warto�� wynikow�. 
	
	W miejscu, w kt�rym aplikacja b�dzie potrzebowa�a tej warto�ci,
	nale�y wywo�a� funkcj� get() dla obiektu przysz�o�ci � wywo�anie tej funkcji zablokuje
	wykonywanie bie��cego w�tku do momentu osi�gni�cia gotowo�ci przez przysz�o��,
	po czym zwr�ci uzyskan� warto��

*/


/*
	struct Y
	{
		double operator()(double);
	};

	Y y;
	auto f3=std::async(Y(),3.141);
	auto f4=std::async(std::ref(y),2.718);

*/


/*
	Wspomniany parametr typu std::launch
	mo�e mie� albo warto�� std::launch::deferred (w�wczas wywo�anie funkcji jest odk�adane
	do momentu wywo�ania funkcji wait() lub get() dla danej przysz�o�ci), albo warto��
	std::launch::async (w�wczas funkcja musi by� wykonywana w odr�bnym w�tku), albo
	warto�� std::launch::deferred | std::launch::async (w�wczas decyzja nale�y do
	implementacji).
*/

bool isEven(int number) {
	std::cout << "[before] do something" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "[after] do something" << std::endl;

	return number % 2 == 0;
}

class ThreadFutureAsync {
public:
	void test() {
		// wychodzi na to ze od razu to sie odpala i po prostu wynik bedzie w rpzyszlosci
		auto future1 = std::async(std::launch::async, isEven, 3);
		auto future2 = std::async(std::launch::async, isEven, 2);

		for (auto i = 0; i < 20; i++) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		std::cout << "before future get" << std::endl;
		auto result1 = future1.get();
		std::cout << "between" << std::endl;
		auto result2 = future2.get();

		std::cout << "results: " << result1 << " " << result2 << std::endl;
	}
};

}