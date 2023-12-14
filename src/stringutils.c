#include "stringutils.h"

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
