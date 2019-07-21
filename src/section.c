#include <cconf/section.h>
#include <string.h>
#include <stdlib.h>

void section_create(section_t *section, char *name) {
  size_t len = strlen(name);
  section->pairs = 0;
  section->pair_count = 0;
  section->name = (char *) malloc(sizeof(char) * (len + 1));
  memcpy(section->name, name, len);
  section->name[len] = 0;
}

void section_delete(section_t *section) {
  int i;
  if(section->pairs) {
    for(i = 0; i < section->pair_count; i++) {
      pair_delete(&section->pairs[i]);
    }
    free(section->pairs);
    if(section->name) {
      free(section->name);
    }
  }
}