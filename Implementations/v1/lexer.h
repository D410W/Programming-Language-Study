#pragma once

/**
 * Lexical elements:
 * 1. Control flow ('if', 'else')
 * 2. Declaration ('let', '=')
 * 3. Grammar symbols (';', ':', '{', '}', '(', ')')
 * 4. String and char delimiters ('"', '\'')
 * 5. Numbers ('1234', '5301', '0001', '-2')
 * 6. Identifiers ('num_1', 'foo', 'bar')
 * 7. Buildin operators ('-', '+', '*', '/', '==')
 */

typedef enum {
  TokenKind_CONTROL_FLOW,
  TokenKind_DECLARATION,
  TokenKind_GRAMMAR,
  TokenKind_SNC_DELIMETER,
  TokenKind_NUMBER,
  TokenKind_IDENTIFIER,
  TokenKind_OPERATOR,
} TokenKind;

typedef struct {
  TokenKind kind;
  char* value; //! Dynamically allocated
} Token;

typedef struct {
  size_t size;
  Token* tokens;
} TokenList;

/**
 * Reads through a 'SourceFile' and creates corresponding tokens in the language's syntax.
 * 
 * Arg1: 'SourceFile' struct which will be read.
 * Return: A list of tokens.
 */
TokenList lexical_analysis(SourceFile sf);

/**
 * Cleans up memory allocated for a 'TokenList' struct.
 * Arg1: TokenList struct.
 */
void free_token_list(TokenList tl);

#ifdef LEXER_IMPLEMENTATION

Token new_token_control_flow() {}
Token new_token_declaration() {}
Token new_token_grammar() {}
Token new_token_snc_delimeter() {}
Token new_token_number() {}
Token new_token_identifier() {}
Token new_token_operator() {}

TokenList lexical_analysis(SourceFile sf) {
  TokenList tl = {0};

  for (size_t line_idx = 0; line_idx < sf.size; ++line_idx) {
    Line line = sf.lines[line_idx];
    size_t curr_idx = 0;

    while (curr_idx < line.size) {
      size_t chars_processed = 1;
      
      // TODO

      curr_idx += chars_processed;
    }
  }
}

void free_token_list(TokenList tl) {
  for (size_t idx = 0; idx < tl.size; ++idx) {
    free(tl.tokens[idx].value);
  }
  free(tl.tokens);
}

#endif