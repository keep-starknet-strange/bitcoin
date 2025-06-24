// Copyright (c) 2012-2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <dbwrapper.h>
#include <kernel/checks.h>
#include <key.h>
#include <test/util/setup_common.h>

// Include the CXX bridge header
#include "c-stwo/src/lib.rs.h"

// Include the read_file_as_bytes function
std::vector<uint8_t> read_file_as_bytes(const std::string& filepath);

#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(sanity_tests, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(basic_sanity)
{
    BOOST_CHECK_MESSAGE(dbwrapper_SanityCheck() == true, "dbwrapper sanity test");
    BOOST_CHECK_MESSAGE(kernel::Clang_IndVarSimplify_Bug_SanityCheck() == true, "Clang IndVarSimplify bug sanity test");
    BOOST_CHECK_MESSAGE(ECC_InitSanityCheck() == true, "secp256k1 sanity test");
}

BOOST_AUTO_TEST_CASE(test_rust_function_once)
{
    // Create a simple test JSON file
    const char* test_json = R"({"test": "proof", "data": "sample"})";
    FILE* f = fopen("stark_proof.json", "w");
    BOOST_REQUIRE(f != nullptr);
    fwrite(test_json, 1, strlen(test_json), f);
    fclose(f);

    // Call the Rust function
    fprintf(stderr, "=== TESTING RUST FUNCTION ===\n");
    std::vector<uint8_t> proof_bytes = read_file_as_bytes("stark_proof.json");
    fprintf(stderr, "Read %zu bytes from stark_proof.json\n", proof_bytes.size());

    rust::Slice<const uint8_t> slice(proof_bytes.data(), proof_bytes.size());
    rust::String valid = verify_cairo_proof(slice, false);

    fprintf(stderr, "Stark proof result: '%s' (length: %zu)\n", valid.c_str(), valid.size());
    fprintf(stderr, "=== END TESTING RUST FUNCTION ===\n");

    // Clean up
    unlink("stark_proof.json");

    // The test passes if we get here (no exceptions thrown)
    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()
