#include <cconf/token.h>
#include <stdlib.h>
#include <string.h>

void token_create(token_t *token) {
  token->type = token_type_none;
}

void token_set_char_value(token_t *token, char c) {
  token->value.c = c;
  if(c == '[') {
    token->type = token_type_lbracket;
  } else if(c == ']') {
    token->type = token_type_rbracket;
  } else {
    token->type = token_type_equal;
  }
}

void token_set_string_value(token_t *token, char *last, char *curr) {
  size_t len = curr - last;
  token->value.s = (char *) malloc(sizeof(char) * (len + 1));
  memcpy(token->value.s, last, len);
  token->value.s[len] = 0;
  token->type = token_type_string;
}

void token_delete(token_t *token) {
  if(token->type == token_type_string) {
    free(token->value.s);
    token->value.s = 0;
  }
}