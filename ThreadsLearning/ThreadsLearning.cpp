// ThreadsLearning.cpp : Defines the entry point for the application.
//

// 1. Stworzyc projekt w cmake.
// 2?. Dodac do niego conana
// 3. Exceptiony w watkach
// 4. RAII w tym wylaczeine watkow
// 5. Przekazywanie obiektow przez referencje (std::ref)
// 6. condition_veriables
// 7. std::future itp.
// 8. jak projektowac(taki dzial, chyba nawet 8)

#include "ThreadsLearning.h"
#include <boost/range/algorithm.hpp>

using namespace std;

#include <ranges>
#include <string>
#include <vector>
#include <algorithm>

int main() {
	std::vector<std::string> in{"a","b","C"};
	std::vector<std::string> out{};

	std::ranges::copy(std::ranges::transform_view(in, [](std::string val) {
		return val + std::string("xd");
	}), std::back_inserter(out));
	
	std::ranges::for_each(out, [](std::string v) {
		std::cout << v << std::endl;
	});

	boost::range::transform(in, std::back_inserter(out), [](std::string val) {
		return val + std::string("xd");
	});

	std::ranges::for_each(out, [](std::string v) {
		std::cout << v << std::endl;
	});

	return 0;
}
