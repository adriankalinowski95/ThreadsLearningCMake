#pragma once


/*


Operacje na typie std::atomic_flag
Typ std::atomic_flag jest najprostszym standardowym typem atomowym, kt�ry reprezentuje
flag� logiczn�. Obiekty tego typu mog� reprezentowa� jeden z dw�ch stan�w �
mog� by� ustawione lub wyzerowane. Prostota tego typu nie jest przypadkowa � typ
std::atomic_flag w za�o�eniu ma by� u�ywany wy��cznie jako element sk�adowy.



Oznacza to, �e bezpo�rednie stosowanie tego typu jest bardzo rzadkie i ma miejsce
tylko w wyj�tkowych przypadkach. Mimo to warto wykorzysta� ten typ w roli punktu
wyj�cia dla analizy pozosta�ych typ�w atomowych, poniewa� nawet ten prosty typ dobrze
ilustruje og�lne zasady rz�dz�ce typami atomowymi.
Obiekty typu std::atomic_flag musz� by� inicjalizowane z wykorzystaniem warto�ci
ATOMIC_FLAG_INIT. Warto�� ta powoduje, �e nowo inicjalizowana flaga jest wyzerowana.
Programista nie ma wp�ywu na ten aspekt dzia�ania typu � nowa flaga zawsze
jest wyzerowana:
std::atomic_flag f=ATOMIC_FLAG_INIT;
\

class spinlock_mutex
{
std::atomic_flag flag;
public:
spinlock_mutex():
flag(ATOMIC_FLAG_INIT)
{}
void lock()
{
while(flag.test_and_set(std::memory_order_acquire));
}
void unlock()
{
flag.clear(std::memory_order_release);
}
};


Najbardziej podstawowym atomowym typem ca�kowitoliczbowym jest std::atomic<bool>.
Jak nietrudno si� domy�li�, typ std::atomic<bool> jest bardziej rozbudowan� implementacj�
flagi logicznej ni� opisany wcze�niej typ std::atomic_flag. Mimo �e tak�e
ten typ nie oferuje mo�liwo�ci konstruowania poprzez kopiowanie ani przypisywania
z kopiowaniem, mo�emy skonstruowa� obiekt tego typu na podstawie nieatomowej
warto�ci logicznej � oznacza to, �e obiekt typu std::atomic<bool> mo�e mie� pocz�tkowo
warto�� true lub false. Co wi�cej, istnieje te� mo�liwo�� przypisywania egzemplarzom
typu std::atomic<bool> nieatomowych warto�ci logicznych:
std::atomic<bool> b(true);
b=false;

std::atomic<bool> b;
bool x=b.load(std::memory_order_acquire);
b.store(true);
x=b.exchange(false,std::memory_order_acq_rel);

*/