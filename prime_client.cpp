#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

#define PORT 8060

bool isPrime(int temp)
{
  bool primeNumber = true;
  int halfWay = temp/2;
    for(int i = 2; i <= halfWay; i++)
    { 

      if(temp%i == 0 )
      {
        primeNumber = false;
        break;
      }
    }

  if(temp <= 3)
  {
   primeNumber = true;
  } 
  return primeNumber;

}

int main(int argc, char const *argv[])
{
  struct sockaddr_in address;
  int sock = 0, valread;
  struct sockaddr_in serv_addr;
  string input_buf;
  char cname[1024];

  if(argc!=2){
    strcpy(cname,"Client");
  }
  else
    {
      strcpy(cname,argv[1]);
    }

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      printf("\n Socket creation error \n");
      return -1;
    }

  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

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



char buffer[1024] = {0};
char echo[255];
strcpy(echo,"Yes");

      send(sock, cname, 1024, 0);

  while(1)
  {
      char returnNumber[256];
	  string checkSum;
      read(sock, returnNumber, 255);
      int number;
	  int data;
      char result[256]; 
	  checkSum = returnNumber; 
	  if(checkSum.compare("Are you there?") == 0){
		send(sock, echo, 255, MSG_NOSIGNAL);
	  } else{

      number = atoi (returnNumber);
      


      bool isPrimeResult = isPrime(number);
	  cout << "returnning: "<< number << endl;
      sprintf(result, "%i", isPrimeResult); 
      send(sock, result, 255, 0);
	  }
    

  }
  close(sock);
  return 0;
}

/*

  data = param.recv(1024);
  if not data
  
  int data;
*/
