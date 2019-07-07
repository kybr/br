#include "br.h"

#include <stdio.h>
#include <string.h>

#pragma comment(lib, "br.lib")

int main() {
  br_receiver* r = br_new_receiver();
  if (!r) {
    // failure
  }
  char message[65536];
  size_t size = 100;
  printf("receiving:");
  if (br_receive(r, message, &size)) {
    // failure
  }
  printf("\r\n");

  br_free_receiver(r);
}
