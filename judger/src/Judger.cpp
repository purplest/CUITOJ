//============================================================================
// Name        : judger.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Judger.h"

const int Judger::PORT = 3790;
const int Judger::MAXTHREAD = 100;
const int Judger::MAXBUFFSIZE = 1024;

Judger::Judger(){
	this->connect();
	this->program = new Program();
}

Judger::~Judger(){
	close(this->client_msg);
	delete this->program;
}

void Judger::connect(){
	if ((serversock=socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
		LOG("create socket error!");
		exit(1);
	}
	int on = 1;
	if ((setsockopt(serversock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0){
		LOG("set sockopt error");
		exit(1);
	}
	struct sockaddr_in server_addr;
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(PORT);
	server_addr.sin_addr.s_addr=INADDR_ANY;
	memset(&server_addr.sin_zero,0,8);
	if (bind(serversock,(struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1){
		LOG("bind error");
		exit(1);
	}
	if (listen(serversock, MAXTHREAD) == -1){
		LOG("listen error");
		exit(1);
	}
}

void Judger::server(){
    while(1){
        socklen_t sin_size = sizeof(struct sockaddr_in);
        if ((client_msg=accept(serversock,(struct sockaddr *)&remote_addr,&sin_size))==-1){
            LOG("accept error");
            continue;
        }
        LOG(string("receive a connection from ")+string(inet_ntoa(remote_addr.sin_addr)));

        char buff[MAXBUFFSIZE];
        if (recv(client_msg, buff, MAXBUFFSIZE, 0)<0){
            LOG("reading error");
            continue;
        }
        //printf("the message is:\n%s\n", buff);
        int runID, problemID, contestID;
        sscanf(buff,"%d %d %d",&runID, &problemID, &contestID);
//        printf("pid = %d, runid = %d\n", problemID, runID);
        LOG(string("recieved pid = "+this->toString(runID)));
        Status *status = new Status();
        status->setStatusId(runID);
		status->setContestId(contestID);
		status->setProblemId(problemID);
        this->program->setStatus(status);
        this->program->start();
//        printf("sleep\n");
        usleep(1000000);
        LOG("Judge over.");
        LOG("---------------------------------------------");
//        printf("Judge over.\n");
//        printf("wake up\n");
    }
}
