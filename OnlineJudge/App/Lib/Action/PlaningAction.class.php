<?php
class PlaningAction extends BaseAction {
	public function index(){
		$this->display();
	}
	public function add_planing(){
		if ($_SESSION['userID']>0){
			$param = $this->base_receive_JSON();
			$userID = $_SESSION['userID'];
			$data['user_id']=$userID;
			$data['plan_title']=$param['title'];
			$data['plan_date']=$param['date'];
			$data['plan_text']=$param['text'];
			$this->base_save_planing($data);
			$this->ajaxReturn("success",'JSON');
		}
		else {
			$this->ajaxReturn("failed",'JSON');
		}
	}
	private function rtn_planing($res) {
		for ( $i = 0 ; $i < count($res) ; $i++ ){
			$User = D('User');
			$nickname = $User->where('user_id='.$res[$i]['user_id'])->getField('user_nickname');
			$res[$i]["nickname"] = $nickname;
		}
		return $res;
	}
	public function list_all_planing() {
		if ($_SESSION['userID']>0){
			$res = $this->base_show_all_planing();
			$this->ajaxReturn(json_encode($this->rtn_planing($res)),'JSON');
		}
		else {
			$this->ajaxReturn("failed",'JSON');
		}
	}
	public function list_my_planing() {
		if ($_SESSION['userID']>0){
			$userID = $_SESSION['userID'];
			$res = $this->base_show_planing($userID);
			$this->ajaxReturn(json_encode($this->rtn_planing($res)),'JSON');
		}
		else {
			$this->ajaxReturn("failed",'JSON');
		}
	}
	public function show_planing() {
		$plan_id = $this->_get("id");
		$result = $this->base_find_planing_by_id($plan_id);
		$this->assign("result",$result);
		$this->display();
	}
	public function show_person_planing() {
		$userid = $this->_get("uid");
		$result = $this->base_get_planing_by_userid($userid);
		$this->assign("result",$this->rtn_planing($result));
		$this->display();
	}
	public function show_date_planing() {
		$time = $this->_get("date");
		$date = date("Y-m-d",strtotime($time));
		$result = $this->base_get_planing_by_date($date);
		$this->assign("result",$this->rtn_planing($result));
		$this->display();
	}
}