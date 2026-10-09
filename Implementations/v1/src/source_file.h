#pragma once

typedef struct {
  char* contents;
  size_t size; //! doesn't count '\0'
} Line;

typedef struct {
  size_t size;
  Line* lines;
} SourceFile;

/**
 * Reads an open file and formats it into a 'SourceFile' struct.
 * 
 * Obs.:
 *   Loads the entire file into memory (bad).
 *   Bad implementation of dynamic array.
 * 
 * Arg1: 'FILE' pointer to the open source file.
 * Return: SourceFile, an array of source-code lines.
 */
SourceFile read_source(FILE* source);

/**
 * Cleans up memory allocated for a 'SourceFile' struct.
 * Arg1: SourceFile struct.
 */
void free_source(SourceFile sf);

#ifdef SOURCE_FILE_IMPLEMENTATION

SourceFile read_source(FILE* source) {
  SourceFile sf = {0};

  char buffer[255];

  while (fgets(buffer, 255, source) != NULL) {

    // allocating new line
    size_t n_chars = strlen(buffer) - 1; // removing '\n';
    Line new_line = {
      .contents = malloc( (n_chars + 1) * sizeof(char) ), // Minimum length in bytes for current line: Char count + '\0'.
      .size = n_chars,
    };
    strncpy(new_line.contents, buffer, n_chars);

    // reallocating dynamic array
    sf.size += 1;
    Line* new_lines = malloc(sf.size * sizeof(Line));
    new_lines[sf.size - 1] = new_line;
    
    if (sf.lines == NULL) {
      sf.lines = new_lines;
      continue;
    }
    
    memcpy(new_lines, sf.lines, (sf.size - 1) * sizeof(Line));
    free(sf.lines);
    sf.lines = new_lines;
  }

  return sf;
}

void free_source(SourceFile sf) {
  for (size_t line_idx = 0; line_idx < sf.size; ++line_idx) {
    free(sf.lines[line_idx].contents);
  }
  free(sf.lines);
}

#endif
