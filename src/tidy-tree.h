#include <stdio.h>
#include <string.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

// Utility macros
#define die(msg)                                                               \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

typedef struct IYL IYL;
struct IYL {
  double lowY;
  int index;
  IYL *nxt;
};
IYL *IYL_new(double lowY, int index, IYL *nxt);
void IYL_free(IYL *ih);
void IYL_update(double minY, int i, IYL *ih, IYL *out);

typedef struct Tree Tree;
struct Tree {
  double w, h; // Width and height
  double x, y, prelim, mod, shift, change;
  Tree *tl, *tr;     // Left and right thread
  Tree *el, *er;     // Extreme left and right nodes
  double msel, mser; // Sum of modifiers at the extreme nodes
  Tree **c;          // Array of children and number of children
  int cs;
  char *line;
  int index;
};

Tree *tree_new(double w, double h, double y, char *line, int index);
void tree_set_children(Tree **c, int cs);
void tree_free(Tree *t);
void tree_print(Tree *t);

void tree_layout(Tree *t);
void tree_first_walk(Tree *t);
void tree_set_extremes(Tree *t);
void tree_separate(Tree *t, int i, IYL *ih);
void tree_move_subtree(Tree *t, int i, int si, double dist);
Tree *tree_next_left_contour(Tree *t);
Tree *tree_next_right_contour(Tree *t);
double tree_bottom(Tree *t);
void tree_set_left_thread(Tree *t, int i, Tree *cl, double modsumcl);
void tree_set_right_thread(Tree *t, int i, Tree *sr, double modsumsr);
void tree_position_root(Tree *t);
void tree_second_walk(Tree *t, double modsum);
void tree_distribute_extra(Tree *t, int i, int si, double dist);
void tree_add_child_spacing(Tree *t);

char *read_text_from_file(char *path);
Tree *tree_load_from_text(char *content);
