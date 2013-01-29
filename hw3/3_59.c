int main(int x, int n) {
  
  int switch_prob(int x, int n) {
    int result = x;
    switch(n) {

      case 1:
        result = x<<2;
        break;

      case 2:
        result = x + 10;
        break;

      case 3:
        result = x << 2;
        break;

      case 4:
        result = (x >> 10);
        break;

      case 5:
        result = x*(2*x+2) + 10;
        break;

      case 6:
        result = x*x + 10;
        break;

      default:
        if(n-32 > 5) {
          result = x+10;
        }
    }
    return result;
  }
  
  return 0;
}
