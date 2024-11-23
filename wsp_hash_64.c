#include "wsp_hash_64.h"

uint32_t _wsp_hash_read_32(const uint8_t *input, unsigned long i) {
  uint32_t input_aligned;

  memcpy(&input_aligned, &input[i], sizeof(input_aligned));
  return input_aligned;
}

uint64_t _wsp_hash_read_64(const uint8_t *input, unsigned long i) {
  uint64_t input_aligned;

  memcpy(&input_aligned, &input[i], sizeof(input_aligned));
  return input_aligned;
}

uint64_t wsp_hash_64(unsigned long input_count, const uint8_t *input) {
  uint64_t input_aligned_capture_a;
  uint64_t input_aligned_capture_b;
  uint64_t input_aligned_capture_c;
  uint64_t input_aligned_capture_d;
  uint64_t a = 1;
  uint64_t b = 11;
  uint64_t c = 111;
  uint64_t d = 1111;
  uint64_t mix = 1111111111;
  uint64_t mix_offset = 111111111;
  unsigned long input_count_capture = input_count;
  unsigned long i = 0;

  if (input_count >= 32) {
    i = 31;

    while (i < input_count) {
      input_aligned_capture_a = _wsp_hash_read_64(input, i - 7);
      input_aligned_capture_b = _wsp_hash_read_64(input, i - 15);
      input_aligned_capture_c = _wsp_hash_read_64(input, i - 23);
      input_aligned_capture_d = _wsp_hash_read_64(input, i - 31);
      mix += input_aligned_capture_a + input_aligned_capture_b
        + input_aligned_capture_c + input_aligned_capture_d;
      a += input_aligned_capture_a + ((a << 30) | (a >> 34)) + mix;
      b += input_aligned_capture_b + ((b << 29) | (b >> 35));
      c += input_aligned_capture_c + ((c << 28) | (c >> 36));
      d += input_aligned_capture_d + ((d << 27) | (d >> 37));
      i += 32;
    }

    if (i >= input_count) {
      i -= 32;
    }

    mix_offset += input_aligned_capture_a + input_aligned_capture_b
        + input_aligned_capture_c + input_aligned_capture_d + a + b + c + d;
    i++;
  }

  if ((input_count - i) >= 16) {
    i += 16;
    input_aligned_capture_a = _wsp_hash_read_64(input, i - 16);
    input_aligned_capture_b = _wsp_hash_read_64(input, i - 8);
    mix += input_aligned_capture_a + input_aligned_capture_b;
    a += input_aligned_capture_a + ((a << 30) | (a >> 34)) + mix;
    b += input_aligned_capture_b + ((b << 28) | (b >> 36)) + mix;
  }

  if ((input_count - i) >= 8) {
    i += 8;
    input_aligned_capture_a = _wsp_hash_read_64(input, i - 8);
    mix += input_aligned_capture_a + a;
    a += ((input_aligned_capture_a << 30) | (input_aligned_capture_a >> 34))
      + mix;
  }

  if (i != input_count) {
    input_count -= i;

    if (input_count >= 4) {
      mix += (a << 16) | (a >> 48);
      a += _wsp_hash_read_32(input, i) + mix_offset;

      if (input_count != 4) {
        mix += (b << 20) | (b >> 44);

        switch (input_count) {
          case 7:
            b += input[i + 6] << 16;
          case 6:
            b += input[i + 5] << 8;
          case 5:
            b += input[i + 4];
        }
      }
    }

    switch (input_count) {
      case 3:
        a += input[i + 2] << 16;
      case 2:
        a += input[i + 1] << 8;
      case 1:
        a += input[i] + mix;
    }
  }

  a += b + mix_offset + input_count_capture;
  mix += ((a << 16) | (a >> 48)) + ((c << 44) | (c >> 20))
    + ((d << 22) | (d >> 42));
  mix_offset += ((a << 40) | (a >> 24)) ^ ((b << 36) | (b >> 28));
  mix ^= a + d + ((d << 14) | (d >> 50));
  b ^= mix_offset + input_count_capture;
  mix_offset += a ^ (((b << 36) | (b >> 28)) + mix);
  a = b + d + mix_offset;
  mix += c + ((d << 22) | (d >> 42));
  mix_offset += ((a << 40) | (a >> 24)) ^ ((b << 36) | (b >> 28));
  b ^= mix + mix_offset;
  mix += ((a << 26) | (a >> 38)) ^ d;
  mix_offset += a ^ ((b << 36) | (b >> 28));
  return mix + mix_offset;
}

