#ifndef __CCONF_CONFIG_H__
#define __CCONF_CONFIG_H__

#include <cconf/section.h>
#include <stdio.h>

typedef struct {
	section_t *sections;
	size_t section_count;
} config_t;

void config_create(config_t *config, char *fn);
section_t *config_get_section(config_t *config, char *name);
int config_to_file(config_t *config, FILE *out);
// get all pairs with the given key and store the count in a passed pointer
pair_t **config_get_pairs(config_t *config, char *key, size_t *count);
void config_delete(config_t *config);

#endif