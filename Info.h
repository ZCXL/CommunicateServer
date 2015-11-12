/*********************************************************/
/* This class is used to save the infomation from client*/
/* Author:zhuchao 2015-11-10                            */
/********************************************************/

#ifndef _INFO_H_
#define _INFO_H_
#include <string>
#include <cstring>
#include <iostream>
#include "Global.h"
#include "Json.h"
#include "JsonObject.h"
using namespace std;
class Info{
	public:
		Info();
		Info(Info&);
		Info(string);
		~Info();
		void setTime(string);
		string getTime(void);
		void setSender(string);
		string getSender(void);
		void setReceiver(string);
		string getReceiver(void);
		void setInfo(string);
		string getInfo(void);
		void setUser(string);
		string getUser(void);
		void setType(MessageType);
		MessageType getType();
		int parseData(string);
		int produceData();
		
		size_t getDataSize();
		string *getDataPtr();
	private:
		string time;
		string sender;
		string receiver;
		string info;
		string user;
		
		string data;
		MessageType type;
};
#endif
