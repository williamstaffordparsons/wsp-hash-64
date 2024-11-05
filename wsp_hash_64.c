#include "wsp_hash_64.h"

uint32_t _wsp_hash_read_32(const uint8_t *input, unsigned long i) {
  uint32_t _input;

  memcpy(&_input, &input[i], sizeof(_input));
  return _input;
}

uint64_t _wsp_hash_read_64(const uint8_t *input, unsigned long i) {
  uint64_t _input;

  memcpy(&_input, &input[i], sizeof(_input));
  return _input;
}

uint64_t wsp_hash_64(unsigned long input_count, const uint8_t *input) {
  uint64_t _a;
  uint64_t _b;
  uint64_t _c;
  uint64_t _d;
  uint64_t a = 1;
  uint64_t b = 11;
  uint64_t c = 111;
  uint64_t d = 1111;
  uint64_t _state = 1111111111;
  uint64_t state = 11111111111;
  unsigned long i = 0;

  if (input_count >= 32) {
    i = 31;

    while (i < input_count) {
      _a = _wsp_hash_read_64(input, i - 7);
      _b = _wsp_hash_read_64(input, i - 15);
      _c = _wsp_hash_read_64(input, i - 23);
      _d = _wsp_hash_read_64(input, i - 31);
      state += _a + _b + _c + _d;
      a += _a + ((a << 30) | (a >> 34)) + state + 1;
      b += _b + ((b << 29) | (b >> 35)) + state + 11;
      c += _c + ((c << 28) | (c >> 36)) + 111;
      d += _d + ((d << 27) | (d >> 37)) + 1111;
      i += 32;
    }

    if (i >= input_count) {
      i -= 32;
    }

    state += a + b + c + d;
    i++;
    input_count -= i;
  }

  if (input_count >= 16) {
    input_count -= 16;
    i += 16;
    _a = _wsp_hash_read_64(input, i - 16);
    _b = _wsp_hash_read_64(input, i - 8);
    state += _a + _b;
    a += _a + ((a << 30) | (a >> 34)) + state + 1;
    b += _b + ((b << 29) | (b >> 35)) + state + 11;
  }

  if (input_count >= 8) {
    input_count -= 8;
    i += 8;
    _a = _wsp_hash_read_64(input, i - 8);
    state += _a;
    a += _a + ((a << 30) | (a >> 34)) + state + 1;
  }

  if (input_count != 0) {
    state += ((a << 16) | (a >> 48)) + _state;

    if (input_count >= 4) {
      a += _wsp_hash_read_32(input, i) + state + 1111111111;

      if (input_count != 4) {
        _state += a + b;
        state += ((b << 20) | (b >> 44)) + _state;

        if (input_count == 7) {
          b += (input[i + 4] | (input[i + 5] << 8)
            | (input[i + 6] << 16)) + state + 111111111;
        } else {
          if (input_count == 6) {
            b += (input[i + 4] | (input[i + 5] << 8)) + state + 11111111;
          } else {
            b += input[i + 4] + state + 1111111;
          }
        }
      }
    } else {
      if (input_count == 3) {
        a += (input[i] | (input[i + 1] << 8)
          | (input[i + 2] << 16)) + state + 111111;
      } else {
        if (input_count == 2) {
          a += (input[i] | (input[i + 1] << 8)) + state + 1111;
        } else {
          a += input[i] + state + 11;
        }
      }
    }
  }

  a += _state;
  state += (a << 36) | (a >> 28);
  b += _state ^ state;
  state += (b << 38) | (b >> 26);
  c += _state + state;
  state += (c << 40) | (c >> 24);
  d += (a + b + c) ^ state;
  state += (d << 42) | (d >> 22);
  _state += state;
  b += a + state;
  state += (b << 28) | (b >> 36);
  c += b + (_state ^ state);
  state += (c << 30) | (c >> 34);
  d += c ^ (_state + state);
  state += (d << 34) | (d >> 30);
  a += _state;
  state += (a << 36) | (a >> 28);
  b += _state + state;
  state += (b << 38) | (b >> 26);
  c += _state ^ state;
  state += (c << 40) | (c >> 24);
  d += (a + b) ^ state;
  state += (d << 42) | (d >> 22);
  return (c + d) ^ (_state + state);
}

void wsp_hash_64_initialize(struct wsp_hash_64_s *s) {
  s->a = 1;
  s->b = 11;
  s->c = 111;
  s->d = 1111;
  s->_state = 1111111111;
  s->state = 11111111111;
}

