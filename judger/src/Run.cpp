/*
 * Run.cpp
 *
 *  Created on: 2013-2-10
 *      Author: purple
 */

#include "Run.h"

using namespace std;

Run::Run(Status *status){
	this->initerror();
	this->status = status;
	this->get_all_case_file();
	this->out_file = "./"+this->status->getRunPath()+"/out.txt";
	this->result_file = "./"+this->status->getRunPath()+"/result.txt";
	this->info_file = "./"+this->status->getRunPath()+"/info.txt";

	this->run_exter_time = 10;//should control in config.ini
	this->status->setTimeUsed(0);
	this->status->setMemoryUsed(0);
}

Run::~Run(){
	this->status = NULL;
}

void Run::saveResultToFile(int res){
//	LOG("save to file "+this->toString(res));
	FILE *fp = fopen(this->result_file.c_str(),"w");
	if (NULL!=fp){
		fprintf(fp, "%d\n", res);
		fclose(fp);
	}
}

int Run::readResultFromFile(){
	FILE *fp = fopen(this->result_file.c_str(),"r");
	int res = -1;
	if (NULL!=fp){
		fscanf(fp, "%d\n", &res);
		fclose(fp);
	}
	return res;
}

void Run::saveInfoToFile(int time, int memory){
	FILE *fp = fopen(this->info_file.c_str(),"w");
		if (NULL!=fp){
			fprintf(fp, "%d %d\n", time, memory);
			fclose(fp);
		}
}

pair<int,int> Run::readInfoFromFile(){
	FILE *fp = fopen(this->info_file.c_str(),"r");
	int time = -1, memory;
	if (NULL!=fp){
		fscanf(fp, "%d %d\n", &time, &memory);
		fclose(fp);
	}
	return make_pair(time,memory);
}

void Run::get_all_case_file(){
	this->case_file.clear();
	DIR *dir;
	dirent *file;
	this->case_dir = "./"+this->status->getCasePath()+"/"+this->toString(this->status->getProblemId())+"/in/";
	// tips: need try...catch ?
	dir = opendir(case_dir.c_str());
	while ((file = readdir(dir))!=NULL){
		if (strncmp(file->d_name,".",1))
			this->case_file.push_back(file->d_name);
	}
//	for (int i = 0 ; i < this->case_file.size() ; i++ )
//		LOG("case file "+this->toString(i)+" is "+this->case_file[i]);
}

