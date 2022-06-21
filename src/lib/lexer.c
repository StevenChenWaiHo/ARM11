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
static bool match(Lexer *l, char expected) {
  if (is_at_end(l))
    return false;
  if (peak(l) != expected)
    return false;
  advance(l);
  return true;
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
      while (!is_at_end(l) && peak(l) != '\n')
        advance(l);
      break;
    default:
      return;
    }
  }
}
static Token make_token(Lexer *l, TokenKind tk) {
  Token t = {
      tk, {&l->source[l->start], l->current - l->start}, l->line, l->column};
  return t;
}
static bool is_alphanum(char c) { return isalnum(c) || c == '_'; }
static bool is_hex(char c) {
  return isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') ||
         (c == 'x' || c == 'X') || c == '-';
}

static Token identifier(Lexer *l) {
  while (is_alphanum(peak(l)))
    advance(l);
  TokenKind tk = match(l, ':') ? TOKEN_LABEL : TOKEN_IDENT;
  return make_token(l, tk);
}
static Token hash_num(Lexer *l) {
  while (is_hex(
      peak(l))) // TODO: be more perciese about hex, enforce at least 1 digit
    advance(l);
  return make_token(l, TOKEN_HASH_NUM);
}
static Token eq_num(Lexer *l) {
  while (is_hex(peak(l))) // TODO: Meger with hash_num
    advance(l);
  return make_token(l, TOKEN_EQ_NUM);
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
  case '[':
    return make_token(l, TOKEN_LSQUARE);
  case ']':
    return make_token(l, TOKEN_RSQUARE);
  case '=':
    return eq_num(l);
  case '+':
    return make_token(l, TOKEN_PLUS);
  case '-':
    return make_token(l, TOKEN_MINUS);
  }
  fprintf(stderr, "Unexpected character: %c\n", c);
#ifdef FUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION
  exit(0);
#else
  exit(1);
#endif
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
  case TOKEN_COMMA:
    return "TOKEN_COMMA";
  case TOKEN_EOF:
    return "TOKEN_EOF";
  case TOKEN_EQ_NUM:
    return "TOKEN_EQ_NUM";
  case TOKEN_HASH_NUM:
    return "TOKEN_HASH_NUM";
  case TOKEN_IDENT:
    return "TOKEN_IDENT";
  case TOKEN_LABEL:
    return "TOKEN_LABEL";
  case TOKEN_LSQUARE:
    return "TOKEN_LSQUARE";
  case TOKEN_NEWLINE:
    return "TOKEN_NEWLINE";
  case TOKEN_RSQUARE:
    return "TOKEN_RSQUARE";
  case TOKEN_PLUS:
    return "TOKEN_PLUS";
  case TOKEN_MINUS:
    return "TOKEN_MINUS";
  default:
    assert(0);
  }
}
