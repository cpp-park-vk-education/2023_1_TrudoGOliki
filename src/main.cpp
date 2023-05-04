#include "cli.hpp"

int main(int argc, char *argv[]) {
    parse_args(argc, const_cast<const char **>(argv));
    return 0;
}