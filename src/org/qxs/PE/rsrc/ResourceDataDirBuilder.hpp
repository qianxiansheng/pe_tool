#ifndef __ORG_QXS_PE_RSRC_RESOURCEDATADIRBUILDER_H__
#define __ORG_QXS_PE_RSRC_RESOURCEDATADIRBUILDER_H__

#include <vector>
#include "org/qxs/PE/rsrc/ResourceDataDir.hpp"
#include "org/qxs/PE/rsrc/ResourceIconGroup.hpp"
#include "org/qxs/PE/rsrc/ResourceIcon.hpp"
#include "org/qxs/PE/rsrc/ResourceManifest.hpp"
#include "org/qxs/PE/PESection.hpp"
#include "org/qxs/bmp/BmpGroup.hpp"
#include "org/qxs/bmp/ColorSpaceConverter.hpp"

using namespace org::qxs::bmp;

namespace org::qxs::pe::rsrc {
    class ResourceDataDirBuilder {
    private:
        ResourceDataDir _o;
    public:
        ResourceDataDirBuilder& setIcon(const BmpGroup& bmp) {
            size_t size = bmp._bitmaps.size();

            /* Resource Icon Group */
            ResourceIconGroup icon_group;
            icon_group._id = 101;
            icon_group._d.resize(6 + (size * 14));

            /* icon dir */
            uint16_t idReserved  = 0x0000;       //����,Ϊ0
            uint16_t idType      = 0x0001;       //�ļ�����,ͼ��Ϊ1,���Ϊ2
            uint16_t idCount     = static_cast<uint16_t>(size); //ͼ�����

            /* icon dir entry */
            uint8_t  bWidth      = 0x00;         //ͼ����,��λ:����
            uint8_t  bHeight     = 0x00;         //ͼ��߶�,��λ:����
            uint8_t  bColorCount = 0x00;         //��ɫ��
            uint8_t  bReserved   = 0x00;         //����,Ϊ0
            uint16_t wPlanes     = 0x0001;       //ƽ����,һ��Ϊ1
            uint16_t wBitCount   = 0x0020;       //ÿ���ر�����
            uint32_t dwBytesInRes= 0x000000000;  //���ݿ��С
            uint16_t dwImageId   = 0x0000;       //ͼ����ԴID

            /* fill data */
            memcpy(icon_group._d.data() + 0, &idReserved, sizeof(uint16_t));
            memcpy(icon_group._d.data() + 2, &idType, sizeof(uint16_t));
            memcpy(icon_group._d.data() + 4, &idCount, sizeof(uint16_t));
            for (size_t i = 0; i < size; ++i) {
                /* icon */
                ResourceIcon icon;
                icon._id = static_cast<int>(i) + 1;
                icon._d = ColorSpaceConverter::convert_to_bgra_flipY_bmp(bmp._bitmaps[i]);
                _o._rsrc_data_dir.push_back(icon);

                /* icon group */
                bWidth = bmp._bitmaps[i]._w;
                bHeight = bmp._bitmaps[i]._h;
                dwBytesInRes = static_cast<uint32_t>(icon._d.size());
                dwImageId = icon._id;

                size_t a = 6 + (i * 14);
                memcpy(icon_group._d.data() + a + 0, &bWidth, sizeof(uint8_t));
                memcpy(icon_group._d.data() + a + 1, &bHeight, sizeof(uint8_t));
                memcpy(icon_group._d.data() + a + 2, &bColorCount, sizeof(uint8_t));
                memcpy(icon_group._d.data() + a + 3, &bReserved, sizeof(uint8_t));
                memcpy(icon_group._d.data() + a + 4, &wPlanes, sizeof(uint16_t));
                memcpy(icon_group._d.data() + a + 6, &wBitCount, sizeof(uint16_t));
                memcpy(icon_group._d.data() + a + 8, &dwBytesInRes, sizeof(uint32_t));
                memcpy(icon_group._d.data() + a + 12, &dwImageId, sizeof(uint16_t));
            }
            _o._rsrc_data_dir.push_back(icon_group);
            return *this;
        }

        ResourceDataDirBuilder& setManifest(std::vector<uint8_t> data) {
            ResourceManifest manifest;
            manifest._id = 1;
            manifest._d = data;
            _o._rsrc_data_dir.push_back(manifest);
            return *this;
        }

        ResourceDataDir build() {
            return _o;
        }
    };
}

#endif