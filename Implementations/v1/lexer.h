#pragma once

#include <string.h>
#include <stdlib.h>

/**
 * Lexical elements:
 * 1. Control flow ('if', 'else')
 * 2. Declaration ('let', '=')
 * 3. Grammar symbols (';', ':', '{', '}', '(', ')')
 * 4. String and char delimiters ('"', '\'')
 * 5. Built-in operators ('-', '+', '*', '/', '==', '!=')
 * 6. Numbers ('1234', '5301', '0001', '2')
 * 7. Identifiers ('num_1', 'foo', 'bar')
 */
typedef enum {
  TokenKind_CONTROL_FLOW,
  TokenKind_DECLARATION,
  TokenKind_GRAMMAR,
  TokenKind_SNC_DELIMETER,
  TokenKind_OPERATOR,
  TokenKind_NUMBER,
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

Token new_token_control_flow(char* start, size_t* token_length) {
  
}
Token new_token_declaration(char* start, size_t* token_length) {}
Token new_token_grammar(char* start, size_t* token_length) {}
Token new_token_snc_delimeter(char* start, size_t* token_length) {}
Token new_token_operator(char* start, size_t* token_length) {}
Token new_token_number(char* start, size_t* token_length) {}
Token new_token_identifier(char* start, size_t* token_length) {}

int is_token_number(char* start) {
  int length = 0;
  while (start[length] != '\0') {
    if ('0' <= start[0] && start[0] <= '9') {
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
        (start[0] == '_')) {
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
    if (strcmp(words[word_idx], start) != 0) continue; // If doesn't equal in size or characters, continue
    if (is_token_identifier(start) > strlen(words[word_idx])) continue; // If can be recognized as a string of greater length, continue
    
    return word_idx;
  }
  
  return -1;
}

int is_token_control_flow(char* start) {
  char* words[] = {
    "if", "else",
  };
  
  int match_result = match_specific_string(words, 2, start);
  return match_result;
}
int is_token_declaration(char* start) {
  char* words[] = {
    "let", "=",
  };
  
  int match_result = match_specific_string(words, 2, start);
  return match_result;
}
int is_token_grammar(char* start) {
  char* words[] = {
    ";", ":", "{", "}", "(", ")",
  };
  
  int match_result = match_specific_string(words, 2, start);
  return match_result;
}
int is_token_snc_delimeter(char* start) {
  char* words[] = {
    "\"", "'",
  };
  
  int match_result = match_specific_string(words, 2, start);
  return match_result;
}
int is_token_operator(char* start) {
  char* words[] = {
    "-", "+", "*", "/", "==", "!=",
  };
  
  int match_result = match_specific_string(words, 2, start);
  return match_result;
}

void append_token(TokenList* tl, Token token) {
  
}

TokenList lexical_analysis(SourceFile sf) {
  TokenList tl = {0};

  for (size_t line_idx = 0; line_idx < sf.size; ++line_idx) {
    Line line = sf.lines[line_idx];
    size_t curr_idx = 0;

    while (curr_idx < line.size) {
      size_t chars_processed = 1;
      
      // Check fixed length tokens
      if (is_token_control_flow(line[curr_idx]) != -1) {
        size_t token_length;
        Token new_t = new_token_control_flow(line + curr_idx, &token_length);
        
        append_token(&tl, new_t);
      } else if (is_token_declaration() != -1) {
        
      } else if (is_token_grammar() != -1) {
        
      } else if (is_token_snc_delimeter() != -1) {
        
      } else if (is_token_operator() != -1) {
      
      } else if (is_token_number() != -1) {
        
      } else if (is_token_identifier() != -1) {
        
      }
      
      // Check variable length tokens (identifiers and numbers)

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
