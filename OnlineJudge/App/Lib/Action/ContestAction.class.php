<?php
class ContestAction extends BaseAction {
    public function index(){
    	$where=array();
    	$res = $this->base_count_contest_page($where);
    	$this->assign("totPage", $res);
		$this->display();
    }
    public function show_contest() {
    	$cid = $this->_get("id");
    	$result = $this->base_find_contest_by_id($cid);
    	$this->assign("cid",$cid);
    	$this->assign("owner",$result['user_id']);
    	$this->display();
    }
    public function list_contest() {
    	$param = $this->base_receive_JSON();
    	$start = $param["start"];
    	$length = $param["length"];
    	$contest = D('Contest');
    	$field="contest_id,contest_title,contest_start_time,contest_end_time,user_username";
    	$result = $contest->order('contest_id')->join('user ON user.user_id = contest.user_id')->page($start,$length)->getField($field);
    	$this->ajaxReturn(json_encode($result),'JSON');
    }
    public function add_contest() {
    	$param = $this->base_receive_JSON();
    	if(null==$this->_session('userID')){
    		$this->ajaxReturn("didn't login",'JSON');
    		return;
    	}
    	if (2>$this->_session('userPower')){
    		$this->ajaxReturn("can't add",'JSON');
    		return;
    	}
    	if (strtotime($param['start'])>=strtotime($param['end'])){
    		$this->ajaxReturn("the time is wrong",'JSON');
    		return;
    	}
    	$data['contest_title']=$param['title'];
    	$data['contest_start_time']=$param['start'];
    	$data['contest_end_time']=$param['end'];
    	$data['user_id']=$this->_session('userID');
    	$this->base_save_contest($data);
    	$this->ajaxReturn("success",'JSON');
    }
    public function change_contest_time(){
    	$param = $this->base_receive_JSON();
    	if(null==$this->_session('userID')){
    		$this->ajaxReturn("didn't login",'JSON');
    		return;
    	}
    	$cid=$param['id'];
    	$contest = $this->base_find_contest_by_id($cid);
    	$owner = $contest['user_id'];
    	if (3>$this->_session('userPower')&&$owner!=$this->_session('userID')){
    		$this->ajaxReturn("can't change",'JSON');
    		return;
    	}
    	$data['contest_id']=$param['id'];
    	$data['contest_title']=$param['title'];
    	$data['contest_start_time']=$param['start'];
    	$data['contest_end_time']=$param['end'];
    	//$this->base_update_contest($data);
    	$this->ajaxReturn("success",'JSON');
    }
    public function show_contest_info() {
    	$cid = $this->_post("cid");
    	$result['info'] = $this->base_find_contest_by_id($cid);
    	$result['detal']= $this->base_select_contest_problem_info_by_cid($cid);
    	$this->ajaxReturn(json_encode($result),'JSON');
    }
    public function set_contest_problem() {
    	$param = $this->base_receive_JSON();
    	if(null==$this->_session('userID')){
    		$this->ajaxReturn("didn't login",'JSON');
    		return;
    	}
    	$cid = $param['cid'];
    	$problemlist = $param['problem'];
    	$problemid = 1;
    	foreach ($problemlist as $val){
    		if (is_numeric($val)){
	    		$data['contest_id']=$cid;
	    		$problem = $this->base_find_problem_by_pid($val);//valid the problem
	    		if (null==$problem){
	    			$data['problem_id']=0;
	    		}
	    		else{
	    			$data['problem_id']=$val;
	    		}
	    		$data['contest_problem_problem']=$problemid;
	    		$res = $this->base_find_contest_problem_by_cid_and_cp($cid,$problemid);
	    		if (null==$res){//create a new contest_problem
	    			$data['contest_problem_id']=null;
	    			$this->base_save_contest_problem($data);
	    		}
	    		else{//update the contest_problem
	    			$data['contest_problem_id']=$res['contest_problem_id'];
	    			$this->base_update_contest_problem($data);
	    		}
	    		$problemid++;
    		}
    	}
    	$this->base_update_contest_problem_bzero($cid,$problemid);//make the remained to be zero
    	$this->ajaxReturn("success",'JSON');
    }
    
    public function get_contest_problem() {
    	$param = $this->base_receive_JSON();
    	$cid = $param["cid"];
    	$contest = $this->base_find_contest_by_id($cid);
    	$now = time();
    	if (strtotime($contest['contest_start_time'])<=$now){
	    	$cp = $param["cp"];
	    	$res = $this->base_select_contest_problem_by_cid($cid);
	    	if ($cp>0){
		    	foreach ($res as $val){
		    		if ($val['contest_problem_problem']==$cp)
		    			$pid = $val['problem_id'];
		    	}
		    	$problem = $this->base_find_problem_by_pid($pid);
		    	$result['list']=$res;
		    	$result['detial']=$problem;
		    	$result['id']=$cp;
		    	$this->ajaxReturn(json_encode($result),'JSON');
	    	}
	    	else{
	    		$this->ajaxReturn(json_encode($res),'JSON');
	    	}
    	}
    	else{
    		$this->assign("",'JSON');
    	}
    }
    
