#include "src/tidy-tree.h"
#include <stdlib.h>

int main(void) {
  char *content = read_text_from_file("./test/corpus/simple.hmm");

  Tree *t = tree_load_from_text(content);

  tree_layout(t);
  tree_print(t);

  tree_free(t);
  return EXIT_SUCCESS;
}
