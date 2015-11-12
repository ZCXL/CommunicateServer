/*Info.cpp*/
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <cstring>

#include "Info.h"
#include "Json.h"
#include "JsonObject.h"
#include "Global.h"
Info::Info()
{
}

Info::Info(string d){
	string funcName="Info:";
	if(parseData(d)<0){
		cerr<<funcName<<"failed to parse data"<<endl;
	}
}
Info::Info(Info& i){
	setSender(i.getSender());
	setReceiver(i.getReceiver());
	setInfo(i.getInfo());
	setTime(i.getTime());
}

Info::~Info()
{
}

void Info::setSender(string s){
	sender=s;
}

string Info::getSender(){
	return sender;
}

void Info::setReceiver(string r){
	receiver=r;
}

string Info::getReceiver(){
	return receiver;
}

void Info::setInfo(string i){
	info=i;
}

string Info::getInfo(){
	return info;
}

void Info::setTime(string t){
	time=t;
}

string Info::getTime(){
	return time;
}

void Info::setUser(string u){
	user=u;
}
string Info::getUser(){
	return user;
}

void Info::setType(MessageType t){
	type=t;
}
MessageType Info::getType(){
	return type;
}

int Info::parseData(string d){
	string funcName="parseData:";
	
	char *p;
	int len=d.length();
	p=(char*)malloc(sizeof(char)*len);
	strcpy(p,d.c_str());
	
	PJsonObject object=jsonObject(p);
	
	if(getString(object,"user")!=NULL){
		type=CLIENT;
		char *u=getString(object,"user");
		user=string(u);
		return 0;
	}

	type=MESSAGE;
	char *s=getString(object,"sender");
	char *t=getString(object,"time");
	char *r=getString(object,"receiver");
	char *i=getString(object,"info");
	if(s==NULL||t==NULL||r==NULL||i==NULL){
		cerr<<funcName<<"failed to parse data"<<endl;
		return -1;
	}
	sender=string(s);
	receiver=string(r);
	time=string(t);
	info=string(i);
	jsonFree(object);
	return 0;
}

int Info::produceData(){
	ostringstream dataStream;
	dataStream<<"{";
	if(type==MESSAGE){
		dataStream<<"\"time\":\""<<time<<"\",";
		dataStream<<"\"sender\":\""<<sender<<"\",";
		dataStream<<"\"receiver\":\""<<receiver<<"\",";
		dataStream<<"\"info\":\""<<info<<"\"";
	}else{
		dataStream<<"\"user\":\""<<user<<"\"";
	}
	dataStream<<"}";
	data=dataStream.str();
	return 0;
}

size_t Info::getDataSize(){
	return data.length();
}

string* Info::getDataPtr(){
	return &data;
}
