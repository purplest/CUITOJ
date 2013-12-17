<?php
class AdminAction extends BaseAction {
	public function index() {
		$this->display();
	}
	public function add_problem() {
		$param = $this->base_receive_JSON();
		if (2>$this->_session('userPower')){
    		$this->ajaxReturn("can't add",'JSON');
    		return;
    	}
		$title = $param['title'];
		$desc = $param['desc'];
		$input = $param['input'];
		$output = $param['output'];
		$sa_in = $param['sa_in'];
		$sa_out = $param['sa_out'];
		$time = $param['time'];
		$memory = $param['memory'];
		$hit = $param['hit'];
		$sources = $param['sources'];
		$author = $param['author'];
		$data['problem_title']=$title;
		$data['problem_time']=$time;
		$data['problem_case_time']=$time;
		$data['problem_memory']=$memory;
		$data['problem_desc']=$desc;
		$data['problem_input']=$input;
		$data['problem_output']=$output;
		$data['problem_saminput']=$sa_in;
		$data['problem_samoutput']=$sa_out;
		$data['problem_hit']=$hit;
		$data['problem_sources']=$sources;
		$data['problem_author']=$author;
		$this->base_save_problem($data);
		$this->ajaxReturn("success",'JSON');
	}
	public function modify_problem() {
		$param = $this->base_receive_JSON();
		if (2>$this->_session('userPower')){
			$this->ajaxReturn("can't modify",'JSON');
			return;
		}
		$pid = $param['pid'];
		$title = $param['title'];
		$desc = $param['desc'];
		$input = $param['input'];
		$output = $param['output'];
		$sa_in = $param['sa_in'];
		$sa_out = $param['sa_out'];
		$time = $param['time'];
		$memory = $param['memory'];
		$hit = $param['hit'];
		$sources = $param['sources'];
		$author = $param['author'];
		$data['problem_id']=$pid;
		$data['problem_title']=$title;
		$data['problem_time']=$time;
		$data['problem_case_time']=$time;
		$data['problem_memory']=$memory;
		$data['problem_desc']=$desc;
		$data['problem_input']=$input;
		$data['problem_output']=$output;
		$data['problem_saminput']=$sa_in;
		$data['problem_samoutput']=$sa_out;
		$data['problem_hit']=$hit;
		$data['problem_sources']=$sources;
		$data['problem_author']=$author;
		$this->base_update_problem($data);
		$this->ajaxReturn("success",'JSON');
	}
	public function add_list_user() {
		$param = $this->base_receive_JSON();
		if (2>$this->_session('userPower')){
			$this->ajaxReturn("can't add list user",'JSON');
			return;
		}
		foreach ( $param['list'] as $user){
			$data['user_username']=$user[0];
			$data['user_password']=$user[1];
			$data['user_nickname']=$user[0];
			$userID = $this->base_save_user($data);
		}
		$this->ajaxReturn("success",'JSON');
	}
	public function modify_list_user() {
		$param = $this->base_receive_JSON();
		if (2>$this->_session('userPower')){
			$this->ajaxReturn("can't modify list user",'JSON');
			return;
		}
		foreach ( $param['list'] as $user){
			$res = $this->base_find_user_by_username($user[0]);
			$data['user_id']=$res['user_id'];
			$data['user_username']=$user[0];
			$data['user_password']=$user[1];
			$data['user_nickname']=$user[0];
			$userID = $this->base_update_user($data);
		}
		$this->ajaxReturn("success",'JSON');
	}
}