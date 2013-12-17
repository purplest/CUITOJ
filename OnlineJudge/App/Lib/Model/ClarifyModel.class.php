<?php
class ClarifyModel extends Model {
	protected $fields = array(
		'clarify_id',
		'contest_id',
		'user_id',
		'clarify_question',
		'clarify_answer',
		'clarify_public',
		'clarify_submit_time',
		'_pk' => 'clarify_id',
		'_autoinc' => true
	);
	
	protected $_auto = array(
		array('clarify_submit_time','getTime',1,'callback'),
		array('clarify_public',0,1),
	);
	
	function getTime(){
		return date("Y-m-d H:i:s");
	}
}