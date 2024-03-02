#pragma once

#include <thread>
#include <mutex>

namespace threads_learning {

/*
	Typowym sposobem unikania zakleszczeń jest konsekwentne blokowanie dwóch
	muteksów w tej samej kolejności — jeśli zawsze blokujemy muteks A przed muteksem
	B, nigdy nie dojdzie do zakleszczenia.

	std::scoped_lock guard(lhs.m,rhs.m); ❶
*/

/*
	NALEŻY UNIKAĆ ZAGNIEŻDŻONYCH BLOKAD
	Pierwsza zasada jest najprostsza — należy unikać żądania blokady, jeśli dany wątek dysponuje
	już jakąś blokadą.

	nie blokowac mutexow, jezeli czekamy na jakis blokade

	NALEŻY UNIKAĆ WYKONYWANIA KODU UŻYTKOWNIKA
	W CZASIE UTRZYMYWANIA BLOKADY


	NALEŻY STOSOWAĆ BLOKADY W STAŁEJ KOLEJNOŚCI

	NALEŻY STOSOWAĆ HIERARCHIĘ BLOKAD


	Jak wspomniałem na początku tego punktu, zakleszczenie nie zawsze wynika ze stosowania
blokad — może mieć związek z dowolną inną formą synchronizacji, która w pewnych
okolicznościach tworzy cykle oczekiwania.
*/

class DataHolder {
public:
	DataHolder(std::string initData) : 
		m_data{ initData } {}

	std::string getData() {
		return m_data;
	}

	void setData(std::string data) {
		m_data = data;
	}

private:
	std::string m_data;
};


class DeadLockProvider {
public:
	DeadLockProvider(std::string data1, std::string data2) : 
		m_dataHolder1{ data1 },
		m_dataHolder2{ data2 },
		m_mutex1{},
		m_mutex2{} {}

	void access() {
		// wszystko albo nic, moze blokowac dwa thready na raz.
		// TUTAJ MZEO POELCICE EXCEPTION!!!!
		std::scoped_lock guard{ m_mutex1, m_mutex2 };
		m_dataHolder1.setData("abc");
		m_dataHolder2.setData("abc");

		/*
		    Alternatywa
			std::uinque_lock ma flagę określającą, czy dany muteks aktualnie należy do tego obiektu.
			std::unique_lock<std::mutex> lock_a(lhs.m,std::defer_lock);
			std::unique_lock<std::mutex> lock_b(rhs.m,std::defer_lock);
			std::lock(lock_a,lock_b);
		*/
	}

	/*
	std::unique_lock<std::mutex> get_lock() {
		extern std::mutex some_mutex;
		std::unique_lock<std::mutex> lk(some_mutex);
		prepare_data();
		return lk; ❶
	}

	void process_data() {
		std::unique_lock<std::mutex> lk(get_lock()); ❷
		do_something();
	}
	*/

private:
	DataHolder m_dataHolder1;
	DataHolder m_dataHolder2;

	std::mutex m_mutex1;
	std::mutex m_mutex2;
};


}