<?php
class PlanModel extends Model {
	protected $fields = array(
		'plan_id',
		'user_id',
		'plan_title',
		'plan_date',
		'plan_text',
		'plan_show',
		'_pk' => 'plan_id',
		'_autoinc' => true
	);
}