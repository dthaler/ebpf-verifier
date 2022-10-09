// Copyright (c) Prevail Verifier contributors.
// SPDX-License-Identifier: MIT

// This program reads BPF instructions from stdin and memory contents from
// the first argument. It then executes the BPF program and prints the
// value of r0 at the end of execution.
// The program is intended to be used with the bpf conformance test suite.

#include <iostream>
#include <sstream>
#include <optional>
#include <string>
#include <vector>
#include "ebpf_verifier.hpp"
#include "ebpf_yaml.hpp"
using string = std::string;

/**
 * @brief Read in a string of hex bytes and return a vector of bytes.
 *
 * @param[in] input String containing hex bytes.
 * @return Vector of bytes.
 */
std::vector<uint8_t>
base16_decode(const std::string& input)
{
    std::vector<uint8_t> output;
    std::stringstream ss(input);
    std::string value;
    while (std::getline(ss, value, ' ')) {
        try {
            output.push_back(std::stoi(value, nullptr, 16));
        } catch (...) {
            // Ignore invalid values.
        }
    }
    return output;
}

/**
 * @brief This program reads BPF instructions from stdin and memory contents from
 * the first agument. It then executes the BPF program and prints the
 * value of r0 at the end of execution.
 */
int
main(int argc, char** argv)
{
    std::cerr << "argc: " << argc << std::endl;
    bool debug = false;
    std::vector<std::string> args(argv, argv + argc);
    if (args.size() > 0) {
        args.erase(args.begin());
    }
    std::string program_string;
    std::string memory_string;

    if (args.size() > 0 && args[0] == "--help") {
        std::cout << "usage: " << argv[0] << " [--program <base16 program bytes>] [<base16 memory bytes>] [--debug]\n";
        return 1;
    }

    if (args.size() > 1 && args[0] == "--program") {
        args.erase(args.begin());
        program_string = args[0];
        args.erase(args.begin());
    } else {
        std::getline(std::cin, program_string);
    }
    std::cerr << "program_string: " << program_string << std::endl;

    // First parameter is optional memory contents.
    if (args.size() > 0 && args[0] != "--debug") {
        memory_string = args[0];
        args.erase(args.begin());
        std::cerr << "memory_string: " << memory_string << std::endl;
    }
    if (args.size() > 0 && args[0] == "--debug") {
        debug = true;
        args.erase(args.begin());
    }
    if (args.size() > 0) {
        std::cerr << "Unexpected arguments: " << args[0] << std::endl;
        return 1;
    }

    uint64_t r0_value;
    bool result = run_conformance_test_case(base16_decode(memory_string), base16_decode(program_string), &r0_value, debug);
    if (!result) {
        if (debug) {
            std::cerr << "Verification failed\n";
        }
        return 1;
    }

    // Print output.
    std::cout << std::hex << r0_value << std::endl;

    return 0;
}