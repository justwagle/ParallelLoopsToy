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
#include <hpx/include/parallel_executors.hpp>
#include <hpx/include/parallel_executor_parameters.hpp>

#include <cstddef>
#include <complex>
#include <string>
#include <vector>
#include <chrono>


int hpx_main(boost::program_options::variables_map &vm) {

    std::size_t const size = vm["size"].as<std::size_t>();
    std::string const policy = vm["policy"].as<std::string>();
    std::size_t const dcs_v = vm["dcs_val"].as<std::size_t>();
    std::size_t const acs_v = vm["acs_val"].as<std::size_t>();
    std::size_t const gcs_v = vm["gcs_val"].as<std::size_t>();
    std::size_t const scs_v = vm["scs_val"].as<std::size_t>();

    hpx::cout << "Vector Size: " << size << "\n" << hpx::flush;
    hpx::cout << "Chunking Policy: " << policy << "\n" << hpx::flush;
    hpx::cout << "Auto Chunking Policy Param: " << acs_v << "\n" << hpx::flush;
    hpx::cout << "Dynamic Chunking Policy Param: " << dcs_v << "\n" << hpx::flush;
    hpx::cout << "Guided Chunking Policy Param: " << gcs_v << "\n" << hpx::flush;
    hpx::cout << "Static Chunking Policy Param: " << scs_v << "\n" << hpx::flush;

    std::vector<double> a(size);
    std::vector<double> b(size);
    std::vector<double> c(size);

    hpx::parallel::fill(hpx::parallel::execution::par, a.begin(), a.end(), 3.0);
    hpx::parallel::fill(hpx::parallel::execution::par, b.begin(), b.end(), 2.0);

    if (policy == "scs") {
        hpx::parallel::execution::static_chunk_size cs(scs_v);
        auto exec_policy = hpx::parallel::execution::par.with(cs);

        auto start = std::chrono::high_resolution_clock::now();
        hpx::parallel::for_loop(exec_policy, 0, size,
                                [&](int i) {
                                    c[i] = a[i] + b[i];
                                }
        );
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Time: " << std::chrono::duration<double>(end - start).count()
                  << std::endl;
    }
    if (policy == "gcs") {
        hpx::parallel::execution::guided_chunk_size cs(gcs_v);
        auto exec_policy = hpx::parallel::execution::par.with(cs);

        auto start = std::chrono::high_resolution_clock::now();
        hpx::parallel::for_loop(exec_policy, 0, size,
                                [&](int i) {
                                    c[i] = a[i] + b[i];
                                }
        );
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Time: " << std::chrono::duration<double>(end - start).count()
                  << std::endl;
    }
    if (policy == "acs") {
        std::chrono::microseconds time(acs_v);
        hpx::parallel::execution::auto_chunk_size cs(time);
        auto exec_policy = hpx::parallel::execution::par.with(cs);

        auto start = std::chrono::high_resolution_clock::now();
        hpx::parallel::for_loop(exec_policy, 0, size,
                                [&](int i) {
                                    c[i] = a[i] + b[i];
                                }
        );
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Time: " << std::chrono::duration<double>(end - start).count()
                  << std::endl;
    }
    if (policy == "dcs") {
        hpx::parallel::execution::dynamic_chunk_size cs(dcs_v);
        auto exec_policy = hpx::parallel::execution::par.with(cs);

        auto start = std::chrono::high_resolution_clock::now();
        hpx::parallel::for_loop(exec_policy, 0, size,
                                [&](int i) {
                                    c[i] = a[i] + b[i];
                                }
        );
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Time: " << std::chrono::duration<double>(end - start).count()
                  << std::endl;
    }
    for (int i = 0; i < size; ++i) {
        assert(c[i] == 5.0);
    }

    return hpx::finalize();
}

int main(int argc, char *argv[]) {
    // Configure application-specific options
    boost::program_options::options_description cmdline(
            "Usage: " HPX_APPLICATION_STRING " [options]");

    cmdline.add_options()
            ("size,s",
             boost::program_options::value<std::size_t>()->default_value(100000000),
             "the size of vector")
            ("policy,p",
             boost::program_options::value<std::string>()->default_value("acs"),
             "the chunking policy to use")
            ("dcs_val,sv",
             boost::program_options::value<std::size_t>()->default_value(1),
             "the optional value for dynamic chunk size policy")
            ("scs_val,sv",
             boost::program_options::value<std::size_t>()->default_value(0),
             "the optional value for static chunk size policy")
            ("gcs_val,gv",
             boost::program_options::value<std::size_t>()->default_value(1),
             "the value for minimum chunk size in guided chunk size policy")
            ("acs_val,av",
             boost::program_options::value<std::size_t>()->default_value(80000),
             "the value for minimum time duration to use for auto chunk size policy");
    // Initialize and run HPX
    return hpx::init(cmdline, argc, argv);
}

