/*
 * status.h
 *
 *  Created on: 2013-2-7
 *      Author: purple
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#ifndef STATUS_H_
#define STATUS_H_

using namespace std;

class Status {
public:
	int getProblemId() const {
		return problem_id;
	}
	void setProblemId(int problemId) {
		problem_id = problemId;
	}
	int getContestId() const {
		return contest_id;
	}
	void setContestId(int contestId) {
		contest_id = contestId;
	}
	string getStatusCode() const {
		return status_code;
	}
	void setStatusCode(string statusCode) {
		status_code = statusCode;
	}
	int getStatusId() const {
		return status_id;
	}
	void setStatusId(int statusId) {
		status_id = statusId;
	}
	string getFilename() const {
		return filename;
	}
	void setFilename(string filename) {
		this->filename = filename;
	}
	string getRunPath() const {
		return runPath;
	}
	string getCeFile() const {
		return CE_file;
	}
	int getStatusInfo() const {
		return status_info;
	}
	void setStatusInfo(int compileInfo) {
		status_info = compileInfo;
	}
	string getSourceFile() const {
		return source_file;
	}
	void setSourceFile(string sourceFile) {
		source_file = sourceFile;
	}
	string getTargetFile() const {
		return target_file;
	}
	void setTargetFile(string targetFile) {
		target_file = targetFile;
	}
	string getCasePath() const{
		return case_path;
	}
    int getMemoryLimited() const{
        return memory_limited;
    }
    void setMemoryLimited(int memoryLimited){
        memory_limited = memoryLimited;
    }
	int getTimeUsed() const {
		return time_used;
	}

	void setTimeUsed(int timeUsed) {
		time_used = timeUsed;
	}

	int getMemoryUsed() const {
		return memory_used;
	}

	void setMemoryUsed(int memoryUsed) {
		memory_used = memoryUsed;
	}

	string getCeReason() const {
		return CE_reason;
	}

	void setCeReason(string ceReason) {
		CE_reason = ceReason;
	}

	int getLanguage() const {
		return language;
	}

	void setLanguage(int language) {
		this->language = language;
	}

	string getTargetFileName() const {
		return target_file_name;
	}

	void setTargetFileName(string targetFileName) {
		target_file_name = targetFileName;
	}

	int getCaseTimeLimited() const {
		return case_time_limited;
	}

	void setCaseTimeLimited(int caseTimeLimited) {
		case_time_limited = caseTimeLimited;
	}

	int getTimeLimited() const {
		return time_limited;
	}

	void setTimeLimited(int totalTimeLimited) {
		time_limited = totalTimeLimited;
	}

	int getCaseTimeUsed() const
	{
		return case_time_used;
	}

	void setCaseTimeUsed(int caseTimeUsed)
	{
		case_time_used = caseTimeUsed;
	}

	Status() {
		this->runPath = "run";
		this->case_path = "testcase";
		this->CE_file = "./" + this->runPath + "/ce.txt";
		this->memory_used = 0;
	}

	static const int SINF_COMPILING = 0;
	static const int SINF_COMPILED_ERROR = 1;
	static const int SINF_TOO_MUCH_TIME = 2;
	static const int SINF_WITH_SOMETHING_WRONG = 3;
	static const int SINF_COMPILIED_COMPLETE = 4;
	static const int SINF_RUNING = 5;
	static const int SINF_Accepted = 6;
	static const int SINF_Time_Limit_Exceeded = 7;
	static const int SINF_Wrong_Answer = 8;
	static const int SINF_Output_Limit_Exceeded = 9;
	static const int SINF_Runtime_Error = 10;
	static const int SINF_Presentation_Error = 11;
	static const int SINF_Memory_Limit_Exceeded = 12;
	static const int SINF_Send_To_Judg = 13;
	static const int SINF_Judge_Error = 14;
	static const int SINF_RESTRICTED_FUNCTION = 15;

private:
	int status_id;
	int problem_id;
	int contest_id;
	string status_code;
	int case_time_limited;
	int time_limited;
	int memory_limited;
	int case_time_used;
	int time_used;
	int memory_used;
	int language;
	int status_info;
	string filename;
	string source_file;
	string target_file;
	string target_file_name;
	string runPath;
	string CE_file;
	string CE_reason;
	string case_path;
};

#endif /* STATUS_H_ */
