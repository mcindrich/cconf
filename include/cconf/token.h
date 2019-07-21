#ifndef __CCONF_TOKEN_H__
#define __CCONF_TOKEN_H__

typedef enum {
  token_type_none = 0,
  token_type_lbracket,
  token_type_rbracket,
  token_type_equal,
  token_type_string
} token_type;

typedef struct {
  union {
    char c;
    char *s;
  } value;
  token_type type;
} token_t;

void token_create(token_t *token);
void token_set_char_value(token_t *token, char c);
void token_set_string_value(token_t *token, char *last, char *curr);
void token_delete(token_t *token);

#endif