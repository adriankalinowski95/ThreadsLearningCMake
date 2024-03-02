#pragma once

#include <thread>
#include <future>
#include <iostream>

namespace threads_learning {

/*
	momencie ustawienia wartości obiektu obietnicy
	(za pomocą funkcji składowej set_value()) obiekt przyszłości przechodzi w stan gotowości
	i jako taki może zostać użyty do pobrania zapisanej wartości. Jeśli nastąpi zniszczenie
	obiektu klasy std::promise bez wcześniejszego ustawienia wartości, zamiast
	oczekiwanej wartości zostanie ustawiony stosowny wyjątek. Sposób przekazywania wyjątków
	pomiędzy wątkami zostanie opisany w punkcie 4.2.4.

	Czyli to dziala podobnie jak temten shit, ale moment ustawienia wartosci w future jest po prostu 
	przez set value...



	#include <future>
	void process_connections(connection_set& connections)
	{
		while(!done(connections)) ❶
		{
			for(connection_iterator ❷
			connection=connections.begin(),end=connections.end();
			connection!=end;
			++connection)
		{
		if(connection->has_incoming_data()) ❸
		{
			data_packet data=connection->incoming();
			std::promise<payload_type>& p=
			connection->get_promise(data.id); ❹
			p.set_value(data.payload);

		if(connection->has_outgoing_data()) ❺
			{
			outgoing_packet data=
			connection->top_of_outgoing_queue();
			connection->send(data.payload);
			data.promise.set_value(true); ❻
			}
		}
	}
*/

/*
	TO CHYBA SIE ROBI, GDY MAMY JAKSI DEQUEUE PROMISOW I W JEDNYM WATKU 
	dziala to tak:
	1. Przychodzi request z zewnatrz
	2. Wrzucamy taki promise na jakas tam kolejke np. w parze std::pair<std::promise*, std::string> 
	3. Jezeli jaksi loop wykryje ze taki sie pojawil to powinien wykonac funkcje np. dla id w std::string
	4. Jezeli mu sie uda to ustawia set value
	5. W miedzy czasie ten watek ktory stworzyl promise pobral z niego feature i czeka na wynik.
	6. Jezeli zwroci wynik to wetdy kontynuje wykonanie.
	7. Mozna to fajnie zastosowac w moich watkach ;)
*/

class TasksManagerForPromise {
public:
	TasksManagerForPromise() :
		m_stringPromise{},
		m_mainMutex{},
		isFinished{} {}

	void mainThread() {
		while (!isFinished) {
			// niby przychodzi ajkeis zlecenie
			if (true){
				// to jest chujwo zrobione, bo mam dostep do promise, a ona nie dziala tak jak powwinno
				auto& myPromise = getPromise();

				auto future = myPromise.get_future();

				if (future._Is_ready()) {
					std::cout << "[BEFORE] is ready " << std::endl;
				}
				else {
					std::cout << "[BEFORE] is not ready " << std::endl;
				}
				// std::shared_future.
				auto result = future.wait_for(std::chrono::milliseconds(1000));
				if (future._Is_ready()) {
					std::cout << "[AFTER] is ready " << std::endl;
				}
				else {
					std::cout << "[AFTER] is not ready " << std::endl;
				}

				if (result == std::future_status::ready) {
					std::cout << "future ready! value: " << future.get() << std::endl;
				} else if (result == std::future_status::timeout) {
					std::cout << "future timeout! " << std::endl;
				} else{
					std::cout << "future someting other! " << std::endl;
				}

				// chyba promisy sa jednorazowe, wiec trzeba od razu tworzyc nowy.
				// on jest jeszcze rozkmina z tym zwracaniem tych promisow, bo w zasadzie nie wiem do konca jak to dziala ;x
				m_stringPromise = std::promise<std::string>();
			}

			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}

	void loopTask() {
		while (!isFinished) {
			std::unique_lock ul(m_mainMutex);

			std::this_thread::sleep_for(std::chrono::seconds(2));

			std::cout << "[BEFORE SET VALUE]" << std::endl;
			m_stringPromise.set_value(utils::getRandomString(5));
			std::cout << "[AFTER SET VALUE]" << std::endl;

			ul.unlock();


			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
	
	std::promise<std::string>& getPromise() {
		// normalnie zwrocic mozna tylko poprzez std::move
		return m_stringPromise;
	}

	void stop() {
		std::scoped_lock sl(m_mainMutex);
		isFinished = true;
	}

private:
	std::promise<std::string> m_stringPromise;
	std::mutex m_mainMutex;

	bool isFinished;
};

class ThreadPromise {
public:
	void test() {
		TasksManagerForPromise taskManager{};

		std::thread mainThread(&TasksManagerForPromise::mainThread, &taskManager);
		std::thread loopThread(&TasksManagerForPromise::loopTask, &taskManager);

		for (auto i = 0; i < 100; i++) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}

		taskManager.stop();

		mainThread.join();
		loopThread.join();
	}
};

}