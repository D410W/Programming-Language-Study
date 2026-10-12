#pragma once

#include <string.h>
#include <stdlib.h>

#include "lexer.h"
#include "common.h"

typedef enum {
  SyntaxNodeType_TOKEN = 0,
} SyntaxNodeType;

typedef struct {
  SyntaxNodeType type;
} SyntaxNode;

typedef struct {
  
} ConcreteSynTree;

#ifdef PARSER_IMPLEMENTATION
#endif
