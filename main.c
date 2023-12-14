#include "src/tidy-tree.h"
#include <stdlib.h>

int main(void) {
  char *content = read_text_from_file("./test/corpus/simple.hmm");

  Arena *a = malloc(sizeof(Arena));
  arena_init(a);
  tree_load_to_arena(a, content);

  tree_layout(a->root);
  tree_print(a->root);

  arena_free(a);

  return EXIT_SUCCESS;
}
