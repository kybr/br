#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>

void endl() {
#if defined(_WIN64) || defined(_WIN32)
  printf("\r\n");
#else
  printf("\n");
#endif
}

int main() {
  WSADATA WinsockData;
  if (WSAStartup(MAKEWORD(2, 2), &WinsockData) != 0) {
    int code = WSAGetLastError();
    printf("ERROR:%d :: %d\n", __LINE__, code);
    return code;
  }

  SOCKET sd = WSASocket(AF_INET, SOCK_DGRAM, 0, 0, 0, 0);
  if (sd == SOCKET_ERROR) {
    int code = WSAGetLastError();
    printf("ERROR:%d :: %d\n", __LINE__, code);
    return code;
  }

  INTERFACE_INFO InterfaceList[20];  // XXX 20?
  unsigned long nBytesReturned;
  if (WSAIoctl(sd, SIO_GET_INTERFACE_LIST, 0, 0, &InterfaceList,
               sizeof(InterfaceList), &nBytesReturned, 0, 0) == SOCKET_ERROR) {
    int code = WSAGetLastError();
    printf("ERROR:%d :: %d\n", __LINE__, code);
    return code;
  }

  int nNumInterfaces = nBytesReturned / sizeof(INTERFACE_INFO);
  for (int i = 0; i < nNumInterfaces; ++i) {
    endl();

    printf("interface:\t%d", i);
    endl();

    u_long nFlags = InterfaceList[i].iiFlags;

    printf("flags:  \t");
    if (nFlags & IFF_POINTTOPOINT) printf("point-to-point ");
    if (nFlags & IFF_LOOPBACK) printf("loopback ");
    if (nFlags & IFF_BROADCAST) printf("broadcast ");
    if (nFlags & IFF_MULTICAST) printf("multicast ");
    endl();

    printf("state:  \t%s", (nFlags & IFF_UP) ? "up" : "down");
    endl();

    struct sockaddr_in *pAddress;
    pAddress = (struct sockaddr_in *)&(InterfaceList[i].iiAddress);
    printf("address:\t%s", inet_ntoa(pAddress->sin_addr));
    endl();

    pAddress = (struct sockaddr_in *)&(InterfaceList[i].iiBroadcastAddress);
    printf("broadcast:\t%s", inet_ntoa(pAddress->sin_addr));
    endl();

    pAddress = (struct sockaddr_in *)&(InterfaceList[i].iiNetmask);
    printf("netmask:\t%s", inet_ntoa(pAddress->sin_addr));
    endl();

    endl();
  }

  WSACleanup();
  return 0;
}
