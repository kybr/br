#include "br.h"

#include <stdlib.h>  // malloc

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

int br_broadcast(br_broadcaster* b, const char* data, int size) {
  return 0;
  //
}

int br_receive(br_receiver* r, char* data, int size) {
  return 0;
  //
}
