/*
 * Program.cpp
 *
 *  Created on: 2013-2-7
 *      Author: purple
 */
#include "Program.h"

using namespace std;
using namespace sql;

void Program::setStatus(Status* status) {
	this->status = status;
}

Program::Program(){
	this->mysql = new Mysql();
}

Program::~Program(){
	delete this->mysql;
}

void Program::prepare(){
	this->mysql->update_status(this->status);
	if (LANG_JAVA==this->status->getLanguage()){
		this->status->setFilename("Main.java");
		this->status->setTargetFileName("Main.class");
	}
	else{
		this->status->setFilename(this->toString(this->status->getStatusId())+this->src_extension[this->status->getLanguage()]);
		this->status->setTargetFileName(this->toString(this->status->getStatusId())+this->exc_extension[this->status->getLanguage()]);
	}
	this->status->setTargetFile("./"+this->status->getRunPath()+"/"+this->status->getTargetFileName());
	this->status->setSourceFile("./"+this->status->getRunPath()+"/"+this->status->getFilename());
}

void Program::saveState(){
	this->mysql->saveStatusInfo(this->status);
	if (this->status->SINF_Accepted==this->status->getStatusInfo()){
		if (this->status->getContestId()==0)
			this->mysql->update_problem_submit(this->status->getProblemId());
		else
			this->mysql->update_contest_submit(this->status);
	}
}

void Program::saveCEreason(){
	this->mysql->saveStatusCEreason(this->status);
}

void Program::saveTimeAndMemory(){
	this->mysql->saveStatusTimeAndMemory(this->status);
}

void Program::start(){
	this->prepare();
	this->status->setStatusInfo(this->status->SINF_COMPILING);
	this->saveState();
	this->compile = new Compile(this->status);
	this->compile->process();
	this->saveState();
//	usleep(10000000);
	if (this->status->SINF_COMPILIED_COMPLETE==this->status->getStatusInfo()){
		this->status->setStatusInfo(this->status->SINF_RUNING);
		this->saveState();
//		usleep(10000000);
		this->run = new Run(this->status);
		this->run->process();
		delete this->run;
		this->saveTimeAndMemory();
	}
	else{
		this->saveCEreason();
	}
//	cout<<"status Info is:"<<this->status->getStatusInfo()<<endl;
	this->saveState();
//	usleep(10000000);
	delete this->compile;
}
