#include <stdio.h>

int main(void)
{
  int x = 5;
  int zero = 0x0;
  int flip = (~0x0);
  printf("%x, %x\n", zero, flip);
  int value = ((x+ ~0x0) >> 31);
  int sign = x >> 31;
  printf("%x, %x\n", sign, value);
  int ans = (sign ^ value) & value;
  printf("%x, %d\n", ans, ans);
}

