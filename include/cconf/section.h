#ifndef __CCONF_SECTION_H__
#define __CCONF_SECTION_H__

#include <cconf/pair.h>
#include <stddef.h>

typedef struct {
  char *name;
  pair_t *pairs;
  size_t pair_count;
} section_t;

void section_create(section_t *section, char *name);
pair_t *section_get_pair(section_t *section, char *key);
void section_delete(section_t *section);

#endif