# A basic library for benchmark algorithms and data structures written in cpp

## Example of usage

```cpp
#include <cassert>
#include <random>
#include "bench-lib/benchmark.hpp"

long long f1(std::vector<int> a) {
    long long sum = 0;
    for(auto &e: a)
        sum += e;
    return sum;
}

long long f2(std::vector<int> &a) {
    long long sum = 0;
    for(auto &e: a)
        sum += e;
    return sum;
}

int main() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(1, 1'000);
	std::string csv_name = "example_res";

	std::vector<int> sizes = {1'000, 5'000, 10'000, 1'000'000, 10'000'000, 20'000'000, 40'000'000};
	for(size_t i = 0; i < sizes.size(); ++i) {
		int sz = sizes[i];
		std::vector<int> a(sz);
		for(auto &e: a)
			e = distrib(gen);

		BenchLib::Benchmark bench;
		bench.add("sum_lambda_copy", [a]()  {
				long long sum = 0;
				for(auto &e: a)
						sum += e;
				return sum;
		}).set_input_size(sz)
			.set_label("slc").set_size_in_megabytes((4 * sz)/1'000'000);
		
		bench.add("sum_lambda_ref", [&a]() {
			long long sum = 0;
			for(auto &e: a)
					sum += e;
			return sum;
		}).set_input_size(sz)
			.set_label("slr")
			.set_size_in_megabytes((4 * sz)/1'000'000);

		bench.add("sum_copy", f1, a)
			.set_input_size(sz)
			.set_label("slr")
			.set_size_in_megabytes((4 * sz)/1'000'000);

		bench.add("sum_ref", f2, a).set_input_size(sz)
			.set_label("slr")
			.set_size_in_megabytes((4 * sz)/1'000'000);
		
		bench.run();

		long long r1 = bench.get_result<long long>(0);
		long long r2 = bench.get_result<long long>(1);
		long long r3 = bench.get_result<long long>(2);
		long long r4 = bench.get_result<long long>(3);

		assert(r1 == r2);
		assert(r2 == r3);
		assert(r3 == r4);

		if(i == 0) bench.write_csv(csv_name);
		else bench.append_csv(csv_name);
	}
	return 0;
}
```