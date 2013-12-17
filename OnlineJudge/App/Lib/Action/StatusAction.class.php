<?php
class StatusAction extends BaseAction {
	public function index() {
		$this->display();
	}
	public function list_status() {
// 		die();
    	$param = $this->base_receive_JSON();
    	$where=array();
    	$where['contest_id']=$param['cid'];
    	if (false==is_null($param['pid']))
    		$where['problem_id']=$param['pid'];
    	if (false==is_null($param['user'])){
    		$user = $this->base_find_user_by_username($param['user']);
    		$where['status.user_id']=$user['user_id'];
    	}
    	if (false==is_null($param['result']))
    		$where['status_result']=$param['result'];
    	if (false==is_null($param['lang']))
    		$where['status_language']=$param['lang'];
    	$start = $param['start'];
		$result['status'] = $this->base_select_status_by_where($where,$start);
		$result['userID'] = $this->_session('userID');
		$result['userPower'] = $this->_session('userPower');
		for ( $i = 0 ; $i < count($result['status']) ; $i++ ){
			$result['status'][$i]["color"]=$this->STATUSS[$result['status'][$i]["status_result"]][1];
			$result['status'][$i]["status_result"]=$this->STATUSS[$result['status'][$i]["status_result"]][0];
			$result['status'][$i]["status_language"]=$this->LANGUAGE[$result['status'][$i]["status_language"]];
		}
		$this->ajaxReturn(json_encode($result),'JSON');
	}
	public function show_run() {
// 		die();
		if(null==$this->_session('userID')){
			$this->ajaxReturn("didn't login",'JSON');
			return;
		}
		$runid = $this->_post("runid");
		//should validing user
		$status = $this->base_find_runid($runid);
		if ($status['user_id']==$this->_session('userID')||0<$this->_session('userPower')){
			$this->ajaxReturn(json_encode($status),'JSON');
		}
		else{
			$this->ajaxReturn(json_encode("You can't see it."),'JSON');
		}
	}
	public function show_ce_reason() {
		$runid = $this->_post("runid");
		$status = $this->base_find_runid($runid);
		$this->ajaxReturn($status["status_ce_reason"],'JSON');
	}
	
	public function count_status_page() {
		$param = $this->base_receive_JSON();
		$where=array();
		$where['contest_id']=0;
		if (false==is_null($param['pid']))
			$where['problem_id']=$param['pid'];
		if (false==is_null($param['user'])){
			$user = $this->base_find_user_by_username($param['user']);
			$where['user_id']=$user['user_id'];
		}
		if (false==is_null($param['result']))
			$where['status_result']=$param['result'];
		if (false==is_null($param['lang']))
			$where['status_language']=$param['lang'];
		$res = $this->base_count_status_page($where);
		$this->ajaxReturn($res,'JSON');
	}
}
?>