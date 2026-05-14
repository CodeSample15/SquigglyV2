#pragma once

#include <string>
#include <functional>
#include <vector>

struct test_fail {
    std::string test_name;
    std::string reason;
};

struct test_t {
    std::string name;
    std::function<void()> test_fun;
};