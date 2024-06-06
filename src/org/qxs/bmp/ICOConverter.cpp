#include "ICOConverter.hpp"

#include <limits>
#include "ColorSpaceConverter.hpp"

namespace org::qxs::bmp {


    void ICOConverter::convert(const BmpGroup& bitmap_group, std::ostream& stream) {
        size_t size = bitmap_group._bitmaps.size();

        /* icon dir */
        uint16_t idReserved      = 0x00;
        uint16_t idType          = 0x01;
        uint16_t idCount         = static_cast<uint16_t>(size);

        /* icon dir entry */
        uint8_t  bWidth          = 0x00;
        uint8_t  bHeight         = 0x00;
        uint8_t  bColorCount     = 0x00;
        uint8_t  bReserved       = 0x00;
        uint16_t wPlanes         = 0x0001;
        uint16_t wBitCount       = 0x0020;
        uint32_t dwBytesInRes    = 0xFFFFFFFF;
        uint32_t dwImageOffset   = 0xFFFFFFFF;

        /* bitmap info headers */
        uint32_t biSize          = 0x00000028;  // �ṹ��Ĵ�С��ͨ��Ϊ40�ֽ�
        uint32_t biWidth         = 0x00000000;  // ͼ��Ŀ�ȣ�������Ϊ��λ
        uint32_t biHeight        = 0x00000000;  // ͼ��ĸ߶ȣ�������Ϊ��λ
        uint16_t biPlanes        = 0x0001;      // λͼ����ɫƽ����������Ϊ1
        uint16_t biBitCount      = 0x0020;      // ÿ�����ص�λ������λ���
        uint32_t biCompression   = 0x00000000;  // ѹ�����ͣ�0��ʾ��ѹ��
        uint32_t biSizeImage     = 0x00000000;  // ͼ�����ݵĴ�С�����ֽ�Ϊ��λ
        uint32_t biXPelsPerMeter = 0x00000000;  // ˮƽ�ֱ��ʣ�����ÿ��
        uint32_t biYPelsPerMeter = 0x00000000;  // ��ֱ�ֱ��ʣ�����ÿ��
        uint32_t biClrUsed       = 0x00000000;  // ʵ��ʹ�õ���ɫ���е���ɫ��
        uint32_t biClrImportant  = 0x00000000;  // ��ͼ����ʾ����ҪӰ�����ɫ������

        stream.write((char*)&idReserved, 2);
        stream.write((char*)&idType, 2);
        stream.write((char*)&idCount, 2);

        uint32_t headerSize = static_cast<uint32_t>(6 + size * 16);
        dwImageOffset = headerSize;

        for (size_t i = 0; i < size; ++i) {
            const auto& bitmap = bitmap_group._bitmaps[i];

            if (bitmap._w > std::numeric_limits<uint8_t>::max() + 1 ||
                bitmap._h > std::numeric_limits<uint8_t>::max() + 1) {
                continue;
            }
            bWidth  = static_cast<uint8_t>(bitmap._w);
            bHeight = static_cast<uint8_t>(bitmap._h);
            dwBytesInRes = static_cast<uint32_t>(bitmap._d.size()) + biSize;

            stream.write((char*)&bWidth, 1);
            stream.write((char*)&bHeight, 1);
            stream.write((char*)&bColorCount, 1);
            stream.write((char*)&bReserved, 1);
            stream.write((char*)&wPlanes, 2);
            stream.write((char*)&wBitCount, 2);
            stream.write((char*)&dwBytesInRes, 4);
            stream.write((char*)&dwImageOffset, 4);

            dwImageOffset += dwBytesInRes;
        }

        for (const auto& bitmap : bitmap_group._bitmaps) {

            biWidth = static_cast<uint32_t>(bitmap._w);
            biHeight = static_cast<uint32_t>(bitmap._h) * 2;
            biSizeImage = static_cast<uint32_t>(bitmap._d.size());

            stream.write((char*)&biSize, 4);
            stream.write((char*)&biWidth, 4);
            stream.write((char*)&biHeight, 4);
            stream.write((char*)&biPlanes, 2);
            stream.write((char*)&biBitCount, 2);
            stream.write((char*)&biCompression, 4);
            stream.write((char*)&biSizeImage, 4);
            stream.write((char*)&biXPelsPerMeter, 4);
            stream.write((char*)&biYPelsPerMeter, 4);
            stream.write((char*)&biClrUsed, 4);
            stream.write((char*)&biClrImportant, 4);

            auto dataBRGA = ColorSpaceConverter::convert_to_bgra_flipY(bitmap);
            stream.write((char*)dataBRGA.data(), dataBRGA.size());
        }
    }
}