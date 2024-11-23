#ifndef WSP_HASH_64_H
#define WSP_HASH_64_H

#include <stdint.h>
#include <string.h>

struct wsp_hash_64_s {
  uint64_t a;
  uint64_t b;
  uint64_t c;
  uint64_t d;
  uint64_t mix;
  uint64_t mix_offset;
  unsigned long input_count_capture;
};

uint64_t wsp_hash_64(unsigned long input_count, const uint8_t *input);

void wsp_hash_64_initialize(struct wsp_hash_64_s *s);

void wsp_hash_64_transform(unsigned long i, unsigned long input_count,
                           const uint8_t *input,
                           struct wsp_hash_64_s *s);

void wsp_hash_64_finalize(struct wsp_hash_64_s *s);

#endif
