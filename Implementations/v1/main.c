#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  size_t size;
  char** lines;
} SourceFile;

/**
 * Reads an open file and formats it into a 'SourceFile' struct.
 * 
 * Obs.:
 *   Loads the entire file into memory (bad).
 *   Bad implementation of dynamic array.
 * 
 * Arg1: 'FILE' pointer to the open source file.
 */
SourceFile read_source(FILE* source) {
  SourceFile sf = {
    0,
    NULL,
  };

  char buffer[255];

  while (fgets(buffer, 255, source) != NULL) {

    // allocating new line
    size_t n_chars = (strlen(buffer) + 1);
    char* new_line = malloc( n_chars * sizeof(char) ); // Minimum length in bytes for current line: Char count + '\0'.
    strncpy(new_line, buffer, n_chars);

    // reallocating dynamic array
    sf.size += 1;
    char** new_lines = malloc(sf.size * sizeof(char*));
    new_lines[sf.size - 1] = new_line;
    
    if (sf.lines == NULL) {
      sf.lines = new_lines;
      continue;
    }
    
    memcpy(new_lines, sf.lines, (sf.size - 1) * sizeof(char*));
    free(sf.lines);
    sf.lines = new_lines;
  }

  return sf;
}

/**
 * Cleans up memory allocated for a 'SourceFile' struct.
 * Arg1: SourceFile struct.
 */
void close_source(SourceFile sf) {
  for (size_t line_idx = 0; line_idx < sf.size; line_idx += 1) {
    free(sf.lines[line_idx]);
  }
  free(sf.lines);
}

int main(int argc, char** argv) {
  if (argc < 2) return 1;

  FILE *source_file = fopen(argv[1], "r");
  if (source_file == NULL) return 2;

  SourceFile sf = read_source(source_file);
  fclose(source_file);

  printf("%s\n", sf.lines[sf.size - 1]);

  close_source(sf);
  
  return 0;
}