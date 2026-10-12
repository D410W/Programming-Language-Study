#pragma once

#include <string.h>
#include <stdlib.h>

#include "common.h"

/**
 * Lexical elements:
 * 1. Control flow ('if', 'else')
 * 2. Declaration ('let', '=')
 * 3. Grammar symbols (';', ':', '.', ',', '{', '}', '(', ')')
 * 4. String and char delimiters ('"', '\'')
 * 5. Built-in operators ('-', '+', '*', '/', '==', '!=')
 * 6. Comment ('//', '/*', '* /')
 * 7. Number ('1234', '5301', '0001', '2')
 * 8. Identifier ('num_1', 'foo', 'bar')
 */

#define TOKENKIND_VALUES \
  TKV(TokenKind_NONE, "TokenKind_NONE") \
  \
  TKV(TokenKindCateg_CONTROL_FLOW, "TokenKindCateg_CONTROL_FLOW") \
  \
  TKV(TokenKind_IF, "TokenKind_IF") \
  TKV(TokenKind_ELSE, "TokenKind_ELSE") \
  \
  TKV(TokenKindCateg_DECLARATION, "TokenKindCateg_DECLARATION") \
  \
  TKV(TokenKind_LET, "TokenKind_LET") \
  TKV(TokenKind_ASSIGN, "TokenKind_ASSIGN") \
  \
  TKV(TokenKindCateg_GRAMMAR, "TokenKindCateg_GRAMMAR") \
  \
  TKV(TokenKind_SEMICOLON, "TokenKind_SEMICOLON") \
  TKV(TokenKind_COLON, "TokenKind_COLON") \
  TKV(TokenKind_DOT, "TokenKind_DOT") \
  TKV(TokenKind_COMMA, "TokenKind_COMMA") \
  TKV(TokenKind_LEFT_BRACKET, "TokenKind_LEFT_BRACKET") \
  TKV(TokenKind_RIGHT_BRACKET, "TokenKind_RIGHT_BRACKET") \
  TKV(TokenKind_LEFT_PARENTHESIS, "TokenKind_LEFT_PARENTHESIS") \
  TKV(TokenKind_RIGHT_PARENTHESIS, "TokenKind_RIGHT_PARENTHESIS") \
  \
  TKV(TokenKindCateg_STRING, "TokenKindCateg_STRING") \
  \
  TKV(TokenKind_SINGLE_QUOTE, "TokenKind_SINGLE_QUOTE") \
  TKV(TokenKind_DOUBLE_QUOTE, "TokenKind_DOUBLE_QUOTE") \
  \
  TKV(TokenKindCateg_OPERATOR, "TokenKindCateg_OPERATOR") \
  \
  TKV(TokenKind_SUBTRACT, "TokenKind_SUBTRACT") \
  TKV(TokenKind_ADD, "TokenKind_ADD") \
  TKV(TokenKind_MULTIPLY, "TokenKind_MULTIPLY") \
  TKV(TokenKind_DIVIDE, "TokenKind_DIVIDE") \
  TKV(TokenKind_EQUALS, "TokenKind_EQUALS") \
  TKV(TokenKind_NOT_EQUALS, "TokenKind_NOT_EQUALS") \
  \
  TKV(TokenKindCateg_COMMENT_TEXT, "TokenKindCateg_COMMENT_TEXT") \
  \
  TKV(TokenKind_LINE_COMMENT, "TokenKind_LINE_COMMENT") \
  TKV(TokenKind_MULTILINE_COMMENT, "TokenKind_MULTILINE_COMMENT") \
  \
  TKV(TokenKindCateg_NUMBER, "TokenKindCateg_NUMBER") \
  \
  TKV(TokenKind_NUMBER, "TokenKind_NUMBER") \
  \
  TKV(TokenKindCateg_IDENTIFIER, "TokenKindCateg_IDENTIFIER") \
  \
  TKV(TokenKind_IDENTIFIER, "TokenKind_IDENTIFIER") \
  \
  TKV(TokenKind_SIZE, "TokenKind_SIZE")
// #define TOKENKIND_VALUES

#define TKV(type, name) type,
typedef enum {
  TOKENKIND_VALUES
} TokenKind;
#undef TKV

extern char* tokenkind_names[TokenKind_SIZE + 1];

