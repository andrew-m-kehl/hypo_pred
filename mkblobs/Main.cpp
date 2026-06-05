#include "Stle/U/ArgU.hpp"
#include "Stle/U/File.hpp"
#include "Stle/U/FileReader.hpp"

#include <algorithm>

#ifdef _WIN32

#include "Stle/U/WinU.hpp"

int u8Main(int argc, char **u8Argv);

int wmain(int argc, wchar_t **argv) {
    std::unique_ptr<char*, U::ArgvDelete> u8Argv = U::makeU8Argv(argc, argv);
    return u8Main(argc, u8Argv.get());
}

#else

int u8Main(int argc, char **u8Argv);

int main(int argc, char **argv) {
    return u8Main(argc, argv);
}

#endif

const char *USAGE = "mkblobs FILES\n";

std::string makeBlobName(const char *fileName) {
    std::string blobName(fileName);
    // TODO: Use <filesystem> when available.
    auto lastSepR = std::find_if(blobName.rbegin(), blobName.rend(), [](char c){return c == '/' || c == '\\';});
    if (lastSepR != blobName.rend())        
        blobName.erase(blobName.begin(), lastSepR.base());   
    // TODO: More blob name escaping.
    std::replace_if(blobName.begin(), blobName.end(), [](char c){ return c == '.'; }, '_');
    return blobName;
}

int u8Main(int argc, char** argv) {
    try {
        U::ArgReader argReader(argc, argv);
        // Skipping the first argument (executable path).
        argReader.read();
        
        U::File blobsHpp("Blobs.hpp", "w");
        fputs("#ifndef BLOBS_HPP\n#define BLOBS_HPP\n\n", blobsHpp.file());
        
        U::File blobsCpp("Blobs.cpp", "w");
        while (argReader.read() != U::ARG_EOF) {
            // TODO: Make an option whether to read as text or not.
            U::FileReader<U::File> reader(U::OF_FILE, argReader.str(), "r");
            
            std::string blobName = makeBlobName(argReader.str());
            fprintf(blobsHpp.file(), "extern const char BLOB_%s[];\n", blobName.c_str());
            
            fprintf(blobsCpp.file(), "extern const char BLOB_%s[] = {\n", blobName.c_str());
            int column = 0;
            while (true) {
                int c = reader.getc();
                if (c == EOF)
                    break;
                if (column == 16) {
                    column = 0;
                    fprintf(blobsCpp.file(), "\n");
                }
                fprintf(blobsCpp.file(), "0x%02x, ", c);
                ++column;                
            }
            fprintf(blobsCpp.file(), "\n0x0\n};\n\n");
        }
        
        fputs("\n#endif\n", blobsHpp.file());
        
        return 0;
    }
    catch (U::ArgNoError &) {        
        return 0;
    }
    catch (U::ArgError &e) {
        fprintf(stderr, "Error: %s\n%s", e.what(), USAGE);        
    }
    catch (std::runtime_error &e) {
        fprintf(stderr, "Error: %s\n", e.what());
    }
    return -1;
}
