/* HTTPServerMain.cpp */

#include<iostream>
#include<string>

#include<stdlib.h>

#include "CServer.h"

using namespace std;

int main(int argc, char* argv[])
{
	int port;
	CServer* server;

	if(argc == 2){
		port = atoi(argv[1]);
		server = new CServer(port);
	}else{
		server = new CServer();
	}

	if(server->run()){
		cerr<<"Error starting Communication Server"<<endl;
	}

	free(server);

	return 0;
}