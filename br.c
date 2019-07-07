#include "br.h"

#include <stdlib.h>  // malloc

#pragma comment(lib, "ws2_32.lib")

struct br_broadcaster {
  int stuff;
};

struct br_receiver {
  int stuff;
};

br_broadcaster* br_new_broadcaster(void) {
  br_broadcaster* b = (br_broadcaster*)malloc(sizeof(br_broadcaster));
  // configure
  return b;
}

br_receiver* br_new_receiver(void) {
  br_receiver* r = (br_receiver*)malloc(sizeof(br_receiver));
  // configure
  return r;
}

void br_free_broadcaster(br_broadcaster* b) {}
void br_free_receiver(br_receiver* r) {}

int br_broadcast(br_broadcaster* b, const char* data, size_t size) {
  return 0;
  //
}

int br_receive(br_receiver* r, char* data, size_t* size) {
  return 0;
  //
}
