#pragma once

#include <iostream>
#include <thread>
#include <memory>
#include "ThreadSharedData.h"
#include "ThreadGuard.h"

namespace threads_learning {

/*
	Jak widaæ na listingu 2.4, przekazywanie argumentów do wywo³ywalnego obiektu lub
		funkcji jest dziecinnie proste i sprowadza siê do przekazywania dodatkowych argumentów
		na wejœciu konstruktora klasy std::thread.Warto jednak pamiêtaæ, ¿e przekazywane
		w ten sposób argumenty s¹ domyœlnie kopiowane do wewnêtrznej pamiêci, gdzie
		mog¹ byæ przetwarzane przez nowo utworzony w¹tek(nawet jeœli odpowiedni parametr
			funkcji zadeklarowano jako przekazywany przez referencjê).Oto prosty przyk³ad :
	void f(int i, std::string const& s);
	std::thread t(f, 3, ”witaj”);
*/

/*
widget_data data;
std::thread t(update_data_for_widget,w,data);
display_status();
t.join();
process_widget_data(data);
}
Mimo ¿e funkcja update_data_for_widget oczekuje, ¿e drugi parametr zostanie przekazany
przez referencjê, konstruktor klasy std::thread nie dysponuje ¿adn¹ wiedz¹ na
temat argumentów oczekiwanych przez wskazan¹ funkcjê i automatycznie kopiuje
wszystkie przekazane wartoœci.
*/

/*

class X
{
public:
void do_lengthy_work();
};
X my_x;
std::thread t(&X::do_lengthy_work,&my_x);
Powy¿szy kod wywo³a funkcjê my_x.do_lengthy_work() w nowym w¹tku, poniewa¿
adres obiektu my_x zosta³ przekazany w formie wskaŸnika do obiektu. Istnieje te¿
mo¿liwoœæ przekazywania argumentów na wejœciu takiej funkcji sk³adowej — w takim
przypadku trzeci argument konstruktora std::thread zostanie zinterpretowany jako
pierwszy argument tej funkcji sk³adowej.

*/

// Rozwiazanie: std::thread t(update_data_for_widget,w,std::ref(data));

void threadReferenceTest(ThreadSharedData& threadSharedData) {
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "thread shared data value: " << threadSharedData.getValue() << std::endl;
}

void threadSharedModifier(ThreadSharedData& threadSharedData) {
	std::cout << "thread shared modifer" << std::endl;
	threadSharedData.setString("data from shared modifier");
}

/* 
	z shared_ptr 
*/
void threadSharedPtr(std::shared_ptr<ThreadSharedData> threadSharedData) {
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "thread shared data value shared ptr: " << threadSharedData->getValue() << std::endl;
}

void threadSharedModifierSharedPtr(std::shared_ptr<ThreadSharedData> threadSharedData) {
	std::cout << "thread shared modifer" << std::endl;
	threadSharedData->setString("data from shared modifier shared ptr");
}


class ThreadArgumentsPassing {
public:
	void test() {
		ThreadSharedData threadSharedData{"test"};
		
		/*
		// its make a compilatino error
		// we are passing by copy so it won't works
		std::thread notPassingReferenceTestThread(threadReferenceTest, threadSharedData);
		std::thread notPassingSharedModifierThread(threadSharedModifier, threadSharedData);

		notPassingReferenceTestThread.join();
		notPassingSharedModifierThread.join();
		*/

		// passing
		std::thread passingReferenceTestThread(threadReferenceTest, std::ref(threadSharedData));
		std::thread passingSharedModifierThread(threadSharedModifier, std::ref(threadSharedData));
		
		passingReferenceTestThread.join();
		passingSharedModifierThread.join();

		auto sharedDataSharedPtr = std::make_shared<ThreadSharedData>("test");

		std::thread passingSharedPtrTestThread(threadSharedPtr, sharedDataSharedPtr);
		std::thread passingSharedModifierSharedPtrThread(threadSharedModifierSharedPtr, sharedDataSharedPtr);

		passingSharedPtrTestThread.join();
		passingSharedModifierSharedPtrThread.join();
	}
};

}