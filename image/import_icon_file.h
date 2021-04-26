#ifndef __DONK_PROJECT_DONK_IMAGE_IMPORT_ICON_FILE_H__
#define __DONK_PROJECT_DONK_IMAGE_IMPORT_ICON_FILE_H__

#include <string>

#include "donk/image/dmi_parser.h"
#include "donk/image/dmi_reader.h"

namespace donk {
namespace image {

std::shared_ptr<donk::image::dmi_data_t> ImportIconFile(std::string fullpath) {
  donk::image::DMIReader reader(fullpath);
  reader.Read();
  if (reader.dmi_data() == "") {
    throw std::runtime_error(
        fmt::format("no DMI data found in image {}", fullpath));
  }
  auto dmi_data = std::make_shared<donk::image::dmi_data_t>();
  auto parser = donk::image::Parser();
  parser.parse(reader.dmi_data().c_str(), *dmi_data);
  dmi_data->image_width = reader.get_image_width();
  dmi_data->image_height = reader.get_image_height();
  return dmi_data;
}

}  // namespace image
}  // namespace donk

#endif  // __DONK_PROJECT_DONK_IMAGE_IMPORT_ICON_FILE_H__
