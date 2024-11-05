# WSP-Hash-64
© 2024 William Stafford Parsons

## About
WSP-Hash-64 is a 64-bit hashing algorithm as a reasonable alternative to CityHash64, 128-bit MurmurHash3 x64 and x86, 64-bit SipHash and 64-bit SpookyHashV2.

Read more [here](https://williamstaffordparsons.github.io/wsp-hash-64/).

## Example
``` c
#include <stdio.h>
#include "wsp_hash_64.h"

int main(void) {
  struct wsp_hash_64_s s;
  uint8_t input[8] = {'m', 'e', 's', 's', 'a', 'g', 'e', 0};
  unsigned char i = 0;

  while (i != 10) {
    i++;
    printf("All-at-once result %u is 0x%016lx.\n", i,
           wsp_hash_64(8, (const uint8_t *) input));
    input[7]++;
  }

  input[7] = 0;
  i = 0;

  while (i != 10) {
    i++;
    wsp_hash_64_initialize(&s);
    wsp_hash_64_transform(0, 8, (const uint8_t *) input, &s);
    wsp_hash_64_finalize(&s);
    input[7]++;
    printf("Segmented result %u is 0x%016lx.\n", i, s.state);
  }

  return 0;
}
```
