#include <iostream>
#include <thread>
#include <string>
#include <random>
#include <chrono>
#include <future>
#include "ThreadGuard.h"
#include "ThreadTests.h"


// 1. Stworzyc projekt w cmake.
// 2. Dodac do niego conana
// 3. Exceptiony w watkach
// 4. RAII w tym wylaczeine watkow
// 5. Przekazywanie obiektow przez referencje (std::ref)
// 6. condition_veriables
// 7. std::future itp.
// 8. jak projektowac(taki dzial, chyba nawet 8)
// 9. Dodac folder na testy!
// 
// w sumie to prosta rzecz -
// thread 1 prosi o odczyt czegos i uzywa tej zmiennej
// thread 2 odczytuje i zapisuje

std::mutex mut;
std::condition_variable data_cond;
bool isToRead{};

std::string value{};

void readValue() {
    isToRead = true;
}

void firstThreadFun() {
    while (1) {
        std::unique_lock<std::mutex> lk(mut);
        readValue();
        auto timeToWait = std::chrono::seconds(3);

        // Jeśli warunek nie jest spełniony (jeśli funkcja lambda zwróciła wartość false), funkcja wait()
        // dblokowuje muteks i wprowadza bieżący wątek w stan blokady (oczekiwania)
        // moze dlatego unique_lock
        auto res = data_cond.wait_for(lk,
            timeToWait,
            [] {
                std::cout << "wait " << std::endl;

                return !value.empty();
            });

        std::cout << "res " << res << std::endl;
        lk.unlock();
        std::cout << value << std::endl;
        value = "";
    }
}

std::string getRandomString(size_t length) {
    const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);
    std::string random_string;

    for (std::size_t i = 0; i < length; ++i){
        random_string += CHARACTERS[distribution(generator)];
    }

    return random_string;
}

void secondThreadFun() {
    while (1) {
        if (isToRead) {
            // w sumie jezeli nie uda sie zablokowac mutexu to czeka dalej.
            std::lock_guard<std::mutex> lk(mut);
            value = getRandomString(5);
            std::cout << "notify for value: " << value << std::endl;
            data_cond.notify_one();
            isToRead = false;

        }
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

int futureValue() {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    return 53;
}

/*

 ten packaed task jest tylko i wylacznie po to zeby stworzyc jakas obudowe na future
 template<typename Func>
 std::future<void> post_task_for_gui_thread(Func f) {
     std::packaged_task<void()> task(f);
     std::future<void> res=task.get_future();
     std::lock_guard<std::mutex> lk(m);
     tasks.push_back(std::move(task));
 return res; 10 }

 */

class PromiseTest {
public:
    std::promise<std::string> m_promise;
    void operator()() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "promise test is settings value" << std::endl;
        m_promise.set_value("abcdef");
    }
};

void t2Fun(std::future<std::string> future) {
    auto result = future.get();
    std::cout << "result: " << result << std::endl;
    std::cout << "After future" << std::endl;
}

int main(int argc, const char* argv[]) {
    auto future = std::async(std::launch::async, futureValue);
    // tutaj mozna ywolywac co sie chce
    // jezeli wywolaym get to wtedy sie zawiesimy i czekamy az watek zwroci wartosc.
    // Wspomniany parametr typu std::launch może mieć albo wartość std::launch::deferred (wówczas wywołanie funkcji jest odkłada- ne do momentu wywołania funkcji wait() lub get() dla danej przyszłości),

    auto result = future.get();

    /*
     struct X
     {
     void foo(int,std::string const&);
         std::string bar(std::string const&);
     };

     X x;
     auto f1=std::async(&X::foo,&x,42,"witaj");
     */

     /*
     std::thread t1{firstThreadFun};
     std::thread t2{secondThreadFun};
     t2.detach();
     t1.join();
     */
     // funkcjonalnosc polegajaca na tym, ze czekamy na zdarzenie. Watek jest zablokowany do momentu, az future zwroci jakas wartosc.

    /*
    PromiseTest promiseTest{};
    auto futureWithPromise = promiseTest.m_promise.get_future();
    
    std::thread t1(std::ref(promiseTest));
    std::thread t2(t2Fun, std::move(futureWithPromise));

    threads_learning::ThreadGuard threadGuard1(t1);
    threads_learning::ThreadGuard threadGuard2(t2);
    */

    threads_learning::ThreadTests tests{};
    tests.start();

    return 0;
}