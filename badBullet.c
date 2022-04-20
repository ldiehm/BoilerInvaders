const struct {
  unsigned int width;
  unsigned int height;
  unsigned int bytes_per_pixel;
  unsigned char pixel_data[2 * 4 * 2 + 1];
} badBullet = {
  2, 4, 2,
  "\000\177400\000\177400"
  "\000\177400\000\177400"
  "\000\177400\000\177400"
  "\000\177400\000\177400"
};
