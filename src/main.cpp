
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

    return 0;

    // auto file_base = std::fstream();
    // auto f_s =
    //     fs::FileSystem(file_base, "techno_park/2023_1_TrudoGOliki/build");
    // auto fid = fs::F::FID{"a"};
    // auto file_info = fs::F::FileInfo{"asdfasdf", 30};
    // f_s.createNewFileWrite(fid, file_info);
    // char buffer[10];
    // for (int i = 0; i < 10; i++) {
    //     buffer[i] = 'e';
    // }
    // auto buf = fs::Buffer{buffer, 10};
    // f_s.writeBuf(buf);
    // f_s.writeBuf(buf);
    // f_s.writeBuf(buf);

    // size_t size = f_s.getSizeFileRead();
    // auto out = std::ofstream("../build/output.pdf", std::ios::binary);
    // for (size_t cur_size = 0; cur_size < size;) {
    //     fs::Buffer buf = f_s.getBuf();
    //     out.write(buf.buf_, buf.size_);
    //     cur_size += buf.size_;
    // }
}
