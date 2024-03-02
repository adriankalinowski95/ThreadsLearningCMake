/*
template<typename Func>
std::experimental::future<decltype(std::declval<Func>()())> spawn_async(Func&& func){
	std::experimental::promise< decltype(std::declval<Func>()())> p;
	auto res=p.get_future();
	std::thread t(
		[p=std::move(p),f=std::decay_t<Func>(func)]() mutable{
		try{
			p.set_value_at_thread_exit(f());
		} catch(...){
			p.set_exception_at_thread_exit(std::current_exception());
		} });
	t.detach();

	return res;
}


Wynik dzia³ania funkcji jest przechowywany w przysz³oœci lub nastêpuje przechwycenie
wyj¹tku zg³oszonego przez funkcjê i jego przechowywanie w przysz³oœci, podobnie jak
ma to miejsce w przypadku std::async. Ponadto w omawianym przyk³adzie mamy u¿ycie
funkcji set_value_at_thread_exit i set_exception_at_thread_exit gwarantuj¹cych, ¿e zmienne
thread_local zostan¹ poprawnie uprz¹tniête, zanim przysz³oœæ bêdzie gotowa.
Wartoœæ zwracana przez wywo³anie then() jest sama w sobie w pe³ni wyposa¿on¹
przysz³oœci¹. To oznacza mo¿liwoœæ ³¹czenia kontynuacji ze sob¹.

*/