/*CServer.cpp*/

#include<string>
#include<cstring>
#include<sstream>

#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#include "Config.h"
#include "CServer.h"
#include "Client.h"

CServer::CServer(): svrPort(Port)
{
}

CServer::CServer(int port)
{
	string funcName="CServer::CServer:";
	
	if(setPort(port)){
		cerr<<funcName<<"Failed to set port"<<endl;
	}
}
CServer::~CServer(){
	for(vector<pair<string,int> >::iterator it=clients.begin();it!=clients.end();it++){
		close((*it).second);
	}
	close(sockfd);
}

int CServer::setPort(size_t port){
	string funcName="CServer::setPort:";
	if(port<1024||port>65535){
		cerr<<funcName<<"Invalid port value.Connot bind.Enter a value between 1024 and 65535"<<endl;
		return -1;
	}

	svrPort=port;
	return 0;
}

int CServer::initSocket(){
	string funcName="initSocket:";
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0){
		cerr<<funcName<<"Failed to create socket"<<endl;
		return -1;
	}
	servAddr.sin_family=AF_INET;
	servAddr.sin_port=htons(svrPort);
	servAddr.sin_addr.s_addr=INADDR_ANY;
	
	if((bind(sockfd,(struct sockaddr*)&servAddr,sizeof(servAddr)))<0){
		cerr<<funcName<<"Failed to bind to port"<<endl;
		return -1;
	}

	if(listen(sockfd,BackLog)){
		cerr<<funcName<<"Listen on port:"<<svrPort<<endl;
		return -1;
	}
	return 0;
}

int CServer::run(){
	cout<<"Server is running."<<endl;
	string funcName="run:";
	if(initSocket()){
		cerr<<funcName<<"Failed to initialize socket"<<endl;
		return -1;
	}
	cout<<"Server has been runned"<<endl;
	while(1){
		cliLen = sizeof(cliAddr);

		if((newsockfd = accept(sockfd, (struct sockaddr *)&cliAddr, &cliLen))<0){
			cerr<<funcName<<"Accept call failed"<<endl;
			return -1;
		}
		if(handleRequest()<0){
			cerr<<funcName<<"failed to handle request"<<endl;
			return -1;
		}
	}
	return 0;
}

int CServer::handleRequest(){
	string funcName="handleRequest:";
	client=new Client();
	
	if(recvRequest()<0){
		cerr<<funcName<<"failed to receive message from client"<<endl;
		return -1;
	}

	if(parseRequest()<0){
		cerr<<funcName<<"failed to parse request"<<endl;
		return -1;
	}
	
	if(client->getType()==CLIENT){
		client->printUserLogin();
		addUser();
	}else if(client->getType()==MESSAGE){
		if(fork()==0){
			client->printRequest();
			if(processRequest()<0){
				cerr<<funcName<<"User not login"<<endl;
				exit(-1);
			}
			if(prepareResponse()){
				cerr<<funcName<<"Preparing reply failed"<<endl;
				exit(-1);
			}
			if(sendResponse()){
				cerr<<funcName<<"Sending reply failed"<<endl;
				exit(-1);
			}
			exit(0);
		}
		close(newsockfd);
	}
	return 0;
}

int CServer::recvRequest(void){
	string funcName = "recvRequest: ";
	int recvLength;
	char* buf = new char[BUF_SIZE];
	memset(buf, '\0', BUF_SIZE);

	if(!(recvLength = recv(newsockfd, buf, BUF_SIZE, 0))){
		cerr<<funcName<<"Failed to receive request (blocking)"<<endl;
		return -1;
	}
	client->addData(buf,recvLength);

	while(1){
		memset(buf, '\0', BUF_SIZE);
		recvLength = recv(newsockfd, buf, BUF_SIZE, MSG_DONTWAIT);

		if(recvLength < 0){
			if(errno == EWOULDBLOCK || errno == EAGAIN){
				break;
			} else {
				cerr<<funcName<<"Failed receiving request (nonblocking)"<<endl;
				return -1;
			}
		}
	
		client->addData(buf, recvLength);

		if(recvLength<BUF_SIZE)
			break;
	}

	return 0;
}

int CServer::parseRequest(void){
	string funcName="parseRequest:";
	if(client->parseRequest()<0){
		cerr<<funcName<<"failed to parse request"<<endl;
		return -1;
	}
	return 0;
}

int CServer::processRequest(void){
	string funcName="processRequest:";
	string name=client->getReceiver();
	if((clientsock=getUser(name))<0){
		cerr<<funcName<<"User not login"<<endl;
		return -1;
	}
	return 0;
}

int CServer::prepareResponse(void){
	string funcName="prepareResponse:";
	if(client->prepareResponse()<0){
		cerr<<funcName<<"failed to prepare response"<<endl;
		return -1;
	}
	return 0;
}

int CServer::sendResponse(void){
	string funcName="sendResponse:";
	if(client->sendData(clientsock)<0){
		cerr<<funcName<<"failed to send data"<<endl;
		return -1;
	}
	return 0;
}

void CServer::addUser(){
	clients.push_back(make_pair(client->getUser(),newsockfd));
}
int CServer::getUser(string name){
	for(vector<pair<string,int> >::iterator it=clients.begin();it!=clients.end();it++)
	{
		if((*it).first==name)
			return (*it).second;
	}
	return -1;
}
