/*
 * Run.h
 *
 *  Created on: 2013-2-10
 *      Author: purple
 */
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <errno.h>

#include "Log.h"
#include "Status.h"
#include "Base.h"

#ifndef RUN_H_
#define RUN_H_

using namespace std;

class Run : public Base {
public:
	Run(Status *status);
	~Run();
	void process();
private:
	void initerror(){
		memset(syscall_java,0,sizeof(syscall_java));
		memset(syscall_other,0,sizeof(syscall_other));
		syscall_other[SYS__sysctl]=true;/*syscall_other[SYS_access]=true;*/syscall_other[SYS_chdir]=true;syscall_other[SYS_chmod]=true;
		syscall_other[SYS_chown]=true;syscall_other[SYS_chroot]=true;syscall_other[SYS_clone]=true;/*syscall_other[SYS_close]=true;*/
		syscall_other[SYS_creat]=true;syscall_other[SYS_create_module]=true;syscall_other[SYS_delete_module]=true;syscall_other[SYS_fork]=true;
		/*syscall_other[SYS_getuid]=true;syscall_other[SYS_getpid]=true;*/syscall_other[SYS_getpgrp]=true;syscall_other[SYS_kill]=true;
		syscall_other[SYS_mkdir]=true;syscall_other[SYS_mknod]=true;syscall_other[SYS_mount]=true;/*syscall_other[SYS_open]=true;*/
		syscall_other[SYS_rmdir]=true;syscall_other[SYS_ptrace]=true;syscall_other[SYS_reboot]=true;syscall_other[SYS_rename]=true;
		syscall_other[SYS_restart_syscall]=true;syscall_other[SYS_select]=true;syscall_other[SYS_setgid]=true;syscall_other[SYS_setitimer]=true;
		syscall_other[SYS_setgroups]=true;syscall_other[SYS_sethostname]=true;syscall_other[SYS_setrlimit]=true;syscall_other[SYS_setuid]=true;
		syscall_other[SYS_settimeofday]=true;syscall_other[SYS_tkill]=true;syscall_other[SYS_setrlimit]=true;syscall_other[SYS_setuid]=true;
		syscall_other[SYS_vfork]=true;syscall_other[SYS_vhangup]=true;syscall_other[SYS_vserver]=true;syscall_other[SYS_wait4]=true;
		syscall_other[SYS_clock_nanosleep]=true;syscall_other[SYS_nanosleep]=true;syscall_other[SYS_pause]=true;/*syscall_other[SYS_arch_prctl]=true;*/
		/*syscall_other[SYS_write]=true;syscall_other[SYS_writev]=true;*/
//		syscall_other[SYS_accept]=true;syscall_other[SYS_bind]=true;syscall_other[SYS_connect]=true;syscall_other[SYS_listen]=true;
//		syscall_other[SYS_socket]=true;

		syscall_java[SYS__sysctl]=true;syscall_java[SYS_chdir]=true;syscall_java[SYS_chmod]=true;
		syscall_java[SYS_chown]=true;syscall_java[SYS_chroot]=true;
		syscall_java[SYS_creat]=true;syscall_java[SYS_create_module]=true;syscall_java[SYS_delete_module]=true;syscall_java[SYS_fork]=true;
		/*syscall_java[SYS_getuid]=true;syscall_java[SYS_getpid]=true;*/syscall_java[SYS_getpgrp]=true;syscall_java[SYS_kill]=true;
		syscall_java[SYS_mkdir]=true;syscall_java[SYS_mknod]=true;syscall_java[SYS_mount]=true;
		syscall_java[SYS_rmdir]=true;syscall_java[SYS_ptrace]=true;syscall_java[SYS_reboot]=true;syscall_java[SYS_rename]=true;
		syscall_java[SYS_restart_syscall]=true;syscall_java[SYS_select]=true;syscall_java[SYS_setgid]=true;syscall_java[SYS_setitimer]=true;
		syscall_java[SYS_setgroups]=true;syscall_java[SYS_sethostname]=true;syscall_java[SYS_setrlimit]=true;syscall_java[SYS_setuid]=true;
		syscall_java[SYS_settimeofday]=true;syscall_java[SYS_tkill]=true;syscall_java[SYS_setrlimit]=true;syscall_java[SYS_setuid]=true;
		syscall_java[SYS_vfork]=true;syscall_java[SYS_vhangup]=true;syscall_java[SYS_vserver]=true;syscall_java[SYS_wait4]=true;
		syscall_java[SYS_clock_nanosleep]=true;syscall_java[SYS_nanosleep]=true;syscall_java[SYS_pause]=true;/*syscall_java[SYS_arch_prctl]=true;*/
		/*syscall_java[SYS_write]=true;syscall_java[SYS_writev]=true;*/
//		syscall_java[SYS_accept]=true;syscall_java[SYS_bind]=true;syscall_java[SYS_connect]=true;syscall_java[SYS_listen]=true;
//		syscall_java[SYS_socket]=true;
	}
	void get_all_case_file();
	void run(int case_num);
	bool compare(int case_num);
	bool PEcompare(int case_num);
	void saveResultToFile(int res);
	int readResultFromFile();
	void saveInfoToFile(int time, int memory);
	pair<int,int> readInfoFromFile();

	vector <string> case_file;
	Status *status;
	string case_dir;
	string out_file;
	string result_file;
	string info_file;
	int run_time_used;
	int run_exter_time;
	bool syscall_java[500];
	bool syscall_other[500];
};

#endif /* RUN_H_ */
