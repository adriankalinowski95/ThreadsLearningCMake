#pragma once


/*


Operacje na typie std::atomic_flag
Typ std::atomic_flag jest najprostszym standardowym typem atomowym, który reprezentuje
flagê logiczn¹. Obiekty tego typu mog¹ reprezentowaæ jeden z dwóch stanów —
mog¹ byæ ustawione lub wyzerowane. Prostota tego typu nie jest przypadkowa — typ
std::atomic_flag w za³o¿eniu ma byæ u¿ywany wy³¹cznie jako element sk³adowy.



Oznacza to, ¿e bezpoœrednie stosowanie tego typu jest bardzo rzadkie i ma miejsce
tylko w wyj¹tkowych przypadkach. Mimo to warto wykorzystaæ ten typ w roli punktu
wyjœcia dla analizy pozosta³ych typów atomowych, poniewa¿ nawet ten prosty typ dobrze
ilustruje ogólne zasady rz¹dz¹ce typami atomowymi.
Obiekty typu std::atomic_flag musz¹ byæ inicjalizowane z wykorzystaniem wartoœci
ATOMIC_FLAG_INIT. Wartoœæ ta powoduje, ¿e nowo inicjalizowana flaga jest wyzerowana.
Programista nie ma wp³ywu na ten aspekt dzia³ania typu — nowa flaga zawsze
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


Najbardziej podstawowym atomowym typem ca³kowitoliczbowym jest std::atomic<bool>.
Jak nietrudno siê domyœliæ, typ std::atomic<bool> jest bardziej rozbudowan¹ implementacj¹
flagi logicznej ni¿ opisany wczeœniej typ std::atomic_flag. Mimo ¿e tak¿e
ten typ nie oferuje mo¿liwoœci konstruowania poprzez kopiowanie ani przypisywania
z kopiowaniem, mo¿emy skonstruowaæ obiekt tego typu na podstawie nieatomowej
wartoœci logicznej — oznacza to, ¿e obiekt typu std::atomic<bool> mo¿e mieæ pocz¹tkowo
wartoœæ true lub false. Co wiêcej, istnieje te¿ mo¿liwoœæ przypisywania egzemplarzom
typu std::atomic<bool> nieatomowych wartoœci logicznych:
std::atomic<bool> b(true);
b=false;

std::atomic<bool> b;
bool x=b.load(std::memory_order_acquire);
b.store(true);
x=b.exchange(false,std::memory_order_acq_rel);

*/