void wsp_hash_64_transform(unsigned long i, unsigned long input_count,
                           const uint8_t *input,
                           struct wsp_hash_64_s *s) {
  uint64_t _a;
  uint64_t _b;
  uint64_t _c;
  uint64_t _d;

  if (input_count >= 32) {
    i = 31;

    while (i < input_count) {
      _a = _wsp_hash_read_64(input, i - 7);
      _b = _wsp_hash_read_64(input, i - 15);
      _c = _wsp_hash_read_64(input, i - 23);
      _d = _wsp_hash_read_64(input, i - 31);
      s->state += _a + _b + _c + _d;
      s->a += ((s->a << 30) | (s->a >> 34)) + s->state + _a + 1;
      s->b += ((s->b << 29) | (s->b >> 35)) + s->state + _b + 11;
      s->c += ((s->c << 28) | (s->c >> 36)) + _c + 111;
      s->d += ((s->d << 27) | (s->d >> 37)) + _d + 1111;
      i += 32;
    }

    if (i >= input_count) {
      i -= 32;
    }

    s->state += s->a + s->b + s->c + s->d;
    i++;
    input_count -= i;
  }

  if (input_count >= 16) {
    input_count -= 16;
    i += 16;
    _a = _wsp_hash_read_64(input, i - 16);
    _b = _wsp_hash_read_64(input, i - 8);
    s->state += _a + _b;
    s->a += ((s->a << 30) | (s->a >> 34)) + s->state + _a + 1;
    s->b += ((s->b << 29) | (s->b >> 35)) + s->state + _b + 11;
  }

  if (input_count >= 8) {
    input_count -= 8;
    i += 8;
    _a = _wsp_hash_read_64(input, i - 8);
    s->state += _a;
    s->a += ((s->a << 30) | (s->a >> 34)) + s->state + _a + 1;
  }

  if (input_count != 0) {
    s->state += ((s->a << 16) | (s->a >> 48)) + s->_state;

    if (input_count >= 4) {
      s->a += _wsp_hash_read_32(input, i) + s->state + 1111111111;

      if (input_count != 4) {
        s->_state += s->a + s->b;
        s->state += ((s->b << 20) | (s->b >> 44)) + s->_state;

        if (input_count == 7) {
          s->b += (input[i + 4] | (input[i + 5] << 8)
            | (input[i + 6] << 16)) + s->state + 111111111;
        } else {
          if (input_count == 6) {
            s->b += (input[i + 4] | (input[i + 5] << 8)) + s->state + 11111111;
          } else {
            s->b += input[i + 4] + s->state + 1111111;
          }
        }
      }
    } else {
      if (input_count == 3) {
        s->a += (input[i] | (input[i + 1] << 8)
          | (input[i + 2] << 16)) + s->state + 111111;
      } else {
        if (input_count == 2) {
          s->a += (input[i] | (input[i + 1] << 8)) + s->state + 1111;
        } else {
          s->a += input[i] + s->state + 11;
        }
      }
    }
  }
}

void wsp_hash_64_finalize(struct wsp_hash_64_s *s) {
  s->a += s->_state;
  s->state += (s->a << 36) | (s->a >> 28);
  s->b += s->_state ^ s->state;
  s->state += (s->b << 38) | (s->b >> 26);
  s->c += s->_state + s->state;
  s->state += (s->c << 40) | (s->c >> 24);
  s->d += (s->a + s->b + s->c) ^ s->state;
  s->state += (s->d << 42) | (s->d >> 22);
  s->_state += s->state;
  s->b += s->a + s->state;
  s->state += (s->b << 28) | (s->b >> 36);
  s->c += s->b + (s->_state ^ s->state);
  s->state += (s->c << 30) | (s->c >> 34);
  s->d += s->c ^ (s->_state + s->state);
  s->state += (s->d << 34) | (s->d >> 30);
  s->a += s->_state;
  s->state += (s->a << 36) | (s->a >> 28);
  s->b += s->_state + s->state;
  s->state += (s->b << 38) | (s->b >> 26);
  s->c += s->_state ^ s->state;
  s->state += (s->c << 40) | (s->c >> 24);
  s->d += (s->a + s->b) ^ s->state;
  s->state += (s->d << 42) | (s->d >> 22);
  s->state = (s->c + s->d) ^ (s->_state + s->state);
}
