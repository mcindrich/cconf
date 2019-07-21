#ifndef __CCONF_PAIR_H__
#define __CCONF_PAIR_H__

typedef struct {
  char *key;
  char *value;
} pair_t;

void pair_create(pair_t *pair);
void pair_set_key(pair_t *pair, char *key);
void pair_set_value(pair_t *pair, char *value);
void pair_delete(pair_t *pair);

#endif