void Run::run(int case_num){
	//this->saveResultToFile(this->status->SINF_RUNING);
	pid_t wpid, cpid;
	int run_status;
	rlimit runtime_limited;
	runtime_limited.rlim_cur = runtime_limited.rlim_max = (this->status->getTimeLimited()-this->status->getTimeUsed()+999)/1000+this->run_exter_time;
	if (0==(wpid = fork())){
		setrlimit(RLIMIT_CPU,&runtime_limited);
		if (0==(cpid = fork())){
			rlimit time_limited;
			time_limited.rlim_cur = min(this->status->getCaseTimeLimited(),this->status->getTimeLimited()-this->status->getTimeUsed());
			time_limited.rlim_cur = (time_limited.rlim_cur+999)/1000;
			if (0>=time_limited.rlim_cur){
				time_limited.rlim_cur=1;
			}
			time_limited.rlim_max = time_limited.rlim_cur+1;
//			LOG("time_limited.rlim_cur is : "+this->toString(time_limited.rlim_cur));
//			LOG("time_limited.rlim_max is : "+this->toString(time_limited.rlim_max));
			setrlimit(RLIMIT_CPU,&time_limited);//time limited

			rlimit output_limited;
			output_limited.rlim_cur=output_limited.rlim_max = 10*1024*1024;
			setrlimit(RLIMIT_FSIZE, &output_limited);//output limited
			string casePath = this->case_dir+this->case_file[case_num];
			LOG("run the case of "+casePath);
//			LOG("/usr/bin/java,java,-Djava.security.manager,-Djava.security.policy=java.policy,-client,-classpath"+string("./"+this->status->getRunPath())+"Main");
			freopen(casePath.c_str(),"r",stdin);
			freopen(this->out_file.c_str(),"w",stdout);
			setuid(1234);//set user;
			ptrace(PTRACE_TRACEME,0,NULL,NULL);

			switch(this->status->getLanguage()){
				case LANG_C:
				case LANG_CPP:
					execl(this->status->getTargetFile().c_str(), this->status->getTargetFile().c_str(), NULL);
					break;
				case LANG_JAVA:
					execl("/usr/bin/java","/usr/bin/java","-Djava.security.manager","-Djava.security.policy=java.policy","-client","-classpath",(string("./"+this->status->getRunPath())).c_str(),"Main",NULL);
					break;
			}
			exit(0);
		}
		else{
			rusage run_info;
			user_regs_struct regs;
			bool execute = 0;
			while (1){
				wait4(cpid, &run_status, NULL, &run_info);
//				LOG("running");
				this->status->setCaseTimeUsed((run_info.ru_utime.tv_sec+run_info.ru_stime.tv_sec)*1000+(run_info.ru_utime.tv_usec+run_info.ru_stime.tv_usec)/1000);
				if (this->status->getCaseTimeUsed()>this->status->getCaseTimeLimited()){
	//				cout<<"TLE"<<endl;
					LOG("Time used:"+this->toString(this->status->getCaseTimeUsed()));
					LOG("Memory used:"+this->toString(this->status->getMemoryUsed()));
					LOG(string("case file of ")+this->toString(case_num)+string(" is case-TLE"));
					LOG("TLE by type 1");
					this->status->setStatusInfo(this->status->SINF_Time_Limit_Exceeded);
					this->saveResultToFile(this->status->SINF_Time_Limit_Exceeded);
					this->saveInfoToFile(this->status->getCaseTimeUsed(),this->status->getMemoryUsed());
					ptrace(PTRACE_KILL,cpid,NULL,NULL);
					exit(0);
				}
				if (this->status->getMemoryUsed()<getpagesize()*run_info.ru_minflt/1024){
					this->status->setMemoryUsed(getpagesize()*run_info.ru_minflt/1024);
//					LOG(string("page size is ")+this->toString(getpagesize()));
//					LOG(string("page is ")+this->toString(run_info.ru_minflt));
				}
				if (WIFEXITED(run_status)){
					if (0!=WEXITSTATUS(run_status)){
						LOG("Time used:"+this->toString(this->status->getCaseTimeUsed()));
						LOG("Memory used:"+this->toString(this->status->getMemoryUsed()));
						LOG(string("case file of ")+this->toString(case_num)+string(" is RE"));
						LOG("RE type 1");
						this->status->setStatusInfo(this->status->SINF_Runtime_Error);
						this->saveResultToFile(this->status->SINF_Runtime_Error);
						this->saveInfoToFile(this->status->getCaseTimeUsed(),this->status->getMemoryUsed());
						exit(0);
					}
					else{
						if (this->status->getMemoryUsed()>this->status->getMemoryLimited()){
			//				cout<<"MLE"<<endl;
							LOG("Time used:"+this->toString(this->status->getCaseTimeUsed()));
							LOG("Memory used:"+this->toString(this->status->getMemoryUsed()));
							LOG(string("case file of ")+this->toString(case_num)+string(" is MLE"));
							LOG("memory used is "+this->toString(this->status->getMemoryUsed()));
							this->status->setStatusInfo(this->status->SINF_Memory_Limit_Exceeded);
							this->saveResultToFile(this->status->SINF_Memory_Limit_Exceeded);
							this->saveInfoToFile(this->status->getCaseTimeUsed(),this->status->getMemoryUsed());
							ptrace(PTRACE_KILL,cpid,NULL,NULL);
							exit(0);
						}
						else{
							LOG(string("case file of ")+this->toString(case_num)+string(" is over"));
							LOG("Time used:"+this->toString(this->status->getCaseTimeUsed()));
							LOG("Memory used:"+this->toString(this->status->getMemoryUsed()));
							this->status->setStatusInfo(this->status->SINF_RUNING);
							this->saveResultToFile(this->status->SINF_RUNING);
							this->saveInfoToFile(this->status->getCaseTimeUsed(),this->status->getMemoryUsed());
							exit(0);
						}
					}
				}
				if (WIFSIGNALED(run_status)&&SIGTRAP!=WTERMSIG(run_status)){
					int signal = WTERMSIG(run_status);
					LOG("the signal is "+this->toString(signal));
	//				cout<<"signal is "<<signal<<endl;
					switch (signal){
						case SIGXCPU:
	//						cout<<"TLE"<<endl;
							LOG("Time used:"+this->toString(this->status->getCaseTimeUsed()));
							LOG("Memory used:"+this->toString(this->status->getMemoryUsed()));
							LOG(string("case file of ")+this->toString(case_num)+string(" is TLE"));
							LOG("in SIGXCPU, killed by signal, TLE by type 2");
							this->status->setStatusInfo(this->status->SINF_Time_Limit_Exceeded);
							this->saveResultToFile(this->status->SINF_Time_Limit_Exceeded);
							this->status->setTimeUsed(this->status->getTimeLimited()+1);
							this->saveInfoToFile(this->status->getCaseTimeUsed(),this->status->getMemoryUsed());
							break;
						case SIGXFSZ:
	//						cout<<"OLE"<<endl;
							LOG("Time used:"+this->toString(this->status->getCaseTimeUsed()));
							LOG("Memory used:"+this->toString(this->status->getMemoryUsed()));
							LOG(string("case file of ")+this->toString(case_num)+string(" is OLE"));
							LOG("in SIGXFSZ, killed by signal");
							this->status->setStatusInfo(this->status->SINF_Output_Limit_Exceeded);
							this->saveResultToFile(this->status->SINF_Output_Limit_Exceeded);
							this->saveInfoToFile(this->status->getCaseTimeUsed(),this->status->getMemoryUsed());
							break;
						default:
	//						cout<<"RE"<<endl;
							LOG("Time used:"+this->toString(this->status->getCaseTimeUsed()));
							LOG("Memory used:"+this->toString(this->status->getMemoryUsed()));
							LOG(string("case file of ")+this->toString(case_num)+string(" is RE"));
							LOG("RE type 2");
							LOG("time is "+this->toString(this->status->getTimeUsed()));
							this->status->setStatusInfo(this->status->SINF_Runtime_Error);
							this->saveResultToFile(this->status->SINF_Runtime_Error);
							this->saveInfoToFile(this->status->getCaseTimeUsed(),this->status->getMemoryUsed());
							break;
					}
					ptrace(PTRACE_KILL,cpid,NULL,NULL);
					exit(0);
				}
				else if (WIFSTOPPED(run_status)&&SIGTRAP!=WSTOPSIG(run_status)){
					int signal = WSTOPSIG(run_status);
					LOG("the signal is "+this->toString(signal));
	//				cout<<"signal is "<<signal<<endl;
					switch (signal){
						case SIGXCPU:
	//						cout<<"TLE"<<endl;
							LOG("Time used:"+this->toString(this->status->getCaseTimeUsed()));
							LOG("Memory used:"+this->toString(this->status->getMemoryUsed()));
							LOG(string("case file of ")+this->toString(case_num)+string(" is TLE"));
							LOG("in SIGXCPU, killed by signal, TLE by type 3");
							this->status->setStatusInfo(this->status->SINF_Time_Limit_Exceeded);
							this->saveResultToFile(this->status->SINF_Time_Limit_Exceeded);
							this->status->setTimeUsed(this->status->getTimeLimited()+1);
							this->saveInfoToFile(this->status->getCaseTimeUsed(),this->status->getMemoryUsed());
							break;
						case SIGXFSZ:
	//						cout<<"OLE"<<endl;
							LOG("Time used:"+this->toString(this->status->getCaseTimeUsed()));
							LOG("Memory used:"+this->toString(this->status->getMemoryUsed()));
							LOG(string("case file of ")+this->toString(case_num)+string(" is OLE"));
							LOG("in SIGXFSZ, killed by signal");
							this->status->setStatusInfo(this->status->SINF_Output_Limit_Exceeded);
							this->saveResultToFile(this->status->SINF_Output_Limit_Exceeded);
							this->saveInfoToFile(this->status->getCaseTimeUsed(),this->status->getMemoryUsed());
							break;
						default:
	//						cout<<"RE"<<endl;
							LOG("Time used:"+this->toString(this->status->getCaseTimeUsed()));
							LOG("Memory used:"+this->toString(this->status->getMemoryUsed()));
							LOG(string("case file of ")+this->toString(case_num)+string(" is RE"));
							LOG("RE type 3");
							this->status->setStatusInfo(this->status->SINF_Runtime_Error);
							this->saveResultToFile(this->status->SINF_Runtime_Error);
							this->saveInfoToFile(this->status->getCaseTimeUsed(),this->status->getMemoryUsed());
							break;
					}
					ptrace(PTRACE_KILL,cpid,NULL,NULL);
					exit(0);
				}
				else if ((run_status>>8)!=5&&(run_status>>8)>0){
					LOG("Time used:"+this->toString(this->status->getCaseTimeUsed()));
					LOG("Memory used:"+this->toString(this->status->getMemoryUsed()));
					LOG(string("case file of ")+this->toString(case_num)+string(" is RE"));
					LOG("RE type 4");
					this->status->setStatusInfo(this->status->SINF_Runtime_Error);
					this->saveResultToFile(this->status->SINF_Runtime_Error);
					this->saveInfoToFile(this->status->getCaseTimeUsed(),this->status->getMemoryUsed());
					ptrace(PTRACE_KILL,cpid,NULL,NULL);
					exit(0);
				}
				ptrace(PTRACE_GETREGS,cpid,NULL,&regs);
				if (regs.orig_eax==SYS_execve&&!execute){
					execute = true;
				}
				else{
					if (LANG_JAVA==this->status->getLanguage()){
						if (syscall_java[regs.orig_eax]){
							LOG("Invalid system call: "+this->toString(regs.orig_eax));
							LOG("memory used is "+this->toString(this->status->getMemoryUsed()));
							this->status->setStatusInfo(this->status->SINF_RESTRICTED_FUNCTION);
							this->saveResultToFile(this->status->SINF_RESTRICTED_FUNCTION);
							this->saveInfoToFile(this->status->getCaseTimeUsed(),this->status->getMemoryUsed());
							ptrace(PTRACE_KILL,cpid,NULL,NULL);
							exit(0);
						}
					}
					else if (syscall_other[regs.orig_eax]){
						LOG("Invalid system call: "+this->toString(regs.orig_eax));
						LOG("memory used is "+this->toString(this->status->getMemoryUsed()));
						this->status->setStatusInfo(this->status->SINF_RESTRICTED_FUNCTION);
						this->saveResultToFile(this->status->SINF_RESTRICTED_FUNCTION);
						this->saveInfoToFile(this->status->getCaseTimeUsed(),this->status->getMemoryUsed());
						ptrace(PTRACE_KILL,cpid,NULL,NULL);
						exit(0);
					}
				}
				if (this->status->getMemoryUsed()>this->status->getMemoryLimited()){
	//				cout<<"MLE"<<endl;
					LOG("Time used:"+this->toString(this->status->getCaseTimeUsed()));
					LOG("Memory used:"+this->toString(this->status->getMemoryUsed()));
					LOG(string("case file of ")+this->toString(case_num)+string(" is MLE"));
					LOG("memory used is "+this->toString(this->status->getMemoryUsed()));
					this->status->setStatusInfo(this->status->SINF_Memory_Limit_Exceeded);
					this->saveResultToFile(this->status->SINF_Memory_Limit_Exceeded);
					this->saveInfoToFile(this->status->getCaseTimeUsed(),this->status->getMemoryUsed());
					ptrace(PTRACE_KILL,cpid,NULL,NULL);
					exit(0);
				}
				ptrace(PTRACE_SYSCALL,cpid,NULL,NULL);
			}
			exit(0);
		}
		exit(0);
	}
	else{
		int status;
		timeval time_begin, time_now;
		gettimeofday(&time_begin, NULL);
//		LOG("kill `pstree -p "+this->toString(wpid)+" | sed 's/(/\\n(/g' | grep '(' | sed 's/(\\(.*\\)).*/\\1/' | tr \"\\n\" \" \"`");
		while (1){
			usleep(100000);
			gettimeofday(&time_now, NULL);
			this->run_time_used = time_now.tv_sec-time_begin.tv_sec;
			if (waitpid(wpid,&status,WNOHANG)==0){
				if (this->run_time_used>runtime_limited.rlim_max){
					LOG("the run_time_used is "+this->toString(this->run_time_used));
					LOG("the runtime_limited.rlim_max is"+this->toString(runtime_limited.rlim_max));
					LOG(string("case file of ")+this->toString(case_num)+string(" run time is too much"));
					string cmd_str = "kill `pstree -p "+this->toString(wpid)+" | sed 's/(/\\n(/g' | grep '(' | sed 's/(\\(.*\\)).*/\\1/' | tr \"\\n\" \" \"`";
					LOG(cmd_str);
					system(cmd_str.c_str());
					this->status->setStatusInfo(this->status->SINF_Judge_Error);
					this->saveResultToFile(this->status->SINF_Judge_Error);
					this->saveInfoToFile(this->status->getCaseTimeUsed(),this->status->getMemoryUsed());
					waitpid(wpid,&status,NULL);
					return;
				}
			}
			else if (WIFSIGNALED(status)&&WTERMSIG(status)!=0){
				LOG(string("Sth is wrong"));
				string cmd_str = "kill `pstree -p "+this->toString(wpid)+" | sed 's/(/\\n(/g' | grep '(' | sed 's/(\\(.*\\)).*/\\1/' | tr \"\\n\" \" \"`";
				LOG(cmd_str);
				system(cmd_str.c_str());
				this->status->setStatusInfo(this->status->SINF_Judge_Error);
				this->saveResultToFile(this->status->SINF_Judge_Error);
				this->saveInfoToFile(this->status->getCaseTimeUsed(),this->status->getMemoryUsed());
				waitpid(wpid,&status,NULL);
				return;
			}
			if (WIFEXITED(status)){
				waitpid(wpid,&status,NULL);
				LOG("Runned.");
				break;
			}
		}
	}
}

