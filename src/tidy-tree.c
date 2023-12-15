#include "tidy-tree.h"
#include "stringutils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void arena_init(Arena *a) {
  a->len = 0;
  a->capacity = INIT_CAPACITY;
  a->root = malloc(sizeof(Tree));
  a->nodes = malloc(sizeof(Tree *) * a->capacity);
  for (int i = 0; i < a->capacity; i++) {
    a->nodes[i] = malloc(sizeof(Tree));
  }
}

void arena_free(Arena *a) {
  for (int i = 0; i < a->capacity; i++) {
    free(a->nodes[i]);
  }
  free(a->nodes);
  free(a->root);
}

Tree *arena_node_at_index(Arena *a, int i) {
  // Do we need to distinguish?
  if (i < a->len) {
    return a->nodes[i];
  } else if (i < a->capacity) {
    return a->nodes[i];
  } else {
    return NULL;
  }
}

void arena_double_capacity(Arena *a) {
  a->nodes = realloc(a->nodes, sizeof(Tree *) * a->capacity * 2);
  if (a->nodes == NULL) {
    die("Could not reallocate");
  }
  for (int i = a->capacity; i < a->capacity * 2; i++) {
    a->nodes[i] = malloc(sizeof(Tree));
  }
  a->capacity *= 2;
}

void arena_shrink_to_len(Arena *a) {
  for (int i = a->len; i < a->capacity; i++) {
    free(a->nodes[i]);
  }
  a->nodes = realloc(a->nodes, sizeof(Tree *) * a->len);
  a->capacity = a->len;
}

int arena_can_insert(Arena *a) { return (a->len < a->capacity); }

void iyl_new(IYL *ih, double lowY, int index, IYL *nxt) {
  ih->lowY = lowY;
  ih->index = index;
  ih->nxt = nxt;
}

void iyl_free(IYL *ih) {
  if (ih) {
    free(ih);
  }
}

void iyl_update(double minY, int i, IYL *ih, IYL *out) {
  while ((ih != NULL) && (minY >= ih->lowY)) {
    ih = ih->nxt;
  }
  iyl_new(out, minY, i, ih);
}

void tree_new(Tree *t, double w, double h, double y, char *line, int index,
              u64 sb, u64 eb) {
  t->w = w;
  t->h = h;
  t->y = y;
  t->x = 0;
  t->prelim = 0;
  t->mod = 0;
  t->shift = 0;
  t->change = 0;
  t->cs = 0;
  t->p = NULL;
  t->c = NULL;
  t->tl = NULL;
  t->tr = NULL;
  t->el = NULL;
  t->er = NULL;
  t->msel = 0;
  t->mser = 0;

  // User data
  t->sb = sb;
  t->eb = eb;
  t->index = index;
}

void tree_print(Tree *t, char *content) {
  char *line = s_get_substring(content, t->sb, t->eb);
  printf("%s: \n"
         "     x,      y: %3.0f, %3.0f\n"
         "     w,      h: %3.0f, %3.0f\n"
         "    cs,      c: %3d, %p\n"
         "prelim,    mod: %3.0f, %3.0f\n"
         " shift, change: %3.0f, %3.0f\n"
         "  msel,   mser: %3.0f, %3.0f\n"
         "    tl,     tr: %p, %p\n"
         "    el,     er: %p, %p\n",
         line, t->x, t->y, t->w, t->h, t->cs, t->c, t->prelim, t->mod, t->shift,
         t->change, t->msel, t->mser, t->tl, t->tr, t->el, t->er);
  free(line);
}

void tree_free(Tree *t) {
  if (t->cs > 0) {
    for (int i = 0; i < t->cs; i++) {
      free(t->c[i]);
    }
  }
}

void tree_layout(Tree *t) {
  tree_first_walk(t);
  // tree_second_walk(t, 0);
}

void tree_first_walk(Tree *t) {
  if (t->cs == 0) {
    tree_set_extremes(t);
    return;
  }
  tree_first_walk(t->c[0]);

  // IYL *ih = malloc(sizeof(IYL));
  // IYL_update(tree_bottom(t->c[0]->el), 0, NULL, ih);

  // for (int i = 1; i < t->cs; i++) {
  //   tree_first_walk(t->c[i]);
  //   double minY = tree_bottom(t->c[i]->er);
  //   tree_separate(t, i, ih);
  //   IYL_update(minY, i, ih, ih);
  // }
  //
  // tree_position_root(t);
  tree_set_extremes(t);
}

