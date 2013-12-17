<?php
class UserModel extends Model {
	protected $fields = array(
		'user_id',
		'user_username',
		'user_password',
		'user_nickname',
		'user_email',
		'user_regtime',
		'user_reg_ip',
		'user_power',// 0 -> normal user; 1 -> code viewer; 2 -> teamer; 3 -> admin;
		'_pk' => 'user_id',
		'_autoinc' => true
	);
	
	protected $_auto = array(
		array('user_password','md5',3,'function'),
		array('user_regtime','getTime',1,'callback'),
		array('user_reg_ip','get_client_ip',1,'function'),
		array('user_power',0,1),
	);
	
	function getTime(){
		return date("Y-m-d H:i:s");
	}
}