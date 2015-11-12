/***********************************************/
/* This class is used to create a new client  */
/* pointed to a socket connected to server    */
/* Author:zhuchao 2015-11-10                 */
/**********************************************/

#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include "Global.h"
#include "Info.h"

using namespace std;
class Client{
	public:
		Client();
		Client(int fd);
		~Client();	
		
		void printRequest();
		void printUserLogin();
		void addData(const char*,const int &);
		
		int parseRequest();
		int prepareResponse();
		
		MessageType getType();
		string getUser();
		string getReceiver();
		int sendData(int);	
	private:
		int sockfd;
		Info *info;
		string data;		
};
#endif
