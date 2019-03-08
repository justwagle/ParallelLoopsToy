//  Copyright (c) 2019 Bibek Wagle
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>
#include <hpx/include/iostreams.hpp>
#include <hpx/include/async.hpp>
#include <hpx/include/serialization.hpp>

#include <cstddef>
#include <complex>
#include <string>
#include <vector>


int hpx_main(boost::program_options::variables_map& vm)
{
    hpx::cout << "Hello Parallel Loops!! " << n << "\n" << hpx::flush;
    return hpx::finalize();
}

int main(int argc, char* argv[])
{
    // Configure application-specific options
    boost::program_options::options_description cmdline(
            "Usage: " HPX_APPLICATION_STRING " [options]");

    cmdline.add_options()
            ("iterations,i",
             boost::program_options::value<std::size_t>()->default_value(100000),
             "the number of iterations");
    // Initialize and run HPX
    return hpx::init(cmdline,argc, argv);
}

