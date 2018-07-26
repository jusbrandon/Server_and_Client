//Brandon
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <map>
#include <iostream>
#include <string.h>
#include <fstream>
#include <algorithm>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;
//Start of program 
int main(int argc, char const *argv[])
{
    //Ask for Text file name ex. input.txt or Salaries.txt
    string fileName;
    cout << "Enter file name: ";
    cin >> fileName;
    //Ask for port number 
    int portNum;
    cout << "Enter server port number: ";
    cin >> portNum;
    cout << endl;
    
    string majors[512];
    string mins[512];
    string maxes[512];
    
    
    
    map <string, vector<string> > myMap;
    //Recieve inputs from client and sends it through searching for results
    fstream keys;
    keys.open(fileName.c_str(), ios::in);
	
    string line;
    int index = 0;
    
    
    while(getline(keys, line)) {
        vector<string> vals;
        string temp1 = "";
        string temp2 = "";
        string temp3 = "";
        int i = 0;
        
        
        while(line[i] != '\t') {
            temp1 += line[i];
            i++;
        }
        i++;
        while(line[i] != '\t') {
            temp2 += line[i];
            i++;
        }
        temp2.erase (std::remove (temp2.begin(), temp2.end(), ' '), temp2.end());
        i++;
        while(line[i] != '\t' && line[i] != ' ') {
            temp3 += line[i];
            i++;
        }
        temp3.erase (std::remove (temp3.begin(), temp3.end(), ' '), temp3.end());
        //cout << temp1 << ", " << temp2 << ", " << temp3 << endl;
        vals.push_back(temp2);
        vals.push_back(temp3);
        myMap.insert(pair <string, vector<string> > (temp1, vals));
        majors[index] = temp1;
        mins[index] = temp2;
        maxes[index] = temp3;
    }
    
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    // Creates socket file and return failes if otherwise
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    // Attaches socket to port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( portNum );
      
    // Force attach socket to port
    if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {//Accepts Listens
        perror("listen");
        exit(EXIT_FAILURE);
    }//Accepts Socket
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    map<string, vector<string> >::iterator it;
    
    while(true) {
        
        read(new_socket , buffer, 256);
        string temp = (string) buffer;
        string temp1 = "";
        string temp2 = "";
        string message = "";
        int len = 0;
        int i = 0;
        cout << buffer << endl;
        it = myMap.find(temp);
        //Displays Salary of College Major if inputed correctly
        if(it != myMap.end()) {
            temp1 = "The average early career pay for a " + temp + " major is $" + it->second.at(0) + "\n";
            temp2 = "The corresponding mid‐career pay is $" + it->second.at(1);
            message = temp1 + temp2;
            len = strlen(message.c_str());
            write(new_socket, message.c_str(), len);
        }//Returns -1 or Manjor not in table if College Major is not in txt file
        else {
            string no = "The major is not in the table";
            len = strlen(no.c_str());
            write(new_socket , no.c_str() , strlen(no.c_str()));
        }
        bzero(buffer, 256);
    }
    return 0;
}