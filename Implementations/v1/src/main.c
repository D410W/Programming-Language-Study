#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SOURCE_FILE_IMPLEMENTATION
#include "source_file.h"
#define LEXER_IMPLEMENTATION
#include "lexer.h"
#define PARSER_IMPLEMENTATION
#include "parser.h"

int main(int argc, char** argv) {
  if (argc < 2) return 1; // TODO error logging

  FILE *source_file = fopen(argv[1], "r");
  if (source_file == NULL) return 2; // TODO error logging

  SourceFile sf = read_source(source_file);
  fclose(source_file);

  TokenList tl = lexical_analysis(sf, false);
  free_source(sf);
  
  for (size_t i = 0; i < tl.size; ++i) {
    char buf[256];
    tokenkind_to_str(buf, 256, tl.tokens[i].type);
    printf("%s %s\n", buf, tl.tokens[i].value);
  }
  
  SyntaxTree st = build_syn_tree(tl);
  free_token_list(tl);
  
  int test(void) {
    return 2;
  }
  
  free_syntax_tree(st);
  
  return 0;
}
