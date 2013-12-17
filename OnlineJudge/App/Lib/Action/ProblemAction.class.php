<?php
class ProblemAction extends BaseAction {
    public function index(){
    	$where=array();
    	if (3>$this->_session('userPower'))
    		$where["problem_show"]=1;
    	$res = $this->base_count_problem_page($where);
    	$this->assign("totPage", $res);
		$this->display();
    }
    public function show_problem() {
//     	$this->display();
//     	return;
    	$pid = $this->_get("id");
    	$problem = D('Problem');
    	$result = $problem->find($pid);
    	if ($_SESSION['userPower']<3){
    		if ($result['problem_show']==0)
    			$result=null;
    	}
    	$this->assign("result",$result);
    	$this->display();
    }
    public function count_problem_page() {
    	$param = $this->base_receive_JSON();
		$where=array();
		if (3>$this->_session('userPower'))
			$where["problem_show"]=1;
		$res = $this->base_count_problem_page($where);
		$this->ajaxReturn($res,'JSON');
    }
    public function get_problem() {
    	$pid = $this->_post("pid");
    	$problem = D('Problem');
    	$result = $problem->find($pid);
    	$this->ajaxReturn(json_encode($result),'JSON');
    }
    public function list_problem() {
//     	$this->ajaxReturn("",'JSON');
//     	return;
    	$param = $this->base_receive_JSON();
    	$start = $param["start"];
    	$length = $param["length"];
    	$problem = D('Problem');
    	$where = array();
    	if ($_SESSION['userPower']<3){
    		$where['problem_show']=1;
    	}
    	$result = $problem->order('problem_id')->where($where)->page($start,$length)->select();
    	$this->ajaxReturn(json_encode($result),'JSON');
    }
    public function submit_problem() {
    	$param = $this->base_receive_JSON();
    	if(null==$this->_session('userID')){
    		$this->ajaxReturn("didn't login",'JSON');
    		return;
    	}
    	$code = $param["sourcesCode"];
    	$user = $this->_session('userID');
    	$pid = $param["problemID"];
    	$language = $param["langID"];
    	
		$data['user_id']=$user;
		$data['problem_id']=$pid;
		$data['contest_id']=0;
		$data['status_result']=$this->STATUS_MAP["Send To Judge"];
		$data['status_language']=$language;
		$data['status_length']=strlen($code);
		$data['status_code']=$code;
		
    	$RunID = $this->base_save_status($data);
    	$this->base_send_to_judge($pid,$RunID,0);
    	$this->ajaxReturn("success",'JSON');
    }
    public function rejudge() {
    	if(null==$this->_session('userID')){
    		$this->ajaxReturn("didn't login",'JSON');
    		return;
    	}
    	if(3!=$this->_session('userPower')){
    		$this->ajaxReturn("you can't rejudge",'JSON');
    		return;
    	}
    	$RunID = $this->_post("runid");
    	$run = $this->base_find_runid($RunID);
    	$pid = $run['problem_id'];
    	if ($run['status_result']==$this->STATUS_MAP['Accepted']){
    		$this->base_decrease_problem_accept($pid);
    	}
    	$this->base_send_to_judge($pid, $RunID);
    	$this->ajaxReturn("success",'JSON');
    }
}