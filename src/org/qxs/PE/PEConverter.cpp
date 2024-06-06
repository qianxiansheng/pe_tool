#include "PEConverter.hpp"

#include <Windows.h>
#include <fstream>
#include "org/qxs/common/Common.hpp"

namespace org::qxs::pe {
    void PEConverter::convertFile(const PEFile& pr, std::filesystem::path path) {
        auto out = convertFile(pr);
        std::ofstream ofs(path, std::ios::binary);
        ofs.write((char*)out.data(), out.size());
        ofs.close();
    }
    std::vector<uint8_t> PEConverter::convertFile(const PEFile& pr) {
        auto& lastSection = pr._sections[pr._sections.size() - 1];

        size_t rawPointerEnd = static_cast<size_t>(lastSection._section_header.PointerToRawData) + 
            lastSection._section_header.SizeOfRawData;

        size_t rawSize = sizeof(IMAGE_DOS_HEADER) +
            pr._misc.size() + sizeof(IMAGE_NT_HEADERS) +
            pr._nt.FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER);

        if (rawSize < rawPointerEnd)
            rawSize = rawPointerEnd;

        size_t p = 0;

        std::vector<uint8_t> out(rawSize);
        /* DOS Header */
        memcpy(out.data() + p, &pr._dos, sizeof(IMAGE_DOS_HEADER));
        p += sizeof(IMAGE_DOS_HEADER);
       
        /* misc content */
        memcpy(out.data() + p, &pr._misc[0], pr._misc.size());
        p += pr._misc.size();
        
        /* NT Header */
        memcpy(out.data() + p, &pr._nt, sizeof(IMAGE_NT_HEADERS));
        p += sizeof(IMAGE_NT_HEADERS);

        /* Section Table */
        for (size_t i = 0; i < pr._nt.FileHeader.NumberOfSections; ++i) {
            memcpy(out.data() + p, &pr._sections[i]._section_header, sizeof(IMAGE_SECTION_HEADER));
            p += sizeof(IMAGE_SECTION_HEADER);
        }

        /* Section Data */
        for (size_t i = 0; i < pr._nt.FileHeader.NumberOfSections; ++i) {
            p = pr._sections[i]._section_header.PointerToRawData;
            memcpy(out.data() + p, &pr._sections[i]._d[0], pr._sections[i]._d.size());
        }
        return out;
    }
}