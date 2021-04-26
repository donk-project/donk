// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#include "donk/image/dmi_reader.h"

#include <fstream>
#include <istream>
#include <string>

#include "fmt/format.h"
#include "png.h"
#include "spdlog/spdlog.h"

namespace donk {
namespace image {

void DMIReader::Read() {
  std::ifstream file(filename_, std::ios_base::in | std::ios_base::binary);
  if (file.is_open()) {
    bool valid = validate(file);
    if (!valid) {
      throw std::runtime_error(fmt::format("invalid PNG {}", filename_));
    }
    png_structp png_ptr =
        png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
      throw std::runtime_error("couldn't initialize PNG read struct");
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
      png_destroy_read_struct(&png_ptr, (png_infopp)0, (png_infopp)0);
      throw std::runtime_error("couldn't initialize PNG info struct");
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
      png_destroy_write_struct(&png_ptr, &info_ptr);
      return;
    }

    png_set_read_fn(png_ptr, (png_voidp)&file, userReadData);
    png_set_sig_bytes(png_ptr, kPngSigSize);
    png_read_info(png_ptr, info_ptr);

    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_type, 0, 0);

    image_width_ = width;
    image_height_ = height;

    png_textp tp = NULL;
    png_uint_32 num_text = png_get_text(png_ptr, info_ptr, &tp, NULL);
    for (int i = 0; i < num_text; ++i) {
      if (std::string(tp[i].key) == "Description") {
        dmi_data_ = std::string(tp[i].text);
      }
    }
  }
}

bool DMIReader::validate(std::istream& source) {
  png_byte pngsig[8];
  int is_png = 0;

  source.read((char*)pngsig, 8);
  if (!source.good()) {
    throw std::system_error(errno, std::system_category(),
                            "failed to read sig");
  }

  is_png = png_sig_cmp(pngsig, 0, 8);
  return (is_png == 0);
}

std::string DMIReader::dmi_data() { return dmi_data_; }

int DMIReader::get_image_width() { return image_width_; }

int DMIReader::get_image_height() { return image_height_; }

}  // namespace image
}  // namespace donk