typedef struct {
  TokenKind categ;
  TokenKind type;
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
 * Arg2: Whether to print debugging info to stdout.
 * Return: A list of tokens.
 */
TokenList lexical_analysis(SourceFile sf, bool verbose);

/**
 * Cleans up memory allocated for a 'TokenList' struct.
 * 
 * Arg1: TokenList struct.
 */
void free_token_list(TokenList tl);

/**
 * Writes the name of a 'TokenKind' into a char array of capacity 'capacity'.
 * 
 * Arg1: char array pointer.
 * Arg2: Maximum capacity for provided char array.
 * Arg3: Enum value of the token to write.
 */
void tokenkind_to_str(char* str, size_t capacity, TokenKind token_type);

#ifdef LEXER_IMPLEMENTATION

char* control_flow_tokens[] = { "if", "else" };
Token new_token_control_flow(char* start, size_t* token_length) {
  size_t length = 0;
  
  Token new_t = {
    .categ = TokenKindCateg_CONTROL_FLOW,
    .type = TokenKind_NONE,
    .value = NULL
  };
  
  if (start[0] == 'i') {
    length = 2;
    new_t.type = TokenKind_IF;
  } else {
    length = 4;
    new_t.type = TokenKind_ELSE;
  }
  
  new_t.value = malloc((length+1) * sizeof(char));
  strncpy(new_t.value, start, length);
  new_t.value[length] = '\0';
  
  if (token_length != NULL) *token_length = length;
  return new_t;
}
char* declaration_tokens[] = { "let", "=" };
Token new_token_declaration(char* start, size_t* token_length) {
  size_t length = 0;
  
  Token new_t = {
    .categ = TokenKindCateg_DECLARATION,
    .type = TokenKind_NONE,
    .value = NULL
  };
  
  if (start[0] == 'l') {
    length = 3;
    new_t.type = TokenKind_LET;
  } else {
    length = 1;
    new_t.type = TokenKind_ASSIGN;
  }
  
  new_t.value = malloc((length+1) * sizeof(char));
  strncpy(new_t.value, start, length);
  new_t.value[length] = '\0';
  
  if (token_length != NULL) *token_length = length;
  return new_t;
}
char* grammar_tokens[] = { ";", ":", ".", ",", "{", "}", "(", ")" };
Token new_token_grammar(char* start, size_t* token_length) {
  size_t length = 1;
  
  Token new_t = {
    .categ = TokenKindCateg_GRAMMAR,
    .type = TokenKind_NONE,
    .value = malloc((length+1) * sizeof(char))
  };
  
  switch (start[0]) {
    case ';': new_t.type = TokenKind_SEMICOLON;         break;
    case ':': new_t.type = TokenKind_COLON;             break;
    case '.': new_t.type = TokenKind_DOT;               break;
    case ',': new_t.type = TokenKind_COMMA;             break;
    case '{': new_t.type = TokenKind_LEFT_BRACKET;      break;
    case '}': new_t.type = TokenKind_RIGHT_BRACKET;     break;
    case '(': new_t.type = TokenKind_LEFT_PARENTHESIS;  break;
    case ')': new_t.type = TokenKind_RIGHT_PARENTHESIS; break;
  }
  
  strncpy(new_t.value, start, length);
  new_t.value[length] = '\0';
  
  if (token_length != NULL) *token_length = length;
  return new_t;
}
char* string_tokens[] = { "\"", "'" };
Token new_token_string_text(char* start, size_t token_length) {
  Token new_t = {
    .categ = TokenKindCateg_STRING,
    .type = TokenKind_NONE,
    .value = malloc((token_length+1) * sizeof(char))
  };
  
  switch (start[0]) {
    case '\'': new_t.type = TokenKind_SINGLE_QUOTE; break;
    case '"' : new_t.type = TokenKind_DOUBLE_QUOTE; break;
  }
  
  strncpy(new_t.value, start, token_length);
  new_t.value[token_length] = '\0';
  
  return new_t;
}
char* operator_tokens[] = { "-", "+", "*", "/", "==", "!=" };
Token new_token_operator(char* start, size_t* token_length) {
  size_t length = 1;
  
  if (start[0] == '=' || start[0] == '!') {
    length = 2;
  }
  
  Token new_t = {
    .categ = TokenKindCateg_OPERATOR,
    .type = TokenKind_NONE,
    .value = malloc((length+1) * sizeof(char))
  };
  
  switch (start[0]) {
    case '-': new_t.type = TokenKind_SUBTRACT;   break;
    case '+': new_t.type = TokenKind_ADD;        break;
    case '*': new_t.type = TokenKind_MULTIPLY;   break;
    case '/': new_t.type = TokenKind_DIVIDE;     break;
    case '=': new_t.type = TokenKind_EQUALS;     break;
    case '!': new_t.type = TokenKind_NOT_EQUALS; break;
  }
  
  strncpy(new_t.value, start, length);
  new_t.value[length] = '\0';
  
  if (token_length != NULL) *token_length = length;
  return new_t;
}
char* comment_tokens[] = { "//", "/*", "*/" };
Token new_token_comment_text(char* start, size_t token_length) {
  Token new_t = {
    .categ = TokenKindCateg_COMMENT_TEXT,
    .type = TokenKind_NONE,
    .value = malloc((token_length+1) * sizeof(char))
  };
  
  if (start[0] == '/' && start[1] == '/') {
    new_t.type = TokenKind_LINE_COMMENT;
  } else {
    new_t.type = TokenKind_MULTILINE_COMMENT;
  }
  
  strncpy(new_t.value, start, token_length);
  new_t.value[token_length] = '\0';
  
  return new_t;
}
Token new_token_number(char* start, size_t token_length) {
  Token new_t = {
    .categ = TokenKindCateg_NUMBER,
    .type = TokenKind_NUMBER,
    .value = malloc((token_length+1) * sizeof(char))
  };
  
  strncpy(new_t.value, start, token_length);
  new_t.value[token_length] = '\0';
  
  return new_t;
}
Token new_token_identifier(char* start, size_t token_length) {
  Token new_t = {
    .categ = TokenKindCateg_IDENTIFIER,
    .type = TokenKind_IDENTIFIER,
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
  size_t words_length = arrlen(control_flow_tokens);
  
  int match_result = match_specific_string(words, words_length, start);
  return match_result;
}
int is_token_declaration(char* start) {
  char** words = declaration_tokens;
  size_t words_length = arrlen(declaration_tokens);
  
  int match_result = match_specific_string(words, words_length, start);
  return match_result;
}
int is_token_grammar(char* start) {
  char** words = grammar_tokens;
  size_t words_length = arrlen(grammar_tokens);
  
  int match_result = match_specific_string(words, words_length, start);
  return match_result;
}
int is_token_string_delimeter(char* start) {
  char** words = string_tokens;
  size_t words_length = arrlen(string_tokens);
  
  int match_result = match_specific_string(words, words_length, start);
  return match_result;
}
int is_token_operator(char* start) {
  char** words = operator_tokens;
  size_t words_length = arrlen(operator_tokens);
  
  int match_result = match_specific_string(words, words_length, start);
  return match_result;
}
int is_token_comment(char* start) {
  char** words = comment_tokens;
  size_t words_length = arrlen(comment_tokens);
  
  int match_result = match_specific_string(words, words_length, start);
  return match_result;
}
int is_ignore_character(char* start) {
  char* words[] = {" ", "\r"};
  size_t words_length = arrlen(words);
  
  int match_result = match_specific_string(words, words_length, start);
  return match_result;
}

void append_token(TokenList* tl, Token token) {
  tl->size += 1;
  tl->tokens = realloc(tl->tokens, tl->size * sizeof(Token));
  tl->tokens[tl->size - 1] = token;
}

void* fixed_length_token_checkers[] = {
  &is_token_control_flow, &is_token_declaration, &is_token_grammar,
  &is_token_operator,
};
void* fixed_length_constructors[] = {
  &new_token_control_flow, &new_token_declaration, &new_token_grammar,
  &new_token_operator,
};

void* var_length_token_checkers[] = {
  &is_token_number, &is_token_identifier,
};
void* var_length_constructors[] = {
  &new_token_number, &new_token_identifier,
};

typedef enum {
  LexerState_CODE = 0,
  LexerState_STRING_SINGLE,
  LexerState_STRING_DOUBLE,
  LexerState_COMMENT,
} LexerState;

TokenList lexical_analysis(SourceFile sf, bool verbose) {
  if (verbose) printf(" - Tokenizing source file\n");
  TokenList tl = {0};
  LexerState state = LexerState_CODE;
  
  size_t text_token_start_line = 0;
  size_t text_token_start = 0;
  
  for (size_t curr_line_idx = 0; curr_line_idx < sf.size; ++curr_line_idx) {
    if (verbose) printf("line %zu\n", curr_line_idx + 1);
    Line line = sf.lines[curr_line_idx];
    size_t curr_char_idx = 0;

    while (curr_char_idx < line.size) {
      if (verbose) printf("character %zu, state %hi\n", curr_char_idx + 1, state);
      char* current_str = line.contents + curr_char_idx;
      size_t chars_processed = (is_ignore_character(current_str) != -1);
      
      // Lexer state changing
      if (chars_processed == 0) {
        if (state == LexerState_CODE) {
          if (is_token_comment(current_str) == 0) { // token == //
            size_t remaining_line_chars = line.size - curr_char_idx;
            
            Token new_t = new_token_comment_text(current_str, remaining_line_chars);
            append_token(&tl, new_t);
            
            break;
          } else if (is_token_comment(current_str) == 1) { // token == /*
            text_token_start_line = curr_line_idx;
            text_token_start = curr_char_idx;
            state = LexerState_COMMENT;
            chars_processed = 2;
          } else if (is_token_string_delimeter(current_str) == 0) { // token == "
            text_token_start_line = curr_line_idx;
            text_token_start = curr_char_idx;
            state = LexerState_STRING_DOUBLE;
            chars_processed = 1;
          } else if (is_token_string_delimeter(current_str) == 1) { // token == '
            text_token_start_line = curr_line_idx;
            text_token_start = curr_char_idx;
            state = LexerState_STRING_SINGLE;
            chars_processed = 1;
          }
        } else if (state == LexerState_STRING_DOUBLE) {
          chars_processed = 1;
          if (is_token_string_delimeter(current_str) == 0) { // token == "
            if (!(curr_char_idx > 0 && line.contents[curr_char_idx - 1] == '\\')) {
              state = LexerState_CODE;
              
              size_t string_length = curr_char_idx - text_token_start + 1;
              
              Token new_t = new_token_string_text(line.contents + text_token_start, string_length);
              append_token(&tl, new_t);
            }
          }
        } else if (state == LexerState_STRING_SINGLE) {
          chars_processed = 1;
          if (is_token_string_delimeter(current_str) == 1) { // token == '
            if (!(curr_char_idx > 0 && line.contents[curr_char_idx - 1] == '\\')) {
              state = LexerState_CODE;
              
              size_t string_length = curr_char_idx - text_token_start + 1;
              
              Token new_t = new_token_string_text(line.contents + text_token_start, string_length);
              append_token(&tl, new_t);
            }
          }
        } else if (state == LexerState_COMMENT) {
          if (is_token_comment(current_str) == 2) { // token == '*/'
            chars_processed = 2;
            state = LexerState_CODE;
          } else {
            chars_processed = 1;
          }
        }
      }
      
      int (*is_token)(char*);
      Token (*new_fix_token)(char*, size_t*);
      Token (*new_var_token)(char*, size_t);
      
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
      if (chars_processed == -1) {
        if (32 <= line.contents[curr_char_idx] && line.contents[curr_char_idx] <= 126) { // is visible
          printf("Unrecognized token starting with char '%c', position %zu, %zu\n", line.contents[curr_char_idx], curr_line_idx+1, curr_char_idx+1);
        } else {
          printf("Unrecognized token starting with char value '%i', position %zu, %zu\n", line.contents[curr_char_idx], curr_line_idx+1, curr_char_idx+1);
        }
        
        return tl;
      }
      
      curr_char_idx += chars_processed;
    }
  }
  
  if (state != LexerState_CODE) {
    printf("Unclosed text token starting at position %zu, %zu\n", text_token_start_line + 1, text_token_start + 1);
  }
  
  return tl;
}

void free_token_list(TokenList tl) {
  for (size_t idx = 0; idx < tl.size; ++idx) {
    free(tl.tokens[idx].value);
  }
  free(tl.tokens);
}

#define TKV(type, name) name,
char* tokenkind_names[] = {
  TOKENKIND_VALUES
};
#undef TKV

void tokenkind_to_str(char* str, size_t capacity, TokenKind token_type) {
  strncpy(str, tokenkind_names[token_type], capacity);
}

#endif
