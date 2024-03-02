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


Wynik dzia�ania funkcji jest przechowywany w przysz�o�ci lub nast�puje przechwycenie
wyj�tku zg�oszonego przez funkcj� i jego przechowywanie w przysz�o�ci, podobnie jak
ma to miejsce w przypadku std::async. Ponadto w omawianym przyk�adzie mamy u�ycie
funkcji set_value_at_thread_exit i set_exception_at_thread_exit gwarantuj�cych, �e zmienne
thread_local zostan� poprawnie uprz�tni�te, zanim przysz�o�� b�dzie gotowa.
Warto�� zwracana przez wywo�anie then() jest sama w sobie w pe�ni wyposa�on�
przysz�o�ci�. To oznacza mo�liwo�� ��czenia kontynuacji ze sob�.

*/