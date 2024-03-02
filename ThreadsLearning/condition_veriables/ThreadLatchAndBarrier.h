#pragma once

/*

Przede wszystkim trzeba sobie odpowiedzie� na pytanie, co oznacza zasuwa lub bariera.
Zasuwa to obiekt synchronizacji, kt�ry staje si� gotowy, gdy warto�� dekrementowanego
licznika spadnie do zera. Nazwa wzi�a si� st�d, �e obiekt zasuwa dane wyj�ciowe, a gdy
jest gotowy, pozostaje w takiej postaci a� do momentu, gdy zostanie usuni�ty. Zasuwa
jest wi�c lekkim mechanizmem pozwalaj�cym oczekiwa� na seri� zdarze�.
Z kolei bariera to komponent synchronizacji wielokrotnego u�ycia, wykorzystywany
do wewn�trznej synchronizacji mi�dzy zbiorem w�tk�w. Podczas gdy dla zasuwy
nie ma znaczenia, dla kt�rego w�tku warto�� licznika spad�a do zera � ten sam w�tek
mo�e dekrementowa� licznik wielokrotnie, wiele w�tk�w mo�e raz dekrementowa�
licznik lub mo�emy mie� do czynienia z po��czeniem tych sytuacji � w przypadku
bariery ka�dy w�tek mo�e w trakcie cyklu tylko raz dotrze� do bariery. Gdy dwa w�tki
dotr� do bariery, pozostan� zablokowane a� do chwili, gdy wszystkie w�tki dotr� do
bariery i dopiero w�wczas nast�pi zwolnienie wszystkich w�tk�w. Nast�pnie bariery
mo�na u�y� ponownie � w�tki zn�w mog� dotrze� do bariery i czeka� na wszystkie pozosta�e
w�tki do nast�pnego cyklu.
Zasuwa jest znacznie prostsza ni� bariera, wi�c zaczn� od przedstawienia typu zasuwy
pochodz�cego ze specyfikacji technicznej Concurrency TS � std::experimental::latch.

*/