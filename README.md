# pe_tool
PE Tool

示例：将A.exe的图标替换成B.png并保存为C.exe

```C++
auto exe_path = std::filesystem::current_path() / "A.exe"; 
auto ico_path = std::filesystem::current_path() / "B.png";
auto out_path = std::filesystem::current_path() / "C.exe";

/* Load png as bitmap */
org::qxs::bmp::PNGLoader loader;
auto bitmap = loader.load_image(ico_path);

/* create other resolution */
org::qxs::bmp::scaler::Scaler* scaler = new org::qxs::bmp::scaler::BilinearInterpolatorScaler();
bitmap->_bitmaps.push_back(scaler->scaler(bitmap->_bitmaps[0], 16, 16));
bitmap->_bitmaps.push_back(scaler->scaler(bitmap->_bitmaps[0], 24, 24));
bitmap->_bitmaps.push_back(scaler->scaler(bitmap->_bitmaps[0], 32, 32));
bitmap->_bitmaps.push_back(scaler->scaler(bitmap->_bitmaps[0], 48, 48));
bitmap->_bitmaps.push_back(scaler->scaler(bitmap->_bitmaps[0], 64, 64));
bitmap->_bitmaps.push_back(scaler->scaler(bitmap->_bitmaps[0], 72, 72));
bitmap->_bitmaps.push_back(scaler->scaler(bitmap->_bitmaps[0], 80, 80));
bitmap->_bitmaps.push_back(scaler->scaler(bitmap->_bitmaps[0], 96, 96));
bitmap->_bitmaps.push_back(scaler->scaler(bitmap->_bitmaps[0], 128, 128));
bitmap->_bitmaps.push_back(scaler->scaler(bitmap->_bitmaps[0], 256, 256));
bitmap->_bitmaps.erase(bitmap->_bitmaps.begin());
delete scaler;

/* create PE resource data directory */
auto resourceDataDir = org::qxs::pe::rsrc::ResourceDataDirBuilder()
    .setIcon(*bitmap)
    .build();

/* Load exe as PE structure */
auto peFile = org::qxs::pe::PELoader::loadFile(exe_path);
/* replace resource data directory */
org::qxs::pe::IconTool::replaceIcon(peFile, resourceDataDir._rsrc_data_dir);
/* save */
org::qxs::pe::PEConverter::convertFile(peFile, out_path);
```

