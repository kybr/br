#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>

#include <windows.h>

#include <ws2tcpip.h>

#include <stdlib.h>  // malloc

#pragma comment(lib, "ws2_32.lib")

DWORD WINAPI ThreadFunc(void* data) {
  // Do stuff.  This will be the first function called on the new thread.
  // When this function returns, the thread goes away.  See MSDN for more
  // details.
  return 0;
}

#include "help.h"

int main(int argc, char** argv) {
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
  local.sin_port = htons(receiver_port);

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
      struct sockaddr_in* pAddress;
      pAddress = (struct sockaddr_in*)&(InterfaceList[i].iiAddress);

      if (found_interface) {
        printf("(%s is an alternative multicast interface",
               inet_ntoa(pAddress->sin_addr));
        endl();
        continue;
      }

      // sets the IP we'll bind to
      local.sin_addr.s_addr = inet_addr(inet_ntoa(pAddress->sin_addr));

      // tell the socket we want to join a multicast group
      struct ip_mreq imr;
      imr.imr_multiaddr.s_addr = inet_addr(multicast_group);
      imr.imr_interface.s_addr = inet_addr(inet_ntoa(pAddress->sin_addr));

      if (SOCKET_ERROR == setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                                     (char*)&imr, sizeof(imr))) {
        int code = WSAGetLastError();
        printf("ERROR:%d :: %s\n", __LINE__, getstr(code));
        exit(code);
      }

      printf("multicast group: %s interface: %s port: %hu", multicast_group,
             inet_ntoa(pAddress->sin_addr), receiver_port);
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
                                   (const char*)&value, sizeof(value))) {
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

  if (SOCKET_ERROR == bind(s, (const struct sockaddr*)&local, sizeof(local))) {
    int code = WSAGetLastError();
    printf("ERROR:%d :: %s\n", __LINE__, getstr(code));
    closesocket(s);
    WSACleanup();
    exit(code);
  }

  const int N = 65536;  // largest possible payload
  unsigned char* data = calloc(N, 1);

  // XXX replace this with a multithreaded collection
  while (1) {
    // know who sent us the packet
    struct sockaddr_in from = {0};
    int length = sizeof(from);

    int size = recvfrom(s, data, N, 0, (struct sockaddr*)&from, &length);
    if (SOCKET_ERROR == size) {
      int code = WSAGetLastError();
      printf("ERROR:%d :: %s\n", __LINE__, getstr(code));
      closesocket(s);
      WSACleanup();
      exit(code);
    }

    if (0 == size) {
      printf("ERROR:%d :: %s\n", __LINE__, "gracefully closed");
      closesocket(s);
      WSACleanup();
      return 0;
    }

    printf("data[0]=%d in %d byte packet from %s\n", data[0], size,
           inet_ntoa(from.sin_addr));

    if (data[0] == 255) break;
  }

  closesocket(s);
  WSACleanup();

  HANDLE thread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);
  if (thread) {
    // Optionally do stuff, such as wait on the thread.
  }
  return 0;
}