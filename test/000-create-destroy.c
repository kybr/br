#include "br.h"

#include <stdio.h>

#pragma comment(lib, "br.lib")

int main() {
  br_broadcaster* b = br_new_broadcaster();
  br_receiver* r = br_new_receiver();

  printf("b:%llx\r\n", (size_t)b);
  printf("r:%llx\r\n", (size_t)r);

  br_free_broadcaster(b);
  br_free_receiver(r);
}
