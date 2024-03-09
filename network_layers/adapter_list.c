#include<stdlib.h>
#include<stdio.h>
#include<ifaddrs.h>
#include<sys/socket.h>

int main(int argc, char const *argv[])
{
    struct ifaddrs *addresses;

    if(getifaddrs(&addresses) == -1){
        printf("Network devices not found.\n");
    }

    struct ifaddrs *address = addresses;

    while(address){
        int family = address->ifa_addr->sa_family;
        printf("Family: %d\n", family);
        address = address->ifa_next;
    }
    
    return 0;
}
