#include <winsock2.h>

#include <ws2tcpip.h>

#include <stdlib.h>  // malloc

#pragma comment(lib, "ws2_32.lib")

#include "help.h"

int main(int argc, char **argv) {
  WSADATA wsadata;
  if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsadata)) {
    int code = WSAGetLastError();
    printf("ERROR:%d :: %s\n", __LINE__, getstr(code));
    exit(code);
  }

  SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (INVALID_SOCKET == s) {
    int code = WSAGetLastError();
    printf("ERROR:%d :: %s\n", __LINE__, getstr(code));
    exit(code);
  }

  INTERFACE_INFO InterfaceList[20];  // XXX 20?
  unsigned long nBytesReturned;
  if (WSAIoctl(s, SIO_GET_INTERFACE_LIST, 0, 0, &InterfaceList,
               sizeof(InterfaceList), &nBytesReturned, 0, 0) == SOCKET_ERROR) {
    int code = WSAGetLastError();
    printf("ERROR:%d :: %s\n", __LINE__, getstr(code));
    exit(code);
  }

  // start configuring the local interface
  struct sockaddr_in local;
  local.sin_family = AF_INET;
  local.sin_port = htons(broadcaster_port);

  // search the list of network interfaces
  int found_interface = 0;
  int nNumInterfaces = nBytesReturned / sizeof(INTERFACE_INFO);
  for (int i = 0; i < nNumInterfaces; ++i) {
    u_long nFlags = InterfaceList[i].iiFlags;
    if (nFlags & IFF_UP)
      ;
    else {
      // ignore interface if it's down
      continue;
    }

    if (nFlags & IFF_LOOPBACK) {
      // ignore interface if it's a "loopback"
      continue;
    }

    if (nFlags & IFF_MULTICAST) {
      struct sockaddr_in *pAddress;
      pAddress = (struct sockaddr_in *)&(InterfaceList[i].iiAddress);

      if (found_interface) {
        printf("(%s is an alternative multicast interface.)",
               inet_ntoa(pAddress->sin_addr));
        endl();
        continue;
      }

      local.sin_addr.s_addr = inet_addr(inet_ntoa(pAddress->sin_addr));

      printf("interface: %s", inet_ntoa(pAddress->sin_addr));
      endl();

      found_interface = 1;
    }
  }

  if (!found_interface) {
    printf("ERROR:%d :: %s\n", __LINE__, "no viable network interface");
    exit(-1);
  }

  // socket option: exclusive address
  {
    int value = 1;
    if (SOCKET_ERROR == setsockopt(s, SOL_SOCKET, SO_EXCLUSIVEADDRUSE,
                                   (const char *)&value, sizeof(value))) {
      int code = WSAGetLastError();
      printf("ERROR:%d :: %s\n", __LINE__, getstr(code));
      closesocket(s);
      WSACleanup();
      exit(code);
    }
    if (!value) {
      printf("ERROR:%d :: %s\n", __LINE__, "exclusive socket option failed");
      WSACleanup();
      exit(-1);
    }
  }

  if (SOCKET_ERROR == bind(s, (const struct sockaddr *)&local, sizeof(local))) {
    int code = WSAGetLastError();
    printf("ERROR:%d :: %s\n", __LINE__, getstr(code));
    closesocket(s);
    WSACleanup();
    exit(code);
  }

  //
  // make two threads:
  // - one that sends a sequence of packets
  // - one that listens, collecting messages
  //

  struct sockaddr_in remote;
  remote.sin_family = AF_INET;
  remote.sin_addr.s_addr = inet_addr(multicast_group);
  remote.sin_port = htons(receiver_port);

  const int N = 65536;  // largest possible payload
  unsigned char *data = calloc(N, 1);
  data[0] = 1;
  int n = 100;

  while (1) {
    int result = sendto(s, data, n, 0, (struct sockaddr *)&remote,
                        sizeof(struct sockaddr));
    if (SOCKET_ERROR == result) {
      int code = WSAGetLastError();
      printf("ERROR:%d :: %s\n", __LINE__, getstr(code));
      closesocket(s);
      WSACleanup();
      exit(code);
    }

    if (n != result) {
      printf("WARNING: sendto of %d returned %d\n", n, result);
    } else {
      printf("sent %d\n", data[0]);
    }

    if (data[0] == 255) break;

    data[0]++;
    Sleep(100);
  }

  closesocket(s);
  WSACleanup();
  return 0;
}