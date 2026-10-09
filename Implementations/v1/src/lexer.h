#pragma once

#include <string.h>
#include <stdlib.h>

#include "common.h"

/**
 * Lexical elements:
 * 1. Control flow ('if', 'else')
 * 2. Declaration ('let', '=')
 * 3. Grammar symbols (';', ':', '{', '}', '(', ')')
 * 4. String and char delimiters ('"', '\'')
 * 5. Built-in operators ('-', '+', '*', '/', '==', '!=')
 * 6. Comment ('//', '/*', '* /')
 * 7. Number ('1234', '5301', '0001', '2')
 * 8. Identifier ('num_1', 'foo', 'bar')
 */
typedef enum {
  TokenKind_CONTROL_FLOW = 0,
  TokenKind_DECLARATION,
  TokenKind_GRAMMAR,
  TokenKind_SNC_DELIMETER,
  TokenKind_OPERATOR,
  TokenKind_NUMBER,
  TokenKind_COMMENT,
  TokenKind_IDENTIFIER,
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

char* control_flow_tokens[] = { "if", "else" };
Token new_token_control_flow(char* start, size_t* token_length) {
  size_t length = 0;
  
  Token new_t = {
    .kind = TokenKind_CONTROL_FLOW,
    .value = NULL
  };
  
  if (start[0] == 'i') {
    length = 2;
    
    new_t.value = malloc((length+1) * sizeof(char));
    strncpy(new_t.value, "if", length+1);
  } else {
    length = 4;
    
    new_t.value = malloc((length+1) * sizeof(char));
    strncpy(new_t.value, "else", length+1);
  }
  
  if (token_length != NULL) *token_length = length;
  return new_t;
}
char* declaration_tokens[] = { "let", "=" };
Token new_token_declaration(char* start, size_t* token_length) {
  size_t length = 0;
  
  Token new_t = {
    .kind = TokenKind_DECLARATION,
    .value = NULL
  };
  
  if (start[0] == 'l') {
    length = 3;
    
    new_t.value = malloc((length+1) * sizeof(char));
    strncpy(new_t.value, "let", length+1);
  } else {
    length = 1;
    
    new_t.value = malloc((length+1) * sizeof(char));
    strncpy(new_t.value, "=", length+1);
  }
  
  if (token_length != NULL) *token_length = length;
  return new_t;
}
char* grammar_tokens[] = { ";", ":", "{", "}", "(", ")" };
Token new_token_grammar(char* start, size_t* token_length) {
  size_t length = 1;
  
  Token new_t = {
    .kind = TokenKind_GRAMMAR,
    .value = malloc((length+1) * sizeof(char))
  };
  
  strncpy(new_t.value, start, length);
  new_t.value[1] = '\0';
  
  if (token_length != NULL) *token_length = length;
  return new_t;
}
char* snc_delimeter_tokens[] = { "\"", "'" };
Token new_token_snc_delimeter(char* start, size_t* token_length) {
  size_t length = 1;
  
  Token new_t = {
    .kind = TokenKind_SNC_DELIMETER,
    .value = malloc((length+1) * sizeof(char))
  };
  
  strncpy(new_t.value, start, length);
  new_t.value[1] = '\0';
  
  if (token_length != NULL) *token_length = length;
  return new_t;
}
char* operator_tokens[] = { "-", "+", "*", "/", "==", "!=" };
Token new_token_operator(char* start, size_t* token_length) {
  size_t length = 1;
  
  if (start[0] == '=' || start[0] == '!') {
    length = 2;
  }
  
  Token new_t = {
    .kind = TokenKind_OPERATOR,
    .value = malloc((length+1) * sizeof(char))
  };
  
  strncpy(new_t.value, start, length);
  new_t.value[length] = '\0';
  
  if (token_length != NULL) *token_length = length;
  return new_t;
}
char* comment_tokens[] = { "//", "/*", "*/" };
Token new_token_comment(char* start, size_t* token_length) {
  size_t length = 2;
  
  Token new_t = {
    .kind = TokenKind_COMMENT,
    .value = malloc((length+1) * sizeof(char))
  };
  
  strncpy(new_t.value, start, length);
  new_t.value[length] = '\0';
  
  if (token_length != NULL) *token_length = length;
  return new_t;
}
Token new_token_number(char* start, size_t token_length) {
  Token new_t = {
    .kind = TokenKind_NUMBER,
    .value = malloc((token_length+1) * sizeof(char))
  };
  
  strncpy(new_t.value, start, token_length);
  new_t.value[token_length] = '\0';
  
  return new_t;
}
Token new_token_identifier(char* start, size_t token_length) {
  Token new_t = {
    .kind = TokenKind_IDENTIFIER,
    .value = malloc((token_length+1) * sizeof(char))
  };
  
  strncpy(new_t.value, start, token_length);
  new_t.value[token_length] = '\0';
  
  return new_t;
}

int is_token_number(char* start) {
  int length = 0;
  while (start[length] != '\0') {
    if ('0' <= start[length] && start[length] <= '9') {
      length += 1;
    } else {
      break;
    }
  }
  
  if (length > 0) {
    return length;
  } else {
    return -1;
  }
}

int is_token_identifier(char* start) {
  // doesn't start with a digit
  if ('0' <= start[0] && start[0] <= '9') return -1;

  int length = 0;
  while (start[length] != '\0') {
    if (('a' <= start[length] && start[length] <= 'z') ||
        ('A' <= start[length] && start[length] <= 'Z') ||
        ('0' <= start[length] && start[length] <= '9') ||
        (start[length] == '_')) {
      length += 1;
    } else {
      break;
    }
  }
  
  if (length > 0) {
    return length;
  } else {
    return -1;
  }
}

int match_specific_string(char** words, int word_count, char* start) {
  for (int word_idx = 0; word_idx < word_count; ++word_idx) {
    if (strncmp(words[word_idx], start, strlen(words[word_idx])) != 0) continue; // If doesn't equal in size or characters, continue
    
    int token_idt = is_token_identifier(start);
    if (token_idt != -1 && (size_t)token_idt > strlen(words[word_idx])) continue; // If can be recognized as an identifier of greater length, continue
    
    return word_idx;
  }
  
  return -1;
}

int is_token_control_flow(char* start) {
  char** words = control_flow_tokens;
  
  int match_result = match_specific_string(words, 2, start);
  return match_result;
}
int is_token_declaration(char* start) {
  char** words = declaration_tokens;
  
  int match_result = match_specific_string(words, 2, start);
  return match_result;
}
int is_token_grammar(char* start) {
  char** words = grammar_tokens;
  
  int match_result = match_specific_string(words, 6, start);
  return match_result;
}
int is_token_snc_delimeter(char* start) {
  char** words = snc_delimeter_tokens;
  
  int match_result = match_specific_string(words, 2, start);
  return match_result;
}
int is_token_operator(char* start) {
  char** words = operator_tokens;
  
  int match_result = match_specific_string(words, 6, start);
  return match_result;
}
int is_token_comment(char* start) {
  char** words = comment_tokens;
  
  int match_result = match_specific_string(words, 3, start);
  return match_result;
}
int is_ignore_character(char* start) {
  char* words[] = {" ", "\r"};
  
  int match_result = match_specific_string(words, 2, start);
  return match_result;
}

void append_token(TokenList* tl, Token token) {
  tl->size += 1;
  tl->tokens = realloc(tl->tokens, tl->size * sizeof(Token));
  tl->tokens[tl->size - 1] = token;
}

typedef enum {
  LexerState_CODE = 0,
  LexerState_STRING,
  LexerState_COMMENT,
} LexerState;

TokenList lexical_analysis(SourceFile sf) {
  TokenList tl = {0};

  for (size_t line_idx = 0; line_idx < sf.size; ++line_idx) {
    Line line = sf.lines[line_idx];
    size_t curr_idx = 0;

    while (curr_idx < line.size) {
      void* fixed_length_token_checkers[] = {
        &is_token_control_flow, &is_token_declaration, &is_token_grammar,
        &is_token_snc_delimeter, &is_token_operator,
      };
      void* fixed_length_constructors[] = {
        &new_token_control_flow, &new_token_declaration, &new_token_grammar,
        &new_token_snc_delimeter, &new_token_operator,
      };
      
      void* var_length_token_checkers[] = {
        &is_token_number, &is_token_identifier,
      };
      void* var_length_constructors[] = {
        &new_token_number, &new_token_identifier,
      };
      
      int (*is_token)(char*);
      Token (*new_fix_token)(char*, size_t*);
      Token (*new_var_token)(char*, size_t);
      
      char* current_str = line.contents + curr_idx;
      size_t chars_processed = (is_ignore_character(current_str) != -1);
      
      // Check fixed length tokens
      if (chars_processed == 0) {
        for (size_t token_type = 0; token_type < arrlen(fixed_length_token_checkers); ++token_type) {
          is_token = fixed_length_token_checkers[token_type];
          new_fix_token = fixed_length_constructors[token_type];
          
          if ((*is_token)(current_str) != -1) {
            Token new_t = (*new_fix_token)(current_str, &chars_processed);
            append_token(&tl, new_t);
            break;
          }
        }
      }
      
      // Check variable length tokens (identifiers and numbers)
      if (chars_processed == 0) {
        for (size_t token_type = 0; token_type < arrlen(var_length_token_checkers); ++token_type) {
          is_token = var_length_token_checkers[token_type];
          new_var_token = var_length_constructors[token_type];
          
          if (chars_processed = (*is_token)(current_str),
              chars_processed != -1) {
            Token new_t = (*new_var_token)(current_str, chars_processed);
            append_token(&tl, new_t);
            break;
          }
        }
      }
      
      // doesn't recognize token
      if (chars_processed == 0) {
        if (32 <= line.contents[curr_idx] && line.contents[curr_idx] <= 126) { // is visible
          printf("Unrecognized token starting with char '%c', position %zu, %zu\n", line.contents[curr_idx], line_idx+1, curr_idx+1);
        } else {
          printf("Unrecognized token starting with char value '%i', position %zu, %zu\n", line.contents[curr_idx], line_idx+1, curr_idx+1);
        }
        
        return tl;
      }
      
      curr_idx += chars_processed;
    }
  }
  
  return tl;
}

void free_token_list(TokenList tl) {
  for (size_t idx = 0; idx < tl.size; ++idx) {
    free(tl.tokens[idx].value);
  }
  free(tl.tokens);
}

#endif
