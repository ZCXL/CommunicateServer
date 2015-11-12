/*Client.cpp*/
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <string.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "Global.h"
#include "Client.h"
#include "Info.h"
Client::Client()
{
}
Client::Client(int fd)
{
	string funcName="Client:";
	sockfd=fd;
}
Client::~Client()
{
	if(info!=NULL)
		delete info;
	close(sockfd);
}

void Client::printRequest(){
	cout<<"Time:"<<(*info).getTime()<<endl<<"From:"<<(*info).getSender()<<endl<<"To:"<<(*info).getReceiver()<<endl<<"Info:"<<(*info).getInfo()<<endl;
}
void Client::printUserLogin(){
	cout<<"User login in"<<endl<<"Name:"<<info->getUser()<<endl;
}
void Client::addData(const char*buf,const int &len){
	data.append(buf,len);
}

int Client::parseRequest(){
	string funcName="parseRequest:";
	info=new Info();
	if((info->parseData(data))<0){
		cerr<<funcName<<"failed to parse request"<<endl;
		return -1;
	}
	return 0;
}

int Client::prepareResponse(){
	string funcName="prepareRequest:";
	if((info->produceData())<0){
		cerr<<funcName<<"failed to produce data"<<endl;
		return -1;
	}
	return 0;
}

MessageType Client::getType(){
	if(info!=NULL)
		return info->getType();
	return UNKNOWN;
}
string Client::getUser(){
	if(info!=NULL)
		return info->getUser();
	return "";
}
string Client::getReceiver(){
	if(info!=NULL)
		return info->getReceiver();
	return "";
}
int Client::sendData(int fd){
	string funcName="sendData:";
	
	size_t size=(*info).getDataSize();
	string* pData=(*info).getDataPtr();
	
	char *buf=new char[size];
	memset(buf,'\0',size);
	memcpy(buf,pData->data(),size);
	
	if((send(fd, buf,size, 0))<0){
		cerr<<funcName<<"Sending response failed"<<endl;
		return -1;
	}
	
	delete buf;
	
	return 0;
}
