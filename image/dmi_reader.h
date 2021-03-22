// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __SNOWFROST_DONK_IMAGE_DMI_READER_H__
#define __SNOWFROST_DONK_IMAGE_DMI_READER_H__

#include <fstream>
#include <istream>
#include <string>

#include "png.h"

// The below is in large part shamelessly stolen from the libpng implementation.
// Retrieving text metadata from PNGs is not a common/easy task in the
// alternative image libraries considered for this task.

namespace donk {
namespace image {

static void userReadData(png_structp png_ptr, png_bytep data,
                         png_size_t length) {
  png_voidp a = png_get_io_ptr(png_ptr);
  ((std::istream*)a)->read((char*)data, length);
}

class DMIReader {
 public:
  DMIReader(std::string filename) : filename_(filename) {}

  void Read();
  bool validate(std::istream& source);

  std::string dmi_data();
  int get_image_width();
  int get_image_height();

 private:
  const int kPngSigSize = 8;
  std::string filename_;
  std::string dmi_data_ = "";
  int image_width_;
  int image_height_;
};

}  // namespace image
}  // namespace donk

#endif  // __SNOWFROST_DONK_IMAGE_DMI_READER_H__