    public function submit_problem() {
    	$param = $this->base_receive_JSON();
    	if(null==$this->_session('userID')){
    		$this->ajaxReturn("didn't login",'JSON');
    		return;
    	}
    	$cid = $param['contestID'];
    	$contest = $this->base_find_contest_by_id($cid);
    	$now_time = time();
    	$start_time = strtotime($contest['contest_start_time']);
    	$end_time = strtotime($contest['contest_end_time']);
    	if ($start_time<=$now_time&&$now_time<=$end_time){
	    	$code = $param["sourcesCode"];
	    	$user = $this->_session('userID');
	    	$cp = $param["problemID"];
	    	$ContestProblem = $this->base_find_contest_problem_by_cid_and_cp($cid, $cp);
	    	$pid = $ContestProblem['problem_id'];
	    	
	    	$language = $param["langID"];
	    	 
	    	$data['user_id']=$user;
	    	$data['problem_id']=$pid;
			$data['contest_id']=$cid;
	    	$data['status_result']=$this->STATUS_MAP["Send To Judge"];
	    	$data['status_language']=$language;
	    	$data['status_length']=strlen($code);
	    	$data['status_code']=$code;
	    	
	    	$RunID = $this->base_save_status($data);
	    	$this->base_send_to_judge($cp,$RunID,$cid);
	    	$this->ajaxReturn("success",'JSON');
    	}
    	else{
    		$this->ajaxReturn("your submit time out of contest",'JSON');
    	}
    }
    
    public function get_contest_status() {
    	if (null==$this->_session('userID')){
    		$this->ajaxReturn("",'JSON');
    		return;
    	}
    	$param = $this->base_receive_JSON();
    	$start = $param['start'];
    	$cid = $param['cid'];
    	$cpp = $this->base_select_contest_problem_by_cid($cid);
    	foreach ($cpp as $val){
    		$map[$val['contest_problem_problem']]=$val['problem_id'];
    	}
    	$map[0]=0;
    	$where['contest_id']=$cid;
    	if (false==is_null($param['pid']))
    		$where['problem_id']=$map[$param['pid']];
    	if (false==is_null($param['user'])){
    		$user = $this->base_find_user_by_username($param['user']);
    		$where['status.user_id']=$user['user_id'];
    	}
    	if (false==is_null($param['result']))
    		$where['status_result']=$param['result'];
    	if (false==is_null($param['lang']))
    		$where['status_language']=$param['lang'];
    	$contest = $this->base_find_contest_by_id($cid);
    	$now_time = time();
    	$end_time = strtotime($contest['contest_end_time']);
    	if ($this->_session('userPower')<3&&$now_time<$end_time){
    		$where['user.user_id']=$this->_session('userID');
    	}
    	$result['status'] = $this->base_select_status_by_where($where,$start);
    	$result['userID'] = $this->_session('userID');
    	$result['userPower'] = $this->_session('userPower');
    	for ( $i = 0 ; $i < count($result['status']) ; $i++ ){
			$result['status'][$i]["color"]=$this->STATUSS[$result['status'][$i]["status_result"]][1];
			$result['status'][$i]["status_result"]=$this->STATUSS[$result['status'][$i]["status_result"]][0];
			$result['status'][$i]["status_language"]=$this->LANGUAGE[$result['status'][$i]["status_language"]];
    		$result['status'][$i]['problem_id']=$map[$result['status'][$i]['problem_id']];
    	}
    	$this->ajaxReturn(json_encode($result),'JSON');
    }
    
    public function get_contest_standing() {
    	$cid = $this->_post("cid");
    	$cpp = $this->base_select_contest_problem_by_cid($cid);
    	foreach ($cpp as $val){
    		$map[$val['problem_id']]=$val['contest_problem_problem'];
    	}
    	$map[0]=0;
    	
    	$status = D('Status');
    	$where=array();
    	$where['contest_id']=$cid;
    	$res = $status->where($where)->order('status_id asc')->join('user ON user.user_id = status.user_id')->select();
    	
    	$result['status'] = $this->base_select_status_by_cid_asc($cid);
    	for ( $i = 0 ; $i < count($result['status']) ; $i++ ){
    		$result['status'][$i]['problem_id']=$map[$result['status'][$i]['problem_id']];
    	}
    	$result['count']=count($map);
    	$contest = $this->base_find_contest_by_id($cid);
    	$result['start_time']=$contest['contest_start_time'];
    	$this->ajaxReturn(json_encode($result),'JSON');
    }
    
	public function count_status_page() {
		$cid = $this->_post('cid');
		$where=array();
		$where['contest_id']=$cid;
		$res = $this->base_count_status_page($where);
		$this->ajaxReturn($res,'JSON');
	}
}
?>