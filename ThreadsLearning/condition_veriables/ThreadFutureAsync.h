#pragma once

#include <thread>
#include <future>
#include <iostream>

namespace threads_learning {

/*
	Sluzy do jednorazowych zdazen, a nie czegos cyklicznego.
	Twórcy biblioteki standardowej jêzyka C++ rozwi¹zali problem jednorazowych
	zdarzeñ za pomoc¹ mechanizmu nazwanego przysz³oœci¹ (ang. future). W¹tek, który
	musi czekaæ na okreœlone jednorazowe zdarzenie, powinien uzyskaæ przysz³oœæ reprezentuj¹c¹
	to zdarzenie. W¹tek oczekuj¹cy na tê przysz³oœæ mo¿e nastêpnie okresowo
	sprawdzaæ, czy odpowiednie zdarzenie nie nast¹pi³o (tak jak pasa¿erowie co jakiœ czas
	zerkaj¹ na tablicê odlotów), i jednoczeœnie pomiêdzy tymi testami wykonywaæ inne
	zadanie (spo¿ywaæ drogi deser w lotniskowej kawiarni).
*/

/*
	Istnieja shared_future i unique_future
	Jeœli wiele w¹tków potrzebuje dostêpu do 
	jednego obiektu przysz³oœci, nale¿y chroniæ ten dostêp za pomoc¹ muteksu lub innego
	mechanizmu synchronizacji (patrz rozdzia³ 3.).
*/

/*
	Asynchroniczne zadanie, którego wynik nie jest potrzebny na bie¿¹cym etapie dzia³ania
	programu, mo¿na rozpocz¹æ za pomoc¹ funkcji std::async. Zamiast zwracania
	obiektu klasy std::thread, który umo¿liwi oczekiwanie na zakoñczenie danego w¹tku,
	funkcja std::async zwraca obiekt klasy std::future, który w przysz³oœci bêdzie zawiera³
	wartoœæ wynikow¹. 
	
	W miejscu, w którym aplikacja bêdzie potrzebowa³a tej wartoœci,
	nale¿y wywo³aæ funkcjê get() dla obiektu przysz³oœci — wywo³anie tej funkcji zablokuje
	wykonywanie bie¿¹cego w¹tku do momentu osi¹gniêcia gotowoœci przez przysz³oœæ,
	po czym zwróci uzyskan¹ wartoœæ

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
	mo¿e mieæ albo wartoœæ std::launch::deferred (wówczas wywo³anie funkcji jest odk³adane
	do momentu wywo³ania funkcji wait() lub get() dla danej przysz³oœci), albo wartoœæ
	std::launch::async (wówczas funkcja musi byæ wykonywana w odrêbnym w¹tku), albo
	wartoœæ std::launch::deferred | std::launch::async (wówczas decyzja nale¿y do
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