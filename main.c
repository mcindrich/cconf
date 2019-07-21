#include <stdio.h>
#include <cconf.h>

int main() {
  config_t conf;
  config_create(&conf, "examples/sample.conf");
  config_delete(&conf);
  return 0;
}