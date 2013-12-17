<?php
class ContestProblemModel extends Model {
	protected $fields = array(
		'contest_problem_id',
		'contest_id',
		'problem_id',
		'contest_problem_problem',
		'contest_problem_tot_submit',
		'contest_problem_tot_ac',
		'_pk' => 'contest_problem_id',
		'_autoinc' => true
	);
}