bool Run::compare(int case_num){
	FILE *stand, *run;
	string casePath = "./"+this->status->getCasePath()+"/"+this->toString(this->status->getProblemId())+"/out/"+this->case_file[case_num].substr(0,this->case_file[case_num].length()-2)+"out";
	stand = fopen(casePath.c_str(),"r");
	run = fopen(this->out_file.c_str(),"r");
//	cout<<casePath+"  "+this->out_file<<endl;
	bool diff = 0;
	char sch, rch;
	while (1){
		while (EOF!=(sch=fgetc(stand)) && sch=='\r');
		while (EOF!=(rch=fgetc(run)) && rch=='\r');
		if (sch==EOF||rch==EOF){
			break;
		}
		if (sch!=rch){
			diff = 1;
			break;
		}
	}
	while ((sch=='\n'||sch=='\r')&&(EOF!=(sch=fgetc(stand))));
	while ((rch=='\n'||rch=='\r')&&(EOF!=(rch=fgetc(run))));
	if (sch!=rch) diff = 1;
	fclose(stand);
	fclose(run);
	return diff;
}

bool Run::PEcompare(int case_num){
	FILE *stand, *run;
	string casePath = "./"+this->status->getCasePath()+"/"+this->toString(this->status->getProblemId())+"/out/"+this->case_file[case_num].substr(0,this->case_file[case_num].length()-2)+"out";
	stand = fopen(casePath.c_str(),"r");
	run = fopen(this->out_file.c_str(),"r");
//	cout<<casePath+"  "+this->out_file<<endl;
	bool diff = 1;
	char sch, rch;
	while (1){
		while (EOF!=(sch=fgetc(stand)) && (sch=='\r'||sch=='\n'||sch==' '));
		while (EOF!=(rch=fgetc(run)) && (rch=='\r'||rch=='\n'||rch==' '));
		if (sch==EOF||rch==EOF){
			break;
		}
		if (sch!=rch){
			diff = 0;
			break;
		}
	}
	while ((sch=='\n'||sch=='\r'||sch==' ')&&(EOF!=(sch=fgetc(stand))));
	while ((rch=='\n'||rch=='\r'||rch==' ')&&(EOF!=(rch=fgetc(run))));
	if (sch!=rch) diff = 0;
	fclose(stand);
	fclose(run);
	return diff;
}

