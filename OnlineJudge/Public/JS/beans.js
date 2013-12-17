function SubmitProblemBean(sourcesCode,problemID,langID){
	this.sourcesCode = sourcesCode;
	this.problemID = problemID;
	this.langID = langID;
}

function SubmitContestProblemBean(sourcesCode,problemID,contestID,langID){
	this.contestID = contestID;
	this.sourcesCode = sourcesCode;
	this.problemID = problemID;
	this.langID = langID;
}

function listBean(start,cid){
	this.start = start;
	this.cid = cid;
}

function ListStatusBean(start,cid,pid,user,result,lang){
	this.start=start;
	this.cid=cid;
	this.pid=pid;
	this.user=user;
	this.result=result;
	this.lang=lang;
}

function listProblemBean(start,length){
	this.start = start;
	this.length=length;
}

function listContestBean(start,length){
	this.start = start;
	this.length=length;
}

function NewContestBean(title,start,end){
	this.title=title;
	this.start=start;
	this.end=end;
}

function ChangeContestTitleAndTimeBean(id,title,start,end){
	this.id=id;
	this.title=title;
	this.start=start;
	this.end=end;
}

function EditContestProblemBean(cid,problem){
	this.cid=cid;
	this.problem=problem;
}

function GetContestProblemBean(cid,cp){
	this.cid=cid;
	this.cp=cp;
}

function ClarifyBean(context,cid,clarify_id){
	this.context=context;
	this.cid=cid;
	this.clarify_id=clarify_id;
}

function PlaningBean(title,date,text){
	this.title=title;
	this.date=date;
	this.text=text;
}

function modifyUserProfile(userid,nickname,email) {
	this.userid=userid;
	this.nickname=nickname;
	this.email=email;
}

function modifyUserPassword(userid,oldpd,newpd,repd) {
	this.userid=userid;
	this.oldpd=oldpd;
	this.newpd=newpd;
	this.repd=repd;
}

function addProblemBean(title,desc,input,output,sa_in,sa_out,time,memory,hit,sources,author){
	this.title=title;
	this.desc=desc;
	this.input=input;
	this.output=output;
	this.sa_in=sa_in;
	this.sa_out=sa_out;
	this.time=time;
	this.memory=memory;
	this.hit=hit;
	this.sources=sources;
	this.author=author;
}

function modifyProblemBean(pid,title,desc,input,output,sa_in,sa_out,time,memory,hit,sources,author){
	this.pid=pid;
	this.title=title;
	this.desc=desc;
	this.input=input;
	this.output=output;
	this.sa_in=sa_in;
	this.sa_out=sa_out;
	this.time=time;
	this.memory=memory;
	this.hit=hit;
	this.sources=sources;
	this.author=author;
}

function adminListUserBean(list){
	this.list=list;
}