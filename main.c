#include <stdio.h>
#include <cconf.h>

int main() {
  config_t conf;
  config_create(&conf, "examples/sample.conf");

  section_t *sec = config_get_section(&conf, "buttons");
  pair_t *pair = section_get_pair(sec, "Button0");
  
  printf("Value: %s\n", pair->value);
  
  config_delete(&conf);
  return 0;
}