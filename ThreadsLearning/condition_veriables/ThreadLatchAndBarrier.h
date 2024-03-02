#pragma once

/*

Przede wszystkim trzeba sobie odpowiedzieæ na pytanie, co oznacza zasuwa lub bariera.
Zasuwa to obiekt synchronizacji, który staje siê gotowy, gdy wartoœæ dekrementowanego
licznika spadnie do zera. Nazwa wziê³a siê st¹d, ¿e obiekt zasuwa dane wyjœciowe, a gdy
jest gotowy, pozostaje w takiej postaci a¿ do momentu, gdy zostanie usuniêty. Zasuwa
jest wiêc lekkim mechanizmem pozwalaj¹cym oczekiwaæ na seriê zdarzeñ.
Z kolei bariera to komponent synchronizacji wielokrotnego u¿ycia, wykorzystywany
do wewnêtrznej synchronizacji miêdzy zbiorem w¹tków. Podczas gdy dla zasuwy
nie ma znaczenia, dla którego w¹tku wartoœæ licznika spad³a do zera — ten sam w¹tek
mo¿e dekrementowaæ licznik wielokrotnie, wiele w¹tków mo¿e raz dekrementowaæ
licznik lub mo¿emy mieæ do czynienia z po³¹czeniem tych sytuacji — w przypadku
bariery ka¿dy w¹tek mo¿e w trakcie cyklu tylko raz dotrzeæ do bariery. Gdy dwa w¹tki
dotr¹ do bariery, pozostan¹ zablokowane a¿ do chwili, gdy wszystkie w¹tki dotr¹ do
bariery i dopiero wówczas nast¹pi zwolnienie wszystkich w¹tków. Nastêpnie bariery
mo¿na u¿yæ ponownie — w¹tki znów mog¹ dotrzeæ do bariery i czekaæ na wszystkie pozosta³e
w¹tki do nastêpnego cyklu.
Zasuwa jest znacznie prostsza ni¿ bariera, wiêc zacznê od przedstawienia typu zasuwy
pochodz¹cego ze specyfikacji technicznej Concurrency TS — std::experimental::latch.

*/