#pragma once

#include <string.h>
#include <stdlib.h>

#include "lexer.h"
#include "common.h"

typedef enum {
  SyntaxNodeKind_TOKEN = 0,
  SyntaxNodeKind_EXPRESSION,
} SyntaxNodeKind;

typedef struct SyntaxNode {
  SyntaxNodeKind syntax_kind;
  Token token;
  
  struct SyntaxNode* children[5];
  size_t children_count;
} SyntaxNode;

typedef struct {
  SyntaxNode* root;
} SyntaxTree;

SyntaxTree build_syn_tree(TokenList tl);

void free_syntax_tree(SyntaxTree st);

#ifdef PARSER_IMPLEMENTATION

void node_add_child(SyntaxNode* node, SyntaxNode* child) {
  if (node->children_count >= 5) {
    printf("Error. Can't add a child to SyntaxNode since it's full. %zu\n", node->children_count);
    return;
  }
  
  node->children[node->children_count] = child;
  node->children_count += 1;
}

SyntaxNode* new_token_node(TokenList tl, size_t position) {
  SyntaxNode* new_node = malloc(sizeof(SyntaxNode));
  *new_node = (SyntaxNode){
    .syntax_kind = SyntaxNodeKind_TOKEN,
    .token = tl.tokens[position],
  };
  
  tl.tokens[position].value = NULL;
  
  return new_node;
}

SyntaxNode* new_expression_node(TokenList tl, size_t start) {
  SyntaxNode* new_node = malloc(sizeof(SyntaxNode));
  *new_node = (SyntaxNode){
    .syntax_kind = SyntaxNodeKind_EXPRESSION,
  };
  
  if (tl.tokens[start].categ != TokenKindCateg_NUMBER) {
    printf("Error.\n");
    return new_node;
  }
  node_add_child(new_node, new_token_node(tl, start));
  printf("Added number %s.\n", new_node->children[new_node->children_count - 1]->token.value);
  
  start += 1;
  if (tl.size == start || tl.tokens[start].type == TokenKind_SEMICOLON) { // No token after this number. Return single number
    return new_node;
  }
  
  if (tl.tokens[start].categ != TokenKindCateg_OPERATOR) {
    printf("Error.\n");
    return new_node;
  }
  node_add_child(new_node, new_token_node(tl, start));
  printf("Added operator %s.\n", new_node->children[new_node->children_count - 1]->token.value);
  
  start += 1;
  if (tl.size == start || tl.tokens[start].type == TokenKindCateg_NUMBER) { // No token after this number. Return single number
    printf("Error.\n");
    return new_node;
  }
  node_add_child(new_node, new_expression_node(tl, start));
  
  return new_node;
}

SyntaxTree build_syn_tree(TokenList tl) {
  SyntaxTree tree = {};
  
  tree.root = new_expression_node(tl, 0);
}

void recursive_node_free(SyntaxNode* node) {
  if (node->children != NULL) {
    for (size_t i = 0; i < node->children_count; ++i) {
      recursive_node_free(node->children[i]);
    }
  }
  
  free(node->token.value);
  free(node);
}

void free_syntax_tree(SyntaxTree st) {
  recursive_node_free(st.root);
}

#endif
