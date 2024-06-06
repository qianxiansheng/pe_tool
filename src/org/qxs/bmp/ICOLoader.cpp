#include "ICOLoader.hpp"

namespace org::qxs::bmp {

    ::std::shared_ptr<BmpGroup> ICOLoader::load_image(const ::std::filesystem::path path) {
        ::std::shared_ptr<BmpGroup> bmpGroup = ::std::make_shared<BmpGroup>();
        return bmpGroup;
    }
}