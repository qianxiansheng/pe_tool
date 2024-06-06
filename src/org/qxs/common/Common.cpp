#include "Common.hpp"

#include <fstream>

namespace org::qxs::common {
    std::vector<uint8_t> loadFile(std::filesystem::path path) {
        std::ifstream is;
        std::vector<uint8_t> buffer;
        is.open(path.generic_string(), std::ios::binary);

        if (is.is_open()) {
            is.seekg(0, std::ios::end);
            std::streampos fileSize = is.tellg();
            is.seekg(0, std::ios::beg);
            buffer.resize(static_cast<uint32_t>(fileSize));
            is.read(reinterpret_cast<char*>(buffer.data()), fileSize);
            is.close();
        }
        else {
            throw std::exception("failed to open file");
        }
        return buffer;
    }

    int round_up(int a, int b) {
        return ((a + (b - 1)) / b) * b;
    }
}