void tree_set_extremes(Tree *t) {
  if (t->cs == 0) {
    t->el = t;
    t->er = t;
    t->msel = t->mser = 0;
  } else {
    t->el = t->c[0]->el;
    t->msel = t->c[0]->msel;
    t->er = t->c[t->cs - 1]->er;
    t->mser = t->c[t->cs - 1]->mser;
  }
}

// void tree_second_walk(Tree *t, double modsum) {
//   modsum += t->mod;
//   // Set absolute (non-relative) horizontal coordinate
//   t->x = t->prelim + modsum;
//   tree_add_child_spacing(t);
//   for (int i = 0; i < t->cs; i++) {
//     tree_second_walk(t->c[i], modsum);
//   }
// }

long read_size_from_file(char *path) {
  FILE *fp;
  long size = 0;
  fp = fopen(path, "rb");
  if (fp == NULL) {
    return 0;
  }
  fseek(fp, 0L, SEEK_END);
  size = ftell(fp) + 1;
  fclose(fp);
  return size;
}

void arena_load_content(Arena *a, char *path) {
  long size = read_size_from_file(path);
  a->nbytes = size;

  FILE *fp;
  fp = fopen(path, "r");
  a->content = (char *)memset(malloc(size), '\0', size);
  fread(a->content, 1, size - 1, fp);
  fclose(fp);
}

void arena_parse_content(Arena *a) {
  // Set the root
  tree_new(a->root, 0, 0, -1, "root", -1, 0,
           a->nbytes); // Set indent and index to -1

  // Iterate over lines, only allocate on overflow
  const char *line_delim = "\n";
  char *rest;
  int indent;
  int count = 0;
  int max_indent = 0;
  u64 sb = 0;
  u64 eb = 0;
  char *line = strtok_r(a->content, line_delim, &rest);
  while (line != NULL) {
    if (!arena_can_insert(a)) {
      arena_double_capacity(a);
    }
    indent = s_get_indent(line);
    if (indent > max_indent) {
      max_indent = indent;
    }
    eb = sb + strlen(line); // Swapping \0 for \n, so count works out hopefully
    tree_new(a->nodes[count], strlen(line), 1, indent, line, count, sb + indent,
             eb);
    sb = eb + 1;
    line = strtok_r(NULL, line_delim, &rest);
    count++;
    a->len = count;
  }
  a->max_indent = max_indent;

  // Reset arena size to free excess
  arena_shrink_to_len(a);

  // Put into tree structure
  Tree **indent_last = malloc(sizeof(Tree *) * (max_indent + 1));
  for (int i = 0; i < a->len; i++) {
    // This is the last seen node at current index
    indent_last[(int)a->nodes[i]->y] = a->nodes[i];

    // Ignore if indent == 0 ( no parents )
    if (a->nodes[i]->y == 0) {
      continue;
    }

    // Attach to parent if indent > 1
    Tree *p = indent_last[(int)a->nodes[i]->y - 1];
    p->cs += 1;
    p->c = realloc(p->c, sizeof(Tree *) * p->cs);
    p->c[p->cs - 1] = a->nodes[i];
    a->nodes[i]->p = p;
  }
  free(indent_last);
}

void arena_print(Arena *a) {
  printf("len: %d, capacity: %d, max_indent: %d, nbytes: %ld\n", a->len,
         a->capacity, a->max_indent, a->nbytes);
  printf("|-----|-----|-----|-------|-------|---------\n");
  printf("|  i  |  y  |  cs |   sb  |   eb  | content \n");
  printf("|-----|-----|-----|-------|-------|---------\n");
  for (int i = 0; i < a->len; i++) {
    char *l = s_get_substring(a->content, a->nodes[i]->sb, a->nodes[i]->eb);
    printf("| %3d | %3.0f | %3d | %5ld | %5ld | %s \n", i, a->nodes[i]->y,
           a->nodes[i]->cs, a->nodes[i]->sb, a->nodes[i]->eb, l);
    free(l);
  }
}
