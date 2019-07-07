#include "br.h"

#include <stdio.h>
#include <string.h>

#pragma comment(lib, "br.lib")

int main() {
  br_broadcaster* b = br_new_broadcaster();
  if (!b) {
    // failure
  }
  const char* message = "this is the truth";

  printf("sending: %s\r\n", message);

  if (br_broadcast(b, message, 1 + strlen(message))) {
    // failure
  }

  br_free_broadcaster(b);
}
