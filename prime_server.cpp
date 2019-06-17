#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <iostream>
#include <pthread.h>
#include <time.h>
#include <string>
#include <string.h>
#include <fstream>
#include <queue> 


using namespace std;

queue <int> unforseenNumber;

#define PORT 8060

// Declare 2 threads getting created
pthread_t threads[2];

// creating my "locker"
pthread_mutex_t lock;

void writeToText(char *primeNumber){
  ofstream myfile;

  myfile.open("P.txt", ios_base::app);
  myfile << primeNumber << "\n" ;
  myfile.close();

}

void isPrime(int temp)
{
  bool notPrime = false;
  int halfWay = temp/2;
    for(int i = 2; i <= halfWay; i++)
    { 

      if(temp%i == 0 )
      {
        notPrime = true;
        break;
      }
    }

  if(!notPrime || temp <= 3)
  {
   cout << "prime test for "<< temp << " (on server) => "<< "Is a prime number => Write " << temp << " in P.txt" << endl;
    char number[256];
  sprintf(number, "%i", temp); 
    writeToText(number);
  } else{
   cout << "prime test for "<< temp << " (on server) => "<< "Is not a prime number" << endl;

  }

}

void *numberQueuer(void *arg)
{
	int unNumber = 2;
	while(unNumber<1000000){
		unforseenNumber.push(unNumber++);
	}
}


void *searchTask(void *arg)
{
  int count = 0;
    while(count < 100 && !unforseenNumber.empty())
    { 
      int temp = unforseenNumber.front();
	  unforseenNumber.pop();
      isPrime(temp);
      count++;
     
  }

return NULL;
}

void *clientTask(void *param)
{

  int new_socket;
  int valread;
  new_socket = (long)param;
	time_t TimeMarker,cTime;
	time(&TimeMarker);
	double snapSeconds = 1;
  char name[1024];
  char *sign;
  sign = "Are you there?";
    char echo[255];
  read(new_socket,name, 1024);
  
  char response[256];
  int unNumber = unforseenNumber.front();
  unforseenNumber.pop();



  
  while(unNumber!=0){
	 int data;
	char result[256]; 
	sprintf(result, "%i", unNumber); 
	printf("%s joins => server sends %s to client \n", name, result);
    send(new_socket, result, 255, MSG_NOSIGNAL);
	time(&cTime);
	if(snapSeconds<=difftime(time(&cTime),TimeMarker)){
		cout<<"more than one second............................................"<<endl;
		
		send(new_socket, sign, 255, MSG_NOSIGNAL);
		int responseWaitTime = 0;
		data = read(new_socket, echo, 255);
		while(data == 0){
			int rez = atoi(result);
			unforseenNumber.push(unNumber);
			cout<<"Pushed "<<unNumber<<" back for testing"<<endl;
			pthread_exit(0);
		}
		time(&TimeMarker);
	}
    valread = read(new_socket, response, 255);
   int rez = atoi(response);
   cout << "Prime test for " << result << " (on client) => " ;
   if(rez == 1){
     cout << "is a prime number => Write " << result << " in P.txt" << endl;
     writeToText(result);
   } else{
          cout << "is not a prime number" << endl;

   }
   memset(response, 0, 255);
unNumber = unforseenNumber.front();
  unforseenNumber.pop();
   sprintf(result, "%i", unNumber); 
  printf("Send %s to Client \n", result);
  
  }
  pthread_exit(0);
}


int main()
{
  time_t clock;
pthread_create(&threads[0], NULL, numberQueuer, (void*)(size_t)(1));
  pthread_mutex_init(&lock,NULL); 
	char buffer[1024];
	int clintListn = 0, cConn = 0;
	struct sockaddr_in address;
  // socket creation
	clintListn = socket(AF_INET, SOCK_STREAM, 0);
	memset(&address, '0', sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(PORT);
	bind(clintListn, (struct sockaddr*)&address , sizeof(address));

  int count = 0;
	listen(clintListn , 15);

  // Search thread the performs the prime test
  pthread_create(&threads[1], NULL, searchTask, (void*)(size_t)(1));

	while(1)
	{
    cConn = accept(clintListn, (struct sockaddr*)NULL, NULL);
    // Main thread that creates other threads and waits for clients to join
    pthread_create(&threads[2], NULL, clientTask, (void *)cConn);
  } 
    close(cConn);
 
     return 0;
}