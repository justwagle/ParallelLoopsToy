//  Copyright (c) 2019 Bibek Wagle
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>
#include <hpx/include/iostreams.hpp>
#include <hpx/include/async.hpp>
#include <hpx/include/serialization.hpp>
#include <hpx/include/parallel_algorithm.hpp>
#include <hpx/include/parallel_executors.hpp>

#include <cstddef>
#include <complex>
#include <string>
#include <vector>


int hpx_main(boost::program_options::variables_map& vm)
{

    std::size_t const size = vm["size"].as<std::size_t>();
    hpx::cout << "Vector Size: " << size << "\n" << hpx::flush;

    std::vector<double> a(size);
    std::vector<double> b(size);
    std::vector<double> c(size);

    hpx::parallel::fill(hpx::parallel::execution::par, a.begin(), a.end(), 3.0);
    hpx::parallel::fill(hpx::parallel::execution::par, b.begin(), b.end(), 2.0);

    auto start = std::chrono::high_resolution_clock::now();
    hpx::parallel::for_loop(hpx::parallel::execution::par, 0, size,
                            [&] (int i) {
                                c[i] = a[i] + b[i];
                            }
    );
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time: " << std::chrono::duration<double>(end-start).count() << std::endl;

    for(int i = 0; i < size; ++i) {
        assert(c[i] == 5.0);
    }

    return hpx::finalize();
}

int main(int argc, char* argv[])
{
    // Configure application-specific options
    boost::program_options::options_description cmdline(
            "Usage: " HPX_APPLICATION_STRING " [options]");

    cmdline.add_options()
            ("size,s",
             boost::program_options::value<std::size_t>()->default_value(100000000),
             "the size of vector");
    // Initialize and run HPX
    return hpx::init(cmdline,argc, argv);
}

