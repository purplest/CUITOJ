/*
 * Compile.cpp
 *
 *  Created on: 2013-2-7
 *      Author: purple
 */
#include "Compile.h"

using namespace std;


Compile::Compile(Status *status){
	this->status = status;

	FILE *fp = fopen(this->status->getSourceFile().c_str(),"w");
	//while (fp==NULL) fp = fopen(file.c_str(),"w");
	if (fp!=NULL){
		fputs(this->status->getStatusCode().c_str(),fp);
		fclose(fp);
	}
	this->status->setStatusInfo(this->status->SINF_COMPILING);

	this->c_arg=" -w ";
	this->java_arg=" -g:none -Xlint ";
}

Compile::~Compile(){
	this->clearPath();

	this->status = NULL;
}

void Compile::clearPath(){
	string remove = "rm ./"+this->status->getRunPath()+"/*";
	system(remove.c_str());
}

bool Compile::checkfile(){
	DIR *dir;
	dirent *file;
	string case_dir = "./"+this->status->getRunPath()+"/";
	// tips: need try...catch ?
	dir = opendir(case_dir.c_str());
	while ((file = readdir(dir))!=NULL){
		//LOG(file->d_name);
		//LOG("target file is "+this->status->getTargetFileName());
		if (string(file->d_name)==this->status->getTargetFileName()){
			LOG("find the target file :"+this->status->getTargetFileName());
			return 1;
		}
	}
	LOG("can't find the target file :"+this->status->getTargetFileName());
	return 0;
}

void Compile::setState(){
	if (this->checkfile()){
		this->status->setStatusInfo(this->status->SINF_COMPILIED_COMPLETE);
	}
	else{
		string cePath = "./"+this->status->getRunPath()+"/ce.txt";
		FILE *ce_file = fopen(cePath.c_str(),"r");
		string reason = "";
		char ch;
		while (EOF!=(ch = fgetc(ce_file))){
			reason.push_back(ch);
		}
		this->status->setCeReason(reason);
		this->status->setStatusInfo(this->status->SINF_COMPILED_ERROR);
		LOG("the result is CE");
	}
}

void Compile::process(){
//	cout<<"process complie"<<endl;
	LOG("process complie "+this->status->getSourceFile());
	string complie_cmd;
	switch (this->status->getLanguage()){
		case LANG_C:
			complie_cmd = "gcc -o "+this->status->getTargetFile()+" "+this->status->getSourceFile()+this->c_arg+" 2> "+this->status->getCeFile();
			break;
		case LANG_CPP:
			complie_cmd = "g++ -o "+this->status->getTargetFile()+" "+this->status->getSourceFile()+this->c_arg+" 2> "+this->status->getCeFile();
			break;
		case LANG_JAVA:
			complie_cmd = "javac "+this->java_arg+this->status->getSourceFile()+" 2> "+this->status->getCeFile();
			break;
		default:
			return;
	}
	LOG(complie_cmd);
	pid_t cpid;
	int cmpstatus;
	if (0==(cpid = fork())){
//		cout<<complie_cmd<<endl;
		rlimit compile_limit;
		compile_limit.rlim_cur=compile_limit.rlim_max=this->compile_time_limited;
		setrlimit(RLIMIT_CPU,&compile_limit);
//		cout<<"begin"<<endl;
		int res = system(complie_cmd.c_str());
//		cout<<"end"<<endl;
		exit(0);
	}
	else{
		timeval time_begin, time_now;
		gettimeofday(&time_begin, NULL);
		while (1){
			usleep(100000);
			gettimeofday(&time_now, NULL);
			this->compile_time_used = time_now.tv_sec-time_begin.tv_sec;
			if (waitpid(cpid,&cmpstatus,WNOHANG)==0){
				//check for times
				if (this->compile_time_used > this->compile_time_limited){
					this->status->setStatusInfo(this->status->SINF_TOO_MUCH_TIME);
					string kill_cmd = "kill -9 "+this->toString(cpid);
					system(kill_cmd.c_str());
					waitpid(cpid,&cmpstatus,NULL);
				}
//				cout<<"wait"<<endl;
				continue;
			}
			else if (WIFSIGNALED(cmpstatus)&&WTERMSIG(cmpstatus)!=0){
				//with sth wrong
				this->status->setStatusInfo(this->status->SINF_WITH_SOMETHING_WRONG);
			}
			if (WIFEXITED(cmpstatus)){
				//ok
				this->status->setStatusInfo(this->status->SINF_COMPILIED_COMPLETE);
			}
			this->setState();
//			cout<<"end of complie"<<endl;
			LOG("end of complie");
			return;
		}
	}
}
