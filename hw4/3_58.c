int main() {
  typedef enum{MODE_A, MODE_B, MODE_C, MODE_D, MODE_E} mode_t;
  int switch3(int *p1, int *p2, mode_t action) {
    int result = 0;
    switch(action) {
      case MODE_A:
        result = *p1;
      case MODE_B:
        result = *p1;
      case MODE_C:
        result = *p1;
      case MODE_D:
        result = *p1;
      case MODE_E:
        result = 17;
      default:
        result = result;
    }
    return result;
  }
}
