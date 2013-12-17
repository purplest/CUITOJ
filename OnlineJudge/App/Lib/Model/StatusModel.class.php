<?php
class StatusModel extends Model {
	protected $fields = array(
		'status_id',
		'user_id',
		'problem_id',
		'contest_id',
		'status_result',
		'status_language',
		'status_time',
		'status_memory',
		'status_length',
		'status_subtime',
		'status_code',
		'status_ce_reason',
		'_pk' => 'status_id',
		'_autoinc' => true
	);
	
	protected $_auto = array(
		array('status_subtime','getTime',1,'callback'),
		array('status_time',0,1),
		array('status_memory',0,1),
	);
	
	function getTime(){
		return date("Y-m-d H:i:s");
	}
}