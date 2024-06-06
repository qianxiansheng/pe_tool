#include "BMPLoader.hpp"

#include "stb_image.h"

namespace org::qxs::bmp {

    ::std::shared_ptr<BmpGroup> BMPLoader::load_image(const ::std::filesystem::path path) {

        if (!std::filesystem::exists(path) ||
            !std::filesystem::is_regular_file(path)) {
            return nullptr;
        }

        int width, height, numChannels;
        unsigned char* imageData = stbi_load(path.generic_string().c_str(), &width, &height, &numChannels, STBI_rgb_alpha);
        if (!imageData)
            return nullptr;

        int req_channels = 4;

        int size = width * height * req_channels;

        std::shared_ptr<BmpGroup> bmpGroup = std::make_shared<BmpGroup>();
        bmpGroup->_bitmaps.resize(1);
        bmpGroup->_bitmaps[0]._w = width;
        bmpGroup->_bitmaps[0]._h = height;
        bmpGroup->_bitmaps[0]._num_channels = req_channels;
        bmpGroup->_bitmaps[0]._d.resize(size);
        memcpy(bmpGroup->_bitmaps[0]._d.data(), imageData, bmpGroup->_bitmaps[0]._d.size());

        stbi_image_free(imageData);

        return bmpGroup;
    }
}