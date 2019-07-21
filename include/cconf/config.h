#ifndef __CCONF_CONFIG_H__
#define __CCONF_CONFIG_H__

#include <cconf/section.h>

typedef struct {
  section_t *sections;
  size_t section_count;
} config_t;

void config_create(config_t *config, char *fn);
section_t *config_get_section(config_t *config, char *name);
void config_delete(config_t *config);

#endif