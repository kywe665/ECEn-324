int main() {
  int decode(int x, int y, int z) {
    return ( x*(y-z) )^ ( ((y-z)<<31) >>31 );
  }
  return 0;
}
