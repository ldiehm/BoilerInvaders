const struct {
  unsigned int width;
  unsigned int height;
  unsigned int bytes_per_pixel;
  unsigned char pixel_data[4 * 2 * 2 + 1];
} bulletShadow = {
  4, 2, 2,
  "\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000"
};
