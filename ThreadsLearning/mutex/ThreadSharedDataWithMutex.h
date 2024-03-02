#pragma once

#include <thread>
#include <mutex>

namespace threads_learning {

/*
	W języku C++ muteks można utworzyć, konstruując obiekt klasy std::mutex. Do
	blokowania muteksu służy funkcja składowa lock(), natomiast do odblokowywania
	muteksu służy wywołanie funkcji unlock().
*/


/*
	Zamiast wspomnianych funkcji można użyć szablonu
	klasy std::lock_guard dostępnego w bibliotece standardowej języka C++. Szablon
	implementuje wzorzec projektowy RAII dla muteksu — blokuje wskazany muteks
	podczas konstruowania obiektu i zwalnia ten muteks w ramach procedury niszczenia
	tego obiektu, co gwarantuje prawidłowe odblokowywanie zablokowanych muteksów
	niezależnie od sytuacji.
	std::lock_guard<std::mutex> guard(some_mutex); ❹
*/

/*
	Jak się dowiesz w sekcji 3.2.4, standard C++17 wprowadził również rozszerzoną wersję
	mechanizmu ochronnego o nazwie std::scoped_lock, więc w środowisku C++17
	zamiast poprzedniego polecenia można użyć następującego:
	std::scoped_guard guard(some_mutex);
*/

/*
	NIE ZWRACAC REFERENCJI/PTR DO DANYCH KTORE SA CHRONONIE MUTEXEM!!!!
	Jak widać, skuteczna ochrona danych przy użyciu
	muteksu wymaga przemyślanego zaprojektowania interfejsu, tak aby muteks był blokowany
	przed każdą próbą dostępu do chronionych danych i aby ten mechanizm gwarantował
	niezawodność niezależnie od okoliczności.

	s 68.
*/


// zeby mutexy dobrze chronily dana zmienna musimy oznaczyc wszystkie miejsca, w ktorych dana zmienna jest uzywana poprzez std::scoped_guard

/*

template<typename T,typename Container=std::deque<T> >
class stack
{
public:
explicit stack(const Container&);
explicit stack(Container&& = Container());
template <class Alloc> explicit stack(const Alloc&);
template <class Alloc> stack(const Container&, const Alloc&);
template <class Alloc> stack(Container&&, const Alloc&);
template <class Alloc> stack(stack&&, const Alloc&);
bool empty() const;
size_t size() const;
T& top();
T const& top() const;
void push(T const&);
void push(T&&);
void pop();
void swap(stack&&);
template <class... Args> void emplace(Args&&... args); Nowość w C++14
};

W tym przypadku problemem jest zawodność wyników zwracanych przez funkcje
empty() i size(). Mimo że w momencie wywołania wyniki mogą być prawidłowe, po
zwróceniu tych wartości pozostałe wątki mają swobodny dostęp do stosu i mogą
umieszczać na nim nowe elementy (za pomocą funkcji push()) lub zdejmować istniejące
elementy (za pomocą funkcji pop()), zanim wątek, który wywołał funkcję empty()
lub size(), wykorzysta uzyskane informacje

W tym przypadku, dostep do samej struktury (w zasadzie do calego obiektu, powinien byc omutexowany.

Mamy więc do czynienia z klasycznym przykładem sytuacji wyścigu, której nie można
zapobiec, stosując wewnętrzny muteks chroniący zawartość stosu — w tym przypadku
sytuacja wyścigu wynika z użytego interfejsu.

Roziwazinem tego problemu jest stworzenie muteksow i zwracanie nie referencji, a std::shared_ptr.
Dodatkowo trzeba tworzyc niepodzielne metody, w taki sposob, aby obie nie wyplwaly na to samo (np. pop i top)

template<typename T>
class threadsafe_stack
{
private:
	std::stack<T> data;
	mutable std::mutex m;
	public:
	threadsafe_stack(){}
	threadsafe_stack(const threadsafe_stack& other)
	{
		std::lock_guard<std::mutex> lock(other.m);
	data=other.data; ❶ Kopia wykonywana w ciele konstruktora
	}
		threadsafe_stack& operator=(const threadsafe_stack&) = delete;
	void push(T new_value)
	{
	std::lock_guard<std::mutex> lock(m);
	data.push(std::move((new_value));
	}
	std::shared_ptr<T> pop()
	{
	std::lock_guard<std::mutex> lock(m);
	if(data.empty()) throw empty_stack();
	std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
	data.pop();
	return res;
	}
	void pop(T& value)
	{
	std::lock_guard<std::mutex> lock(m);
	if(data.empty()) throw empty_stack();
	value=data.top();
	data.pop();
	}
	bool empty() const
	{
	std::lock_guard<std::mutex> lock(m);
	return data.empty();
	}
};


*/

class ThreadSharedDataWithMutex {
public:
	ThreadSharedDataWithMutex(std::string initValue) : 
		m_value{ initValue } {}

	std::string getValue() {
		std::scoped_lock scopedLock{ m_accessingMutex };

		return m_value;
	}

	void setValue(std::string value) {
		std::scoped_lock scopedLock{ m_accessingMutex };
		m_value = value;
	}

	bool compare(std::string value) {
		std::scoped_lock scopedLock{ m_accessingMutex };
		
		return m_value.compare(value) == 0;
	}

private:
	std::string m_value;
	std::mutex m_accessingMutex;
};

}