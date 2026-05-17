#pragma once

#include <string>
#include <functional>
#include <vector>

//use this as the test name to print a space in the test segment
#define TEST_NAME_FOR_SPACE "SPACE"

struct test_fail {
    std::string test_name;
    std::string reason;
};

struct test_t {
    std::string name;
    std::function<void()> test_fun;

    test_t(std::string name, std::function<void()> test_fun) {
        this->name = name;
        this->test_fun = test_fun;
    }
};