void wsp_hash_64_initialize(struct wsp_hash_64_s *s) {
  s->a = 1;
  s->b = 11;
  s->c = 111;
  s->d = 1111;
  s->mix = 1111111111;
  s->mix_offset = 111111111;
  s->input_count_capture = 0;
}

void wsp_hash_64_transform(unsigned long i, unsigned long input_count,
                           const uint8_t *input,
                           struct wsp_hash_64_s *s) {
  uint64_t input_aligned_capture_a;
  uint64_t input_aligned_capture_b;
  uint64_t input_aligned_capture_c;
  uint64_t input_aligned_capture_d;

  s->input_count_capture += input_count;

  if (input_count >= 32) {
    i = 31;

    while (i < input_count) {
      input_aligned_capture_a = _wsp_hash_read_64(input, i - 7);
      input_aligned_capture_b = _wsp_hash_read_64(input, i - 15);
      input_aligned_capture_c = _wsp_hash_read_64(input, i - 23);
      input_aligned_capture_d = _wsp_hash_read_64(input, i - 31);
      s->mix += input_aligned_capture_a + input_aligned_capture_b
        + input_aligned_capture_c + input_aligned_capture_d;
      s->a += input_aligned_capture_a + ((s->a << 30) | (s->a >> 34)) + s->mix;
      s->b += input_aligned_capture_b + ((s->b << 29) | (s->b >> 35));
      s->c += input_aligned_capture_c + ((s->c << 28) | (s->c >> 36));
      s->d += input_aligned_capture_d + ((s->d << 27) | (s->d >> 37));
      i += 32;
    }

    if (i >= input_count) {
      i -= 32;
    }

    s->mix_offset += input_aligned_capture_a + input_aligned_capture_b
      + input_aligned_capture_c + input_aligned_capture_d + s->a + s->b + s->c
      + s->d;
    i++;
  }

  if ((input_count - i) >= 16) {
    i += 16;
    input_aligned_capture_a = _wsp_hash_read_64(input, i - 16);
    input_aligned_capture_b = _wsp_hash_read_64(input, i - 8);
    s->mix += input_aligned_capture_a + input_aligned_capture_b;
    s->a += input_aligned_capture_a + ((s->a << 30) | (s->a >> 34)) + s->mix;
    s->b += input_aligned_capture_b + ((s->b << 28) | (s->b >> 36)) + s->mix;
  }

  if ((input_count - i) >= 8) {
    i += 8;
    input_aligned_capture_a = _wsp_hash_read_64(input, i - 8);
    s->mix += input_aligned_capture_a + s->a;
    s->a += ((input_aligned_capture_a << 30) | (input_aligned_capture_a >> 34))
      + s->mix;
  }

  if (i != input_count) {
    input_count -= i;

    if (input_count >= 4) {
      s->mix += (s->a << 16) | (s->a >> 48);
      s->a += _wsp_hash_read_32(input, i) + s->mix_offset;

      if (input_count != 4) {
        s->mix += (s->b << 20) | (s->b >> 44);

        switch (input_count) {
          case 7:
            s->b += input[i + 6] << 16;
          case 6:
            s->b += input[i + 5] << 8;
          case 5:
            s->b += input[i + 4];
        }
      }
    }

    switch (input_count) {
      case 3:
        s->a += input[i + 2] << 16;
      case 2:
        s->a += input[i + 1] << 8;
      case 1:
        s->a += input[i] + s->mix;
    }
  }
}

void wsp_hash_64_finalize(struct wsp_hash_64_s *s) {
  s->a += s->b + s->mix_offset + s->input_count_capture;
  s->mix += ((s->a << 16) | (s->a >> 48)) + ((s->c << 44) | (s->c >> 20))
    + ((s->d << 22) | (s->d >> 42));
  s->mix_offset += ((s->a << 40) | (s->a >> 24))
    ^ ((s->b << 36) | (s->b >> 28));
  s->mix ^= s->a + s->d + ((s->d << 14) | (s->d >> 50));
  s->b ^= s->mix_offset + s->input_count_capture;
  s->mix_offset += s->a ^ (((s->b << 36) | (s->b >> 28)) + s->mix);
  s->a = s->b + s->d + s->mix_offset;
  s->mix += s->c + ((s->d << 22) | (s->d >> 42));
  s->mix_offset += ((s->a << 40) | (s->a >> 24))
    ^ ((s->b << 36) | (s->b >> 28));
  s->b ^= s->mix + s->mix_offset;
  s->mix += (s->a ^ ((s->b << 36) | (s->b >> 28)))
    + (((s->a << 26) | (s->a >> 38)) ^ s->d) + s->mix_offset;
}
