#pragma once

#include <condition_variable>
#include <functional>

namespace threads_learning {

using namespace std;

class PrintInOrder {
public:
    PrintInOrder() :
        m_mutex{},
        m_cond{},
        m_wasFirst{},
        m_wasSecond{} {}

    void first(function<void()> printFirst) {
        // printFirst() outputs "first". Do not change or remove this line.
        printFirst();

        m_wasFirst = true;
        m_cond.notify_all();
    }

    void second(function<void()> printSecond) {
        unique_lock ul{ m_mutex };
        m_cond.wait(ul, [this]() {
            return wasFirst();
            });

        // printSecond() outputs "second". Do not change or remove this line.
        printSecond();

        m_wasSecond = true;
        m_cond.notify_all();
    }

    void third(function<void()> printThird) {
        // ogolnie to odblokowuje mutex jezeli sie nie uda, wiec teoretycznie to jest dobre - watek jets zawiszony a mutex odblokwoany
        // dlatego mozna zrboic tylko jeden mutex
        unique_lock ul{ m_mutex };
        m_cond.wait(ul, [this]() {
            return wasSecond();
            });
        // printThird() outputs "third". Do not change or remove this line.
        printThird();
    }

    bool wasFirst() {
        return m_wasFirst;
    }

    bool wasSecond() {
        return m_wasSecond;
    }

private:
    mutable mutex m_mutex;
    condition_variable m_cond;

    atomic<bool> m_wasFirst;
    atomic<bool> m_wasSecond;
};

}