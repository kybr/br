#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>

#include <windows.h>

#include <ws2tcpip.h>

//#include <iphlpapi.h>  // ???

#include <stdlib.h>  // malloc

#pragma comment(lib, "ws2_32.lib")
//#pragma comment(lib, "mswsock.lib")
//#pragma comment(lib, "AdvApi32.lib")

#include "help.h"

DWORD WINAPI ThreadFunc(void* data) {
  // Do stuff.  This will be the first function called on the new thread.
  // When this function returns, the thread goes away.  See MSDN for more
  // details.
  return 0;
}

int main(int argc, char** argv) {
  WSADATA wsadata;
  if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsadata)) {
    int code = WSAGetLastError();
    printf("ERROR:%d :: %s\n", __LINE__, getstr(code));
    exit(code);
  }

  // IPv4 UDP
  SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (INVALID_SOCKET == s) {
    int code = WSAGetLastError();
    printf("ERROR:%d :: %s\n", __LINE__, getstr(code));
    exit(code);
  }

  struct sockaddr_in local;
  local.sin_family = AF_INET;
  local.sin_addr.s_addr = inet_addr("0.0.0.0");
  local.sin_port = htons(26162);

  HANDLE thread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);
  if (thread) {
    // Optionally do stuff, such as wait on the thread.
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
  // SO_RCVBUF
  // SO_RCVTIMEO
  // SO_SNDBUF
  // SO_SNDTIMEO

  if (SOCKET_ERROR == bind(s, (const struct sockaddr*)&local, sizeof(local))) {
    int code = WSAGetLastError();
    printf("ERROR:%d :: %s\n", __LINE__, getstr(code));
    closesocket(s);
    WSACleanup();
    exit(code);
  }

  char* receiveData = calloc(65535, 1);
  int n = 100;

  int done = 0;
  while (!done) {
    struct sockaddr_in from = {0};
    int length = sizeof(from);

    {
      int result = recvfrom(s, receiveData, n, 0, (SOCKADDR*)&from, &length);
      if (SOCKET_ERROR == result) {
        int code = WSAGetLastError();
        printf("ERROR:%d :: %s\n", __LINE__, getstr(code));
        closesocket(s);
        WSACleanup();
        exit(code);
      }

      if (0 == result) {
        printf("ERROR:%d :: %s\n", __LINE__, "gracefully closed");
        closesocket(s);
        WSACleanup();
        return 0;
      }

      if (n != result) {
        printf("WARNING: recvfrom of %d returned %d\n", n, result);
      } else {
        printf("received %d\n", receiveData[0]);
      }
    }

    {
      int result =
          sendto(s, receiveData, 1, 0, (SOCKADDR*)&from, sizeof(SOCKADDR));
      if (SOCKET_ERROR == result) {
        int code = WSAGetLastError();
        printf("ERROR:%d :: %s\n", __LINE__, getstr(code));
        closesocket(s);
        WSACleanup();
        exit(code);
      }

      if (1 != result) {
        printf("WARNING: sendto of %d returned %d\n", n, result);
      } else {
        printf("echo'd %d\n", receiveData[0]);
      }
    }
  }

  closesocket(s);
  WSACleanup();
  return 0;
}