#include "BilinearInterpolatorScaler.hpp"

namespace org::qxs::bmp::scaler {

    float BilinearInterpolatorScaler::interpolation(int a, int b, float u) {
        return a + (b - a) * u;
    }

    BmpR8G8B8A8 BilinearInterpolatorScaler::scaler(const BmpR8G8B8A8& bmp, 
        uint32_t targetWidth, uint32_t targetHeight) {

        BmpR8G8B8A8 r;
        r._d.resize(targetWidth * targetHeight * 4);
        r._w = targetWidth;
        r._h = targetHeight;
        r._num_channels = bmp._num_channels;

        uint8_t pixel1[4];
        uint8_t pixel2[4];
        uint8_t pixel3[4];
        uint8_t pixel4[4];
        uint8_t pixel1_2[4];
        uint8_t pixel3_4[4];
        uint8_t pixel1_2_3_4[4];

        for (int i = 0; i < (int)targetHeight; i++) {
            for (int j = 0; j < (int)targetWidth; j++) {
                int pixel = i * targetWidth + j;

                float xu = (float)j / targetWidth;
                float yu = (float)i / targetHeight;

                float sampleX = interpolation(0, bmp._w, xu);
                float sampleY = interpolation(0, bmp._h, yu);

                int x1 = (int)sampleX;
                int x2 = 1 + x1;
                int y1 = (int)sampleY;
                int y2 = 1 + y1;

                if (x2 >= bmp._w) {
                    if (y2 >= bmp._h) {
                        pixel1_2_3_4[0] = bmp._d.data()[(x1 + y1 * bmp._w) * 4 + 0];
                        pixel1_2_3_4[1] = bmp._d.data()[(x1 + y1 * bmp._w) * 4 + 1];
                        pixel1_2_3_4[2] = bmp._d.data()[(x1 + y1 * bmp._w) * 4 + 2];
                        pixel1_2_3_4[3] = bmp._d.data()[(x1 + y1 * bmp._w) * 4 + 3];
                    }
                    else {
                        pixel1[0] = bmp._d.data()[(x1 + y1 * bmp._w) * 4 + 0];
                        pixel1[1] = bmp._d.data()[(x1 + y1 * bmp._w) * 4 + 1];
                        pixel1[2] = bmp._d.data()[(x1 + y1 * bmp._w) * 4 + 2];
                        pixel1[3] = bmp._d.data()[(x1 + y1 * bmp._w) * 4 + 3];

                        pixel3[0] = bmp._d.data()[(x1 + y2 * bmp._w) * 4 + 0];
                        pixel3[1] = bmp._d.data()[(x1 + y2 * bmp._w) * 4 + 1];
                        pixel3[2] = bmp._d.data()[(x1 + y2 * bmp._w) * 4 + 2];
                        pixel3[3] = bmp._d.data()[(x1 + y2 * bmp._w) * 4 + 3];

                        pixel1_2_3_4[0] = (uint8_t)interpolation(pixel1[0] & 0xFF, pixel3[0] & 0xFF, 0.5f);
                        pixel1_2_3_4[1] = (uint8_t)interpolation(pixel1[1] & 0xFF, pixel3[1] & 0xFF, 0.5f);
                        pixel1_2_3_4[2] = (uint8_t)interpolation(pixel1[2] & 0xFF, pixel3[2] & 0xFF, 0.5f);
                        pixel1_2_3_4[3] = (uint8_t)interpolation(pixel1[3] & 0xFF, pixel3[3] & 0xFF, 0.5f);
                    }
                }
                else {
                    if (y2 >= bmp._h) {
                        pixel1[0] = bmp._d.data()[(x1 + y1 * bmp._w) * 4 + 0];
                        pixel1[1] = bmp._d.data()[(x1 + y1 * bmp._w) * 4 + 1];
                        pixel1[2] = bmp._d.data()[(x1 + y1 * bmp._w) * 4 + 2];
                        pixel1[3] = bmp._d.data()[(x1 + y1 * bmp._w) * 4 + 3];

                        pixel2[0] = bmp._d.data()[(x2 + y1 * bmp._w) * 4 + 0];
                        pixel2[1] = bmp._d.data()[(x2 + y1 * bmp._w) * 4 + 1];
                        pixel2[2] = bmp._d.data()[(x2 + y1 * bmp._w) * 4 + 2];
                        pixel2[3] = bmp._d.data()[(x2 + y1 * bmp._w) * 4 + 3];

                        pixel1_2_3_4[0] = (uint8_t)interpolation(pixel1[0] & 0xFF, pixel2[0] & 0xFF, 0.5f);
                        pixel1_2_3_4[1] = (uint8_t)interpolation(pixel1[1] & 0xFF, pixel2[1] & 0xFF, 0.5f);
                        pixel1_2_3_4[2] = (uint8_t)interpolation(pixel1[2] & 0xFF, pixel2[2] & 0xFF, 0.5f);
                        pixel1_2_3_4[3] = (uint8_t)interpolation(pixel1[3] & 0xFF, pixel2[3] & 0xFF, 0.5f);
                    }
                    else {
                        pixel1[0] = bmp._d.data()[(x1 + y1 * bmp._w) * 4 + 0];
                        pixel1[1] = bmp._d.data()[(x1 + y1 * bmp._w) * 4 + 1];
                        pixel1[2] = bmp._d.data()[(x1 + y1 * bmp._w) * 4 + 2];
                        pixel1[3] = bmp._d.data()[(x1 + y1 * bmp._w) * 4 + 3];

                        pixel2[0] = bmp._d.data()[(x2 + y1 * bmp._w) * 4 + 0];
                        pixel2[1] = bmp._d.data()[(x2 + y1 * bmp._w) * 4 + 1];
                        pixel2[2] = bmp._d.data()[(x2 + y1 * bmp._w) * 4 + 2];
                        pixel2[3] = bmp._d.data()[(x2 + y1 * bmp._w) * 4 + 3];

                        pixel3[0] = bmp._d.data()[(x1 + y2 * bmp._w) * 4 + 0];
                        pixel3[1] = bmp._d.data()[(x1 + y2 * bmp._w) * 4 + 1];
                        pixel3[2] = bmp._d.data()[(x1 + y2 * bmp._w) * 4 + 2];
                        pixel3[3] = bmp._d.data()[(x1 + y2 * bmp._w) * 4 + 3];

                        pixel4[0] = bmp._d.data()[(x2 + y2 * bmp._w) * 4 + 0];
                        pixel4[1] = bmp._d.data()[(x2 + y2 * bmp._w) * 4 + 1];
                        pixel4[2] = bmp._d.data()[(x2 + y2 * bmp._w) * 4 + 2];
                        pixel4[3] = bmp._d.data()[(x2 + y2 * bmp._w) * 4 + 3];

                        pixel1_2[0] = (uint8_t)interpolation(pixel1[0] & 0xFF, pixel2[0] & 0xFF, 0.5f);
                        pixel1_2[1] = (uint8_t)interpolation(pixel1[1] & 0xFF, pixel2[1] & 0xFF, 0.5f);
                        pixel1_2[2] = (uint8_t)interpolation(pixel1[2] & 0xFF, pixel2[2] & 0xFF, 0.5f);
                        pixel1_2[3] = (uint8_t)interpolation(pixel1[3] & 0xFF, pixel2[3] & 0xFF, 0.5f);

                        pixel3_4[0] = (uint8_t)interpolation(pixel3[0] & 0xFF, pixel4[0] & 0xFF, 0.5f);
                        pixel3_4[1] = (uint8_t)interpolation(pixel3[1] & 0xFF, pixel4[1] & 0xFF, 0.5f);
                        pixel3_4[2] = (uint8_t)interpolation(pixel3[2] & 0xFF, pixel4[2] & 0xFF, 0.5f);
                        pixel3_4[3] = (uint8_t)interpolation(pixel3[3] & 0xFF, pixel4[3] & 0xFF, 0.5f);

                        pixel1_2_3_4[0] = (uint8_t)interpolation(pixel1_2[0] & 0xFF, pixel3_4[0] & 0xFF, 0.5f);
                        pixel1_2_3_4[1] = (uint8_t)interpolation(pixel1_2[1] & 0xFF, pixel3_4[1] & 0xFF, 0.5f);
                        pixel1_2_3_4[2] = (uint8_t)interpolation(pixel1_2[2] & 0xFF, pixel3_4[2] & 0xFF, 0.5f);
                        pixel1_2_3_4[3] = (uint8_t)interpolation(pixel1_2[3] & 0xFF, pixel3_4[3] & 0xFF, 0.5f);
                    }
                }

                r._d.data()[pixel * 4 + 0] = pixel1_2_3_4[0];
                r._d.data()[pixel * 4 + 1] = pixel1_2_3_4[1];
                r._d.data()[pixel * 4 + 2] = pixel1_2_3_4[2];
                r._d.data()[pixel * 4 + 3] = pixel1_2_3_4[3];
            }
        }
        return r;
    }
}