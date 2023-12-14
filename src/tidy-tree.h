#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef uint8_t u8;
typedef int32_t b32;
typedef int32_t i32;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float f32;
typedef double f64;
typedef uintptr_t uptr;
typedef char byte;

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define INIT_CAPACITY 16

// Utility macros
#define die(msg)                                                               \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

// Declare types at start
typedef struct Arena Arena;
typedef struct IYL IYL;
typedef struct Tree Tree;

// Holds data
struct Arena {
  u32 len, capacity;
  Tree *root;
  Tree **nodes;
};
void arena_init(Arena *a);
void arena_free(Arena *a);
Tree *arena_node_at_index(Arena *a, int i);
void arena_double_capacity(Arena *a);
void arena_shrink_to_len(Arena *a);
int arena_can_insert(Arena *a);

struct IYL {
  f64 lowY;
  int index;
  IYL *nxt;
};
IYL *IYL_new(f64 lowY, int index, IYL *nxt);
void IYL_free(IYL *ih);
void IYL_update(f64 minY, int i, IYL *ih, IYL *out);

struct Tree {
  f64 w, h; // Width and height
  f64 x, y, prelim, mod, shift, change;
  Tree *tl, *tr;  // Left and right thread
  Tree *el, *er;  // Extreme left and right nodes
  f64 msel, mser; // Sum of modifiers at the extreme nodes
  Tree *p;        // Parent
  Tree **c;       // Array of children and number of children
  int cs;
  char *line; // Should actually be start and end byte into content
  int index;
};

void tree_new(Tree *t, double w, double h, double y, char *line, int index);
void tree_set_children(Tree **c, int cs);
void tree_free(Tree *t);
void tree_print(Tree *t);

void tree_layout(Tree *t);
void tree_first_walk(Tree *t);
void tree_set_extremes(Tree *t);
void tree_separate(Tree *t, int i, IYL *ih);
void tree_move_subtree(Tree *t, int i, int si, f64 dist);
Tree *tree_next_left_contour(Tree *t);
Tree *tree_next_right_contour(Tree *t);
f64 tree_bottom(Tree *t);
void tree_set_left_thread(Tree *t, int i, Tree *cl, f64 modsumcl);
void tree_set_right_thread(Tree *t, int i, Tree *sr, f64 modsumsr);
void tree_position_root(Tree *t);
void tree_second_walk(Tree *t, f64 modsum);
void tree_distribute_extra(Tree *t, int i, int si, f64 dist);
void tree_add_child_spacing(Tree *t);

char *read_text_from_file(char *path);
Tree *tree_load_from_text(char *content);
void tree_load_to_arena(Arena *a, char *content);
