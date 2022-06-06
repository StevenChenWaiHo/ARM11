#ifndef AEMU_LEXER_H
#define AEMU_LEXER_H

#include <stddef.h>
// Loosly based on the clox lexer

typedef enum {
  TOKEN_COMMA,
  TOKEN_EOF,
  TOKEN_EQ_NUM,
  TOKEN_HASH_NUM,
  TOKEN_IDENT,
  TOKEN_LABEL,
  TOKEN_LSQUARE,
  TOKEN_NEWLINE,
  TOKEN_RSQUARE,
} TokenKind;

typedef struct {
  TokenKind kind;
  const char *source;
  size_t len;
  size_t line;
  size_t column;
} Token;

typedef struct {
  const char *source;
  const char *filename;
  size_t start;
  size_t current;
  size_t line;
  size_t column;
  size_t source_len;
} Lexer;

Token lexer_next(Lexer *);
Lexer lexer_new(const char *source, const char *filename);
const char *token_kind_name(TokenKind);

#endif
