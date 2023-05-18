#include <iostream>
#include <iomanip>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>

int main() {
    int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sockfd < 0) {
        std::cout << "Error opening socket" << std::endl;
        return 1;
    }

    unsigned char buffer[2048];
    while (true) {
        int dataSize = recv(sockfd, buffer, sizeof(buffer), 0);
        if (dataSize < 0) {
            std::cout << "Error receiving packet" << std::endl;
            break;
        }

        struct ethhdr* ethHeader = (struct ethhdr*)buffer;
        struct iphdr* ipHeader = (struct iphdr*)(buffer + sizeof(struct ethhdr));

        // Check if the packet is an IP packet
        if (ntohs(ethHeader->h_proto) == ETH_P_IP) {
            std::cout << "IP Packet:" << std::endl;
            std::cout << "Source IP: " << inet_ntoa(*(struct in_addr*)&ipHeader->saddr) << std::endl;
            std::cout << "Destination IP: " << inet_ntoa(*(struct in_addr*)&ipHeader->daddr) << std::endl;
        }
        // Check if the packet is an ARP packet
        else if (ntohs(ethHeader->h_proto) == ETH_P_ARP) {
            std::cout << "ARP Packet" << std::endl;
        }

        // Print the first 42 bytes of the packet in hexadecimal format
        for (int i = 0; i < 42; i++) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)buffer[i] << " ";
            if ((i + 1) % 22 == 0 || (i + 1) % 42 == 0) {
                std::cout << std::endl;
            }
        }
        std::cout << std::endl << std::endl;
    }

    close(sockfd);
    return 0;
}
