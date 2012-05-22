//LINUX ONLY, WORKING ON WINDOWS VERSION
#ifndef __EASYSOCK_HPP__
#define __EASYSOCK_HPP__
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <vector>
#include <unistd.h>
namespace bstd{
	class eserver{
		public:
			int sockfd, portno, n;
			 socklen_t clilen;
			std::vector<unsigned int> newsockfd;
			char *buffer;
			int bs;
			struct sockaddr_in serv_addr, cli_addr;
			void err(char *msg){
			  printf("%s",msg);
			}
			eserver(int buffersize, int port){
				buffer = new char[buffersize];
				bs = buffersize;
				sockfd = socket(AF_INET, SOCK_STREAM, 0);
				if (sockfd < 0){
					err((char*)"ERROR opening socket\n");
				}
				bzero((char *) &serv_addr, sizeof(serv_addr));
				portno = port;
				serv_addr.sin_family = AF_INET;
				serv_addr.sin_port = htons(portno);
				serv_addr.sin_addr.s_addr = INADDR_ANY;
				if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
					err((char*)"ERROR on binding\n");
				
			}
			void Listen(){
				listen(sockfd,5);
			}
			int Accept(){
				clilen = sizeof(cli_addr);
				newsockfd.push_back(accept(sockfd, (struct sockaddr *) &cli_addr, &clilen));
				return newsockfd.size()-1;
			}
			char * Read(int index,int amt){
				bzero(buffer,bs);
				n = read(newsockfd[index],buffer,amt);
				if(n<0){
					err((char*)"ERROR reading from socket.\n");
					buffer = (char*)"";
				}
				return buffer;
			}
			void Send(int index,char * msg){
				n = write(newsockfd[index],msg,strlen(msg));
				 if (n < 0)
					err((char*)"ERROR writing to socket\n");
			}
			void Send(int index,char * msg, int len){
				n = write(newsockfd[index],msg,len);
				 if (n < 0)
					err((char*)"ERROR writing to socket\n");
			}
			void Close(){
				close(sockfd);
			}
			~eserver(){
				delete[] buffer;
			}
	};
	class eclient{
		public:
			int sockfd, portno, n;
			struct sockaddr_in serv_addr;
			struct hostent *server;
			int bs;
			char *buffer;
			void err(char *msg){
			  printf("%s",msg);
			}
			eclient(int buffersize, int portno, char* hostname){
				bs = buffersize;
				buffer = new char[buffersize];
				sockfd = socket(AF_INET, SOCK_STREAM, 0);
				server = gethostbyname(hostname);
				bzero((char *) &serv_addr, sizeof(serv_addr));
				serv_addr.sin_family = AF_INET;
				bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
				serv_addr.sin_port = htons(portno);
			}	
			int Connect(){
				if (connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0){
					err((char*)"ERROR connecting\n");
					return -1;
				}
				return 0;			
			}
			void Send(char * msg){
				n = write(sockfd,msg,strlen(msg));
				 if (n < 0)
					err((char*)"ERROR writing to socket\n");
			}
			void Send(char * msg, int len){
				n = write(sockfd,msg,len);
				 if (n < 0)
					err((char*)"ERROR writing to socket\n");
			}
			char * Read(int amt){
				bzero(buffer,bs);
				n = read(sockfd,buffer,amt);
				if (n < 0)
					err((char*)"ERROR reading from socket\n"); 
				return buffer;
			}
			~eclient(){
				delete[] buffer;
			}
	};
}
#endif
