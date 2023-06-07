#pragma once
#include <stdexcept>

namespace fs {
class FSError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};
}