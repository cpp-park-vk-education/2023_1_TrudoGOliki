
#include "cli.hpp"
#include "file_system.hpp"

#include <iostream>

int main(int argc, char *argv[]) {

    CLI cliI;
    try {
        cliI.ParseArgs(argc, argv);
    } catch (const std::exception &ex) {
        throw CLI::CliParseError("in ParseArgs: " +
                                 static_cast<std::string>(ex.what()));
    } catch (...) {
        std::cerr << "Some other exception\n";
        return 1;
    }

    cliI.Execute();

    //////////////////////////
    auto manager_net = fs::ManagerFilesNet();
    manager_net.selectNewFile(fs::F::Path(".gitignore"));
    fs::Buffer buf = manager_net.getBuf();
    for (int i = 0; i < fs::STANDARD_BUFFER_SIZE; i++) {
        std::cout << buf.buf_[i];
    }
    /////////////////////////////
    return 0;
}
