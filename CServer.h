/**********************************************************/
/* This class CServer is used to get communication request*/
/* handle data and so on                                  */
/* Author:zhuchao 2015-11-10                              */
/**********************************************************/
#ifndef _C_SERVER_H_
#define _C_SERVER_H_

#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>
#include "Client.h"
using namespace std;
class CServer{
	public:
		CServer();
		CServer(int);
		~CServer();

		int run(void);
		int setPort(size_t);

		int initSocket(void);
		int handleRequest(void);
		int recvRequest(void);
		int parseRequest(void);
		int processRequest(void);

		int prepareResponse(void);
		int sendResponse(void);
		void addUser(void);
		int getUser(string);
	private:
		size_t svrPort;
		int sockfd,newsockfd,clientsock;//server's socket
		socklen_t cliLen;
		struct sockaddr_in servAddr,cliAddr;
		vector<pair<string,int> > clients;//clients's socket
		Client* client;
};/*CServer*/

#endif
