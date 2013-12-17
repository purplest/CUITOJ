<?php
class LoginLogModel extends Model {
	protected $fields = array(
		'login_log_id',
		'user_id',
		'login_log_ip',
		'login_log_time',
		'_pk' => 'login_log_id',
		'_autoinc' => true
	);
	
	protected $_auto = array(
		array('login_log_time','getTime',1,'callback'),
		array('login_log_ip','get_client_ip',1,'function'),
	);
	
	function getTime(){
		return date("Y-m-d H:i:s");
	}
}