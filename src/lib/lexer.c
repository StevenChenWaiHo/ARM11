#include "lexer.h"

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool is_at_end(Lexer *l) { return l->current >= l->source_len; }
static char peak(Lexer *l) { return l->source[l->current]; }
static char advance(Lexer *l) {
  char c = peak(l);
  l->current++;
  if (c == '\n') {
    l->line++;
    l->column = 0;
  } else {
    l->column++;
  }
  return c;
}

static void skip_whitespace(Lexer *l) {
  // Dont skip newlines, they are used to split instructions.
  while (!is_at_end(l)) {
    char c = peak(l);
    switch (c) {
    case ' ':
    case '\t':
      advance(l);
      break;
    case ';':
      // Comment
      // TODO: skip_comment
      assert(0);
      break;
    default:
      return;
    }
  }
}
static Token make_token(Lexer *l, TokenKind tk) {
  Token t = {tk, &l->source[l->start], l->current - l->start, l->line,
             l->column};
  return t;
}
static bool is_alphanum(char c) { return isalnum(c) || c == '_'; }

static Token identifier(Lexer *l) {
  while (is_alphanum(peak(l)))
    advance(l);
  return make_token(l, TOKEN_IDENT);
}
static Token hash_num(Lexer *l) {
  while (isdigit(peak(l))) // TODO: hex, enforce at least 1 digit
    advance(l);
  return make_token(l, TOKEN_HASH_NUM);
}

Token lexer_next(Lexer *l) {
  skip_whitespace(l);
  l->start = l->current;
  if (is_at_end(l))
    return make_token(l, TOKEN_EOF);

  char c = advance(l);
  if (isalpha(c)) {
    return identifier(l);
  }
  switch (c) {
  case ',':
    return make_token(l, TOKEN_COMMA);
  case '#':
    return hash_num(l);
  case '\n':
    return make_token(l, TOKEN_NEWLINE);
  }
  fprintf(stderr, "Unexpected character: %c\n", c);
  exit(-1);
}

Lexer lexer_new(const char *source, const char *filename) {
  Lexer l = {
      .source = source,
      .filename = filename,
      .start = 0,
      .current = 0,
      .line = 0,
      .column = 0,
      .source_len = strlen(source),
  };
  return l;
}

const char *token_kind_name(TokenKind tk) {
  switch (tk) {
  case TOKEN_IDENT:
    return "TOKEN_IDENT";
  case TOKEN_HASH_NUM:
    return "TOKEN_HASH_NUM";
  case TOKEN_EQ_NUM:
    return "TOKEN_EQ_NUM";
  case TOKEN_COMMA:
    return "TOKEN_COMMA";
  case TOKEN_EOF:
    return "TOKEN_EOF";
  case TOKEN_NEWLINE:
    return "TOKEN_NEWLINE";
  }
}
