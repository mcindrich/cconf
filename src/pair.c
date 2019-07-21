#include <cconf/pair.h>
#include <string.h>
#include <stdlib.h>

void pair_create(pair_t *pair) {
  pair->key = 0;
  pair->value = 0;
}

void pair_set_key(pair_t *pair, char *key) {
  size_t len = strlen(key);
  pair->key = (char *) malloc(sizeof(char) * (len + 1));
  memcpy(pair->key, key, len);
  pair->key[len] = 0;
}

void pair_set_value(pair_t *pair, char *value) {
  size_t len = strlen(value);
  pair->value = (char *) malloc(sizeof(char) * (len + 1));
  memcpy(pair->value, value, len);
  pair->value[len] = 0;
}

void pair_delete(pair_t *pair) {
  if(pair->key) {
    free(pair->key);
  }
  if(pair->value) {
    free(pair->value);
  }
  pair_create(pair);
}