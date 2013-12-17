<?php
class ContestModel extends Model {
	protected $fields = array(
		'contest_id',
		'user_id',
		'contest_title',
		'contest_start_time',
		'contest_end_time',
		'contest_status',
		'_pk' => 'contest_id',
		'_autoinc' => true
	);
}