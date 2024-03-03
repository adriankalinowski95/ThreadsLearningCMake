#pragma once

/*
Okazuje się, że właśnie tak działa prezentowane rozwiązanie : jeśli funkcja square_
root wywołana za pośrednictwem funkcji std::async zgłosi jakiś wyjątek, wyjątek
ten zostanie zapisany w obiekcie przyszłości(w miejscu dla wartości wynikowej), przyszłość
przejdzie w stan gotowości, a funkcja get() spowoduje ponowne zgłoszenie zapisanego
wyjątku.


try
{
some_promise.set_value(calculate_value());
}
catch(...)
{
some_promise.set_exception(std::current_exception());
}


144s.

std::experimental::future<void> process_login(
std::string const& username,std::string const& password)
{
	return spawn_async([=](){
			return backend.authenticate_user(username,password);
		}).then([](std::experimental::future<user_id> id){
			return backend.request_current_info(id.get());
		}).then([](std::experimental::future<user_data> info_to_display){
		try{
			update_display(info_to_display.get());
		} catch(std::exception& e){
		display_error(e);
		}
	});
}



Zwróć uwagę na to, jak kontynuacja pobiera std::experimental::feature jako jedyny
parametr, a następnie używa .get() do pobrania znajdującej się w nim wartości. To oznacza,
że wyjątki są propagowane w dół łańcucha, więc wywołanie info_to_display.get()
w ostatniej kontynuacji zgłosi wyjątek, jeśli którakolwiek funkcja w łańcuchu zgłosiła

148s.
Oczekiwanie an wiele przyszlosci 
when all 
std::experimental::future<FinalResult> process_data(std::vector<MyData>& vec)
{
	size_t const chunk_size=cokolwiek;
	std::vector<std::experimental::future<ChunkResult>> results;
	for(auto begin=vec.begin(),end=vec.end();beg!=end;){
		size_t const remaining_size=end-begin;
		size_t const this_chunk_size=std::min(remaining_size,chunk_size);
		results.push_back(
			spawn_async(
			process_chunk,begin,begin+this_chunk_size));
		begin+=this_chunk_size;
	}
	
	return std::experimental::when_all(
		results.begin(),results.end()).then( ❶
		[](std::future<std::vector<
		std::experimental::future<ChunkResult>>> ready_results)
		{
			std::vector<std::experimental::future<ChunkResult>>
			all_results=ready_results .get();
			std::vector<ChunkResult> v;
			v.reserve(all_results.size());
			for(auto& f: all_results)
			{
				v.push_back(f.get()); ❷
			}
			return gather_results(v);
	});
}

Jest tez when_any


*/