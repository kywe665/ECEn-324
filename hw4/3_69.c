typedef struct ELE *tree_ptr;

struct ELE {
  long val;
  long long dummy;
  tree_ptr left;
  tree_ptr right;
};

long trace(tree_ptr tp) {
  struct ELE* current = tp;
  if(!current) {
    return 0;
  }
  else{
    while(current->val != 0) {
      current = current->right;
    }
    return current->val;
  }
}

