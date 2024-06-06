#include "PELoader.hpp"

#include <Windows.h>
#include "org/qxs/common/Common.hpp"

namespace org::qxs::pe {
    
    PEFile PELoader::loadFile(std::filesystem::path path) {
        std::vector<uint8_t> d = org::qxs::common::loadFile(path);
        return loadFile(d);
    }

    PEFile PELoader::loadFile(std::vector<uint8_t>& d) {
        PIMAGE_DOS_HEADER pImageDOSHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(d.data());
        PIMAGE_NT_HEADERS pImageNTHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(d.data() + pImageDOSHeader->e_lfanew);
        PIMAGE_SECTION_HEADER pSecHeader = IMAGE_FIRST_SECTION(pImageNTHeader);

        sizeof(IMAGE_DOS_HEADER);
        sizeof(IMAGE_NT_HEADERS);
        sizeof(IMAGE_SECTION_HEADER);

        PEFile pe;
        pe._dos = *pImageDOSHeader;
        pe._nt = *pImageNTHeader;
        pe._sections.resize(pImageNTHeader->FileHeader.NumberOfSections);

        pe._misc.resize(pImageDOSHeader->e_lfanew - sizeof(IMAGE_DOS_HEADER));
        memcpy(pe._misc.data(), d.data() + sizeof(IMAGE_DOS_HEADER), pe._misc.size());

        for (uint16_t i = 0; i < pImageNTHeader->FileHeader.NumberOfSections; ++i) {
            pe._sections[i]._section_header = pSecHeader[i];
            pe._sections[i]._d.resize(pSecHeader[i].SizeOfRawData);
            memcpy(pe._sections[i]._d.data(), d.data() + pSecHeader[i].PointerToRawData, pSecHeader[i].SizeOfRawData);
        }
        return pe;
    }
}