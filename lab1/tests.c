/* Testing Code */

#include <limits.h>
int test_bitAnd(int x, int y)
{
  return x&y;
}
int test_bitNor(int x, int y)
{
  return ~(x|y);
}
int test_copyLSB(int x)
{
  return (x & 0x1) ? -1 : 0;
}
int test_evenBits(void) {
  int result = 0;
  int i;
  for (i = 0; i < 32; i+=2)
    result |= 1<<i;
  return result;
}
int test_logicalShift(int x, int n) {
  unsigned u = (unsigned) x;
  unsigned shifted = u >> n;
  return (int) shifted;
}





int test_bang(int x)
{
  return !x;
}
int test_leastBitPos(int x) {
  int mask = 1;
  if (x == 0)
    return 0;
  while (!(mask & x)) {
    mask = mask << 1;
  }
  return mask;
}
int test_isNotEqual(int x, int y)
{
  return x != y;
}
int test_negate(int x) {
  return -x;
}
int test_isPositive(int x) {
  return x > 0;
}
int test_isNonNegative(int x) {
  return x >= 0;
}
int test_sum3(int x, int y, int z)
{
  return x+y+z;
}
int test_addOK(int x, int y)
{
  int sum = x+y;
  return !(x < 0 && y < 0 && sum >= 0) && !(x > 0 && y > 0 && sum <= 0);
}
int test_abs(int x) {
  return (x < 0) ? -x : x;
}
int test_isNonZero(int x)
{
  return x!=0;
}
