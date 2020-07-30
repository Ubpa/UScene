#include <USTL/tuple.h>

#include <iostream>

int main() {
	constexpr auto acc = Ubpa::USTL::tuple_accumulate<true, false, true>(
		std::tuple{ 1,2,3 },
		6,
		[](auto&& acc, auto i) {
			return acc + i;
		}
	);
	static_assert(acc == 10);

	Ubpa::USTL::tuple_for_each<true, false, true>(
		std::tuple{ 1,2,3 },
		[](auto i) {
			std::cout << i << std::endl;
		}
	);

	constexpr auto idx = Ubpa::USTL::tuple_find_if(
		std::tuple{ 1,2,3 },
		[](auto i) {
			return i % 2 == 0;
		}
	);
	static_assert(idx == 1);

	constexpr auto idx2 = Ubpa::USTL::tuple_find(
		std::tuple{ 1,2,3 },
		3
	);
	static_assert(idx2 == 2);

	constexpr auto cnt = Ubpa::USTL::tuple_count_if(
		std::tuple{ 1,2,3 },
		[](auto e) {
			return e >= 2;
		}
	);
	static_assert(cnt == 2);

	constexpr auto cnt2 = Ubpa::USTL::tuple_count(
		std::tuple{ 1,2,3 },
		3
	);
	static_assert(cnt2 == 1);

	constexpr auto appendedTuple = Ubpa::USTL::tuple_append(std::tuple{ 1,2,3 }, 4, 5);
	static_assert(std::get<3>(appendedTuple) == 4);
	static_assert(std::get<4>(appendedTuple) == 5);

	constexpr auto prependedTuple = Ubpa::USTL::tuple_prepend(std::tuple{ 1,2,3 }, -1, 0);
	static_assert(std::get<0>(prependedTuple) == -1);
	static_assert(std::get<1>(prependedTuple) == 0);
}
