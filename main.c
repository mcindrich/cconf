#include <stdio.h>
#include <cconf.h>

int main() {
  config_t conf;
  config_create(&conf, "examples/sample.conf");

  section_t *sec = config_get_section(&conf, "");
  pair_t *pair = section_get_pair(sec, "global2");
  
  printf("Value: %s\n", pair->value);
  
  config_delete(&conf);
  return 0;
}