void Run::process(){
	for ( int i = 0 ; i < this->case_file.size() ; i++ ){
//		cout<<"run the test file "<<i<<endl;
		this->run(i);
		int res = this->readResultFromFile();
		pair<int,int> pii = this->readInfoFromFile();
		LOG("res = "+this->toString(res));
		LOG("time used: "+this->toString(pii.first)+" memory used: "+this->toString(pii.second));
		if (-1==res) res = this->status->SINF_RUNING;
		this->status->setStatusInfo(res);
		this->status->setTimeUsed(this->status->getTimeUsed()+pii.first);
		this->status->setMemoryUsed(max(this->status->getMemoryUsed(),pii.second));
		if (this->status->getTimeUsed()>this->status->getTimeLimited()){
			LOG("time used:"+this->toString(this->status->getTimeUsed()));
			LOG("time limited:"+this->toString(this->status->getTimeLimited()));
			LOG(string("case file of ")+this->toString(i)+string(" is total-TLE"));
			this->status->setStatusInfo(this->status->SINF_Time_Limit_Exceeded);
		}
		LOG(string("case file of ")+this->toString(i)+string(" result is ")+this->toString(res));
		if (this->status->SINF_RUNING==this->status->getStatusInfo()){
			bool diff = this->compare(i);
			if (diff){
				bool PE = this->PEcompare(i);
				if (PE){
					LOG(string("case file of ")+this->toString(i)+string(" is PE"));
					this->status->setStatusInfo(this->status->SINF_Presentation_Error);
				}
				else{
					LOG(string("case file of ")+this->toString(i)+string(" is WA"));
					this->status->setStatusInfo(this->status->SINF_Wrong_Answer);
				}
				return;
			}
		}
		else{
			return;
		}
		LOG(string("case file of ")+this->toString(i)+string(" time used ")+this->toString(this->status->getTimeUsed())+string("ms and memory used ")+this->toString(this->status->getMemoryUsed()));
	}
	if (this->status->SINF_RUNING==this->status->getStatusInfo()){
		LOG("this submit is AC");
		this->status->setStatusInfo(this->status->SINF_Accepted);
	}
}
