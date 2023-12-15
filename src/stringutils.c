#include "stringutils.h"
#include <stdlib.h>

// String functions
int s_get_indent(char *line) {
  int indent = 0;
  for (int i = 0; i < strlen(line); i++) {
    if (line[i] == '\t') {
      indent++;
    }
  }
  return indent;
}

char *s_get_substring(char *line, u64 start, u64 end) {
  int len = end - start;
  char *s = malloc(sizeof(char) * (len + 1));
  for (int i = 0; i < len; i++) {
    s[i] = line[start + i];
  }
  s[len] = '\0';
  return s;
}
