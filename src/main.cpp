#include "cli.hpp"

int main(int argc, char *argv[]) {

    CLI cliI;
    cliI.ParseArgs(argc, argv);
    // cliI.Execute();
    return 0;
}