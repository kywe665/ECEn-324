int main() {
  
  int loop(int x, int n) {
    int result = 1 & x;
    int mask;
    for(mask = 1; mask > 0; mask << 1) {
      result ^= -1;
    }
    return result;
  }
}
