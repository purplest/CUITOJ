<?php
class UserAction extends BaseAction {
	public function login(){
		$param = $this->base_receive_JSON();
// 		$username=htmlspecialchars($param['username'],ENT_QUOTES,"UTF-8");
// 		$password=htmlspecialchars($param['password'],ENT_QUOTES,"UTF-8");
		$username=$param['username'];
		$password=$param['password'];
		$user = $this->base_find_user_by_username($username);
		if (0===strcmp($user['user_password'],md5($password)))
		{
			$_SESSION['userID']=$user['user_id'];
			$_SESSION['userPower']=$user['user_power'];
			$_SESSION['username']=$user['user_username'];
			$_SESSION['nickname']=$user['user_nickname'];
			$this->base_Login_log($user['user_id']);
			$this->ajaxReturn("success",'JSON');
		}
		else{
			$this->ajaxReturn("failed",'JSON');
		}
	}
	public function logout() {
		$_SESSION['userID']=null;
		$_SESSION['userPower']=null;
		$_SESSION['username']=null;
		$_SESSION['nickname']=null;
		$this->ajaxReturn("success",'JSON');
	}
	public function register() {
		$param = $this->base_receive_JSON();
		//valied username
// 		$username = htmlspecialchars($param['username'],ENT_QUOTES,"UTF-8");
		$username = $param['username'];
		if (strlen($username)>20||strlen($username)<3){
			$this->ajaxReturn("failed",'JSON');
		}
		$pattern="/^[a-zA-Z]\w{2,19}/";
		if (false==preg_match($pattern, $username))
			$this->ajaxReturn($i." username is incurrent",'JSON');
// 		for ( $i = 0 ; $i < strlen($username) ; $i++ ){
// 			if (!( (ord("a")<=ord($username[$i])&&(ord("z")>=ord($username[$i])))||
// 			(ord("A")<=ord($username[$i])&&(ord("Z")>=ord($username[$i])))||
// 			(ord("0")<=ord($username[$i])&&(ord("9")>=ord($username[$i]))) )){
// 				$this->ajaxReturn($i." username is incurrent",'JSON');
// 			}
// 		}
// 		$data['user_username']=htmlspecialchars($param['username'],ENT_QUOTES,"UTF-8");
// 		$data['user_password']=htmlspecialchars($param['password'],ENT_QUOTES,"UTF-8");
// 		$data['user_nickname']=htmlspecialchars($param['nickname'],ENT_QUOTES,"UTF-8");
// 		$data['user_email']=htmlspecialchars($param['email'],ENT_QUOTES,"UTF-8");
		$data['user_username']=$param['username'];
		$data['user_password']=$param['password'];
		$data['user_nickname']=$param['nickname'];
		$data['user_email']=$param['email'];
		$userID = $this->base_save_user($data);
		$_SESSION['userID']=$userID;
		$_SESSION['userPower']=$user['user_power'];
		$_SESSION['username']=$data['user_username'];
		$_SESSION['nickname']=$data['user_nickname'];
		$this->base_Login_log($userID);
		$this->ajaxReturn("success",'JSON');
	}
	
	public function show_user() {
		$username = $this->_get("name");
		$res = $this->base_find_user_by_username($username);
		$this->assign("detal",$res);
		$this->display();
	}
	
	public function modify_user() {
		$username = $this->_get("name");
		if ($username==$this->_session('username')||3==$this->_session('userPower')){
			$res = $this->base_find_user_by_username($username);
			$res['user_password']=null;
			$this->assign("detal",$res);
			$this->display();
		}
	}
	
	public function update_user_profile() {
		$param = $this->base_receive_JSON();
		if ($username==$this->_session('username')||3==$this->_session('userPower')){
// 			$data['user_id']=htmlspecialchars($param['userid'],ENT_QUOTES,"UTF-8");
// 			$data['user_nickname']=htmlspecialchars($param['nickname'],ENT_QUOTES,"UTF-8");
// 			$data['user_email']=htmlspecialchars($param['email'],ENT_QUOTES,"UTF-8");
			$data['user_id']=$param['userid'];
			$data['user_nickname']=$param['nickname'];
			$data['user_email']=$param['email'];
			$this->base_update_user($data);
			$this->ajaxReturn("success",'JSON');
		}
		else{
			$this->ajaxReturn("failed",'JSON');
		}
	}
	
	public function update_user_password() {
		$param = $this->base_receive_JSON();
		if ($username==$this->_session('username')||3==$this->_session('userPower')){
// 			$data['user_id']=htmlspecialchars($param['userid'],ENT_QUOTES,"UTF-8");
// 			$data['user_password']=htmlspecialchars($param['newpd'],ENT_QUOTES,"UTF-8");
// 			$oldpd=htmlspecialchars($param['oldpd'],ENT_QUOTES,"UTF-8");
// 			$repd=htmlspecialchars($param['repd'],ENT_QUOTES,"UTF-8");
			$data['user_id']=$param['userid'];
			$data['user_password']=$param['newpd'];
			$oldpd=$param['oldpd'];
			$repd=$param['repd'];
			$user = $this->base_find_user_by_id($userID);
			if ($user['user_password']==$oldpd&&$data['user_password']==$repd){
				$this->base_update_user($data);
				$this->ajaxReturn("success",'JSON');
			}
			else{
				$this->ajaxReturn("old password error",'JSON');
			}
		}
		else{
			$this->ajaxReturn("failed",'JSON');
		}
	}
}