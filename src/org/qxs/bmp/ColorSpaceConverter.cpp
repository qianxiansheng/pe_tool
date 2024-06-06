#include "ColorSpaceConverter.hpp"

namespace org::qxs::bmp {
    std::vector<uint8_t> ColorSpaceConverter::convert_to_bgra(const BmpR8G8B8A8& bitmap) {
        const auto& src = bitmap._d;
        size_t size = src.size();
        std::vector<uint8_t> dst(size);
        for (int i = 0; i < (int)size; i += 4) {
            dst[i + 0] = src[i + 2];
            dst[i + 1] = src[i + 1];
            dst[i + 2] = src[i + 0];
            dst[i + 3] = src[i + 3];
        }
        return dst;
    }

    std::vector<uint8_t> ColorSpaceConverter::convert_to_bgra_flipY(const BmpR8G8B8A8& bitmap) {
        size_t size = bitmap._d.size();
        std::vector<uint8_t> dst(size);
        for (int i = 0; i < bitmap._h; ++i) {
            for (int j = 0; j < bitmap._w; ++j) {
                int d = ((i * bitmap._w) + j) * 4;
                int s = (((bitmap._h - 1 - i) * bitmap._w) + j) * 4;
                dst[d + 0] = bitmap._d[s + 2];
                dst[d + 1] = bitmap._d[s + 1];
                dst[d + 2] = bitmap._d[s + 0];
                dst[d + 3] = bitmap._d[s + 3];
            }
        }
        return dst;
    }

    std::vector<uint8_t> ColorSpaceConverter::convert_to_bgra_flipY_bmp(const BmpR8G8B8A8& bitmap) {
        std::vector<uint8_t> pixels = convert_to_bgra_flipY(bitmap);
        std::vector<uint8_t> dst(40 + pixels.size());

        /* bitmap info headers */
        uint32_t biSize          = 0x00000028;  // 结构体的大小，通常为40字节
        uint32_t biWidth         = 0x00000000;  // 图像的宽度，以像素为单位
        uint32_t biHeight        = 0x00000000;  // 图像的高度，以像素为单位
        uint16_t biPlanes        = 0x0001;      // 位图的颜色平面数，总是为1
        uint16_t biBitCount      = 0x0020;      // 每个像素的位数，即位深度
        uint32_t biCompression   = 0x00000000;  // 压缩类型，0表示不压缩
        uint32_t biSizeImage     = 0x00000000;  // 图像数据的大小，以字节为单位
        uint32_t biXPelsPerMeter = 0x00000000;  // 水平分辨率，像素每米
        uint32_t biYPelsPerMeter = 0x00000000;  // 垂直分辨率，像素每米
        uint32_t biClrUsed       = 0x00000000;  // 实际使用的颜色表中的颜色数
        uint32_t biClrImportant  = 0x00000000;  // 对图像显示有重要影响的颜色索引数
        biWidth                  = static_cast<uint32_t>(bitmap._w);
        biHeight                 = static_cast<uint32_t>(bitmap._h) * 2;
        biSizeImage              = static_cast<uint32_t>(bitmap._d.size());

        /* fill data */
        memcpy(dst.data() + 0, &biSize, 4);
        memcpy(dst.data() + 4, &biWidth, 4);
        memcpy(dst.data() + 8, &biHeight, 4);
        memcpy(dst.data() + 12, &biPlanes, 2);
        memcpy(dst.data() + 14, &biBitCount, 2);
        memcpy(dst.data() + 16, &biCompression, 4);
        memcpy(dst.data() + 20, &biSizeImage, 4);
        memcpy(dst.data() + 24, &biXPelsPerMeter, 4);
        memcpy(dst.data() + 28, &biYPelsPerMeter, 4);
        memcpy(dst.data() + 32, &biClrUsed, 4);
        memcpy(dst.data() + 36, &biClrImportant, 4);
        memcpy(dst.data() + 40, pixels.data(), pixels.size());
        return dst;
    }
}
