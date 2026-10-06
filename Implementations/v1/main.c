#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SOURCE_FILE_IMPLEMENTATION
#include "source_file.h"
#define LEXER_IMPLEMENTATION
#include "lexer.h"

int main(int argc, char** argv) {
  if (argc < 2) return 1; // TODO error logging

  FILE *source_file = fopen(argv[1], "r");
  if (source_file == NULL) return 2; // TODO error logging

  SourceFile sf = read_source(source_file);
  fclose(source_file);

  printf("%s\n", sf.lines[sf.size - 1].contents);

  free_source(sf);
  
  return 0;
}