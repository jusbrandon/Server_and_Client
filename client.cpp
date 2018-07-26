//Brandon
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;

  
int main(int argc, char const *argv[])
{
    
    string hostName;
	int portNum;
	cout << "Enter server host name: ";
	cin >> hostName;
	cout << "Enter server port number: ";
	cin >> portNum; 
	cout << endl;
	string major;
	
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *message;
    int bufsize = 1024;
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portNum);
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    do 
    {
        major = "";
        char buffer[1024] = {0};
		cout << "Enter a college major: ";
		cin.ignore();
		getline(cin, major, '\n');
		if(major == "") {
		    return 0;
		}
		for(int i = 0; i < major.length(); i++) {
			buffer[i] = major[i];
		}
        write(sock , buffer, strlen(buffer));
        valread = read( sock , buffer, 256);
        printf("%s\n", buffer);
        cout << endl;
        bzero(buffer, 256);
    } while(true);
    
    return 0;
}