#include "src/tidy-tree.h"
#include <stdlib.h>

int main(void) {
  char *path = "./test/corpus/simple.hmm";
  Arena *a = malloc(sizeof(Arena));
  arena_init(a);
  arena_load_content(a, path);
  arena_parse_content(a);
  tree_layout(a->root);
  arena_print(a);
  arena_free(a);
  return EXIT_SUCCESS;
}
