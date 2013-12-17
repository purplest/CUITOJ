<?php
class ProblemModel extends Model {
	protected $fields = array(
		'problem_id',
		'problem_title',
		'problem_time',
		'problem_case_time',
		'problem_memory',
		'problem_desc',
		'problem_input',
		'problem_output',
		'problem_saminput',
		'problem_samoutput',
		'problem_hit',
		'problem_sources',
		'problem_author',
		'problem_AC',
		'problem_ALL',
		'problem_show',// 0-> only admin. 1-> for all
		'problem_vjudge',//0->local problem 1->pku
		'problem_vpid',
		'_pk' => 'problem_id',
		'_autoinc' => true
	);
}