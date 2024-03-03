#include <condition_variable>
#include <functional>

namespace threads_learning {

using namespace std;

class ZeroEvenOdd {
private:
    int m_n;

public:
    ZeroEvenOdd(int n) :
        m_n(n),
        m_mutex(),
        m_cond(),
        m_nextNumber(1),
        m_wasZero{} {}

    // printNumber(x) outputs "x", where x is an integer.
    void zero(function<void(int)> printNumber) {
        unique_lock ul{ m_mutex };
        m_cond.wait(ul, [this]() {
            return (m_wasZero == false && m_nextNumber <= m_n) || finished;
        });

        if (m_nextNumber <= m_n) {
            printNumber(0);

            m_wasZero = true;
            m_cond.notify_all();

            if (m_nextNumber > m_n) {
                finished = true;
            }
        }
    }

    void even(function<void(int)> printNumber) {
        unique_lock ul{ m_mutex };
        m_cond.wait(ul, [this]() {
            return (m_wasZero == true && m_nextNumber % 2 == 0 && m_nextNumber <= m_n) || finished;
        });

        if (m_nextNumber <= m_n) {
            printNumber(m_nextNumber);
            m_nextNumber++;
            m_wasZero = false;
            m_cond.notify_all();

            if (m_nextNumber > m_n) {
                finished = true;
            }
        }
    }

    void odd(function<void(int)> printNumber) {
        unique_lock ul{ m_mutex };
        m_cond.wait(ul, [this]() {
            return (m_wasZero == true && m_nextNumber % 2 == 1 && m_nextNumber <= m_n) || finished;
        });

        if (m_nextNumber <= m_n) {
            printNumber(m_nextNumber);
            m_nextNumber++;
            m_wasZero = false;
            m_cond.notify_all();

            if (m_nextNumber > m_n) {
                finished = true;
            }
        }
    }

bool finished = false;

public:
    mutex m_mutex;
    condition_variable m_cond;
    atomic<int> m_nextNumber;
    bool m_wasZero;

};

}