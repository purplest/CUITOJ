<?php
class BaseAction extends Action{
	protected $STATUSS = array (
		array("Compiling","black"),
		array("Compiled Error","green"),
		array("TOO MUCH TIME","black"),
		array("WITH SOMETHING WRONG","black"),
		array("COMPILIED COMPLETE","black"),
		array("Runing","black"),
		array("Accepted","blue"),
		array("Time Limit Exceeded","red"),
		array("Wrong Answer","red"),
		array("Output Limit Exceeded","red"),
		array("Runtime Error","red"),
		array("Presentation Error","red"),
		array("Memory Limit Exceeded","red"),
		array("Send To Judge","black"),
		array("Judge Error","red"),
		array("Restricted Function","red"),
	);
	protected $STATUS_MAP = array(
		"Compiling"=>0,
		"Compiled Error"=>1,
		"TOO MUCH TIME"=>2,
		"WITH SOMETHING WRONG"=>3,
		"COMPILIED COMPLETE"=>4,
		"Runing"=>5,
		"Accepted"=>6,
		"Time Limit Exceeded"=>7,
		"Wrong Answer"=>8,
		"Output Limit Exceeded"=>9,
		"Runtime Error"=>10,
		"Presentation Error"=>11,
		"Memory Limit Exceeded"=>12,
		"Send To Judge"=>13,
		"Judge Error"=>14,
		"Restricted Function"=>15
	);
	protected $LANGUAGE = array(
		"GNU G++",
		"GNU GCC",
		"Java"
	);
	protected $LANGUAGE_MAP = array(
		"GNU C++"=>0,
		"GNU C"=>1,
		"Java"=>2
	);
	
	protected function base_receive_JSON() {
		$json = file_get_contents('php://input');
		return json_decode($json,true);
	}
	
	protected function base_send_to_judge($problemID,$RunID,$contestID) {
		$host = "localhost";
		$port = "3790";
		if (!($sock = fsockopen($host,$port,$errno,$errstr))){
			echo "can't open socket";
			die();
		}
		//if (!fwrite($sock, "the problem is ".$problemID." Run ID is ".$RunID)){
		if (!fwrite($sock, $RunID." ".$problemID." ".$contestID."\n")){
			echo "can't write";
			die();
		}
		fclose($sock);
	}
	
	protected function base_save_status($data) {
		$status = D('Status');
		$status->create($data);
		$status->startTrans();
		$res = $status->add();
		if ($res!=false){
			$status->commit();
		}else{
			$status->rollback();
		}
		return $res;
	}
	
	protected function base_find_runid($runid) {
		$status = D('Status');
		$res = $status->find($runid);
		return $res;
	}
	
	protected function base_select_status_by_where($where,$start) {
		$status = D('Status');
		$res = $status->where($where)->order('status_id desc')->page($start,10)->join('user ON user.user_id = status.user_id')->select();
		return $res;
	}
	
	protected function base_select_status_by_cid($cid,$start) {
		$status = D('Status');
		$where=array();
		$where['contest_id']=$cid;
		$res = $status->where($where)->order('status_id desc')->page($start,10)->join('user ON user.user_id = status.user_id')->select();
		return $res;
	}
	
	protected function base_select_status_by_cid_asc($cid=0) {
		$status = D('Status');
		$where=array();
		$where['contest_id']=$cid;
		$res = $status->where($where)->order('status_id asc')->join('user ON user.user_id = status.user_id')->select();
		return $res;
	}
	
	protected function base_count_status_page($where) {
		$status = D('Status');
		$res = ceil($status->where($where)->count()/10.0);
		return $res;
	}
	
	protected function base_save_problem($data) {
		$problem = D('Problem');
		$problem->create($data);
		$problem->startTrans();
		$res = $problem->add();
		if ($res!=false){
			$problem->commit();
		}else{
			$problem->rollback();
		}
	}
	
	protected function base_update_problem($data) {
		$problem = D('Problem');
		$problem->create($data);
		$problem->startTrans();
		$res = $problem->save();
		if ($res!=false){
			$problem->commit();
		}else{
			$problem->rollback();
		}
	}
	
	protected function base_find_problem_by_pid($pid) {
		$problem = D('Problem');
		$res = $problem->find($pid);
		return $res;
	}
	
	protected function base_decrease_problem_accept($pid) {
		$problem = D('Problem');
		$problem->startTrans();
		$where=array();
		$where['problem_id']=$pid;
		$res = $problem->where($where)->setDec('problem_AC');
		if ($res!=false){
			$problem->commit();
		}else{
			$problem->rollback();
		}
	}
	
	protected function base_find_new_pid($pid) {
		$problem = D('Problem');
		$res = $problem->max('problem_id')+1;
		return $res;
	}
	
	protected function base_count_problem_page($where) {
		$problem = D('Problem');
		$res = ceil($problem->where($where)->count()/10.0);
		return $res;
	}
	
	protected function base_count_contest_page($where) {
		$contest = D('Contest');
		$res = ceil($contest->where($where)->count()/10.0);
		return $res;
	}
	
	protected function base_save_contest($data) {
		$contest = D('Contest');
		$contest->create($data);
		$contest->startTrans();
		$res = $contest->add();
		if ($res!=false){
			$contest->commit();
		}else{
			$contest->rollback();
		}
	}
	
	protected function base_update_contest($data) {
		$contest = D('Contest');
		$contest->create($data);
		$contest->startTrans();
		$res = $contest->save();
		if ($res!=false){
			$contest->commit();
		}else{
			$contest->rollback();
		}
	}
	
	protected function base_find_contest_by_id($cid) {
		$contest = D('Contest');
		$res = $contest->find($cid);
		return $res;
	}
	
	protected function base_find_contest_problem_by_id($cpid) {
		$contest_problem = D('ContestProblem');
		$res = $contest_problem->find($cpid);
		return $res;
	}
	
	protected function base_find_contest_problem_by_cid_and_cp($cid, $cp) {
		$contest_problem = D('ContestProblem');
		$where=array();
		$where['contest_id']=$cid;
		$where['contest_problem_problem']=$cp;
		$res = $contest_problem->where($where)->find();
		return $res;
	}
	
	protected function base_select_contest_problem_by_cid($cid) {
		$contest_problem = D('ContestProblem');
		$where=array();
		$where['contest_id']=$cid;
		$res = $contest_problem->where($where)->order('contest_problem_problem')->select();
		return $res;
	}
	
	protected function base_select_contest_problem_info_by_cid($cid) {
		$contest_problem = D('ContestProblem');
		$where=array();
		$where['contest_id']=$cid;
		$res = $contest_problem->where($where)->order('contest_problem_problem')->join('INNER JOIN problem ON problem.problem_id = contest_problem.problem_id')->select();
		return $res;
	}
	
	protected function base_save_contest_problem($data) {
		$contest_problem = D('ContestProblem');
		$contest_problem->create($data);
		$contest_problem->startTrans();
		$res = $contest_problem->add();
		if ($res!=false){
			$contest_problem->commit();
		}else{
			$contest_problem->rollback();
		}
	}
	
	protected function base_update_contest_problem($data) {
		$contest_problem = D('ContestProblem');
		$contest_problem->create($data);
		$contest_problem->startTrans();
		$res = $contest_problem->save();
		if ($res!=false){
			$contest_problem->commit();
		}else{
			$contest_problem->rollback();
		}
	}
	
	protected function base_update_contest_problem_bzero($cid,$problemid) {
		$contest_problem = D('ContestProblem');
		$update['problem_id']=0;
		$contest_problem->startTrans();
		$where=array();
		$where['contest_id']=$cid;
		$where['contest_problem_problem']=array('egt',$problemid);
		$res = $contest_problem->where($where)->setField($update);
		if ($res!=false){
			$contest_problem->commit();
		}else{
			$contest_problem->rollback();
		}
	}
	
	protected function base_save_planing($data) {
		$plan = D('Plan');
		$plan->create($data);
		$plan->startTrans();
		$res = $plan->add();
		if ($res!=false){
			$plan->commit();
		}else{
			$plan->rollback();
		}
	}
	
	protected function base_update_planing($data) {
		$plan = D('Plan');
		$plan->create($data);
		$plan->startTrans();
		$res = $plan->save();
		if ($res!=false){
			$plan->commit();
		}else{
			$plan->rollback();
		}
	}
	
	protected function base_show_all_planing() {
		$plan = D('Plan');
		$res = $plan->order('plan_date desc')->select();
		return $res;
	}
	
	protected function base_show_planing($userID) {
		$plan = D('Plan');
		$where=array();
		$where['user_id']=$userid;
		$res = $plan->where($where)->order('plan_date')->select();
		return $res;
	}
	
	protected function base_find_planing_by_id($planid) {
		$Plan = D('Plan');
		$res = $Plan->find($planid);
		return $res;
	}
	
	protected function base_get_planing_by_userid($userid) {
		$Plan = D('Plan');
		$where=array();
		$where['user_id']=$userid;
		$res = $Plan->where($where)->select();
		return $res;
	}
	
	protected function base_get_planing_by_date($date) {
		$Plan = D('Plan');
		$start = $date;
		$end = date("Y-m-d", strtotime($date)+86400);
		$where=array();
		$where['plan_date']=array('between',array($start,$end));
		$res = $Plan->where('plan_date>="'.$start.'" AND plan_date < "'.$end.'"')->select();
		return $res;
	}
	
	protected function base_find_user_by_id($userID) {
		$User = D('User');
		$res = $User->find($userID);
		return $res;
	}
	
	protected function base_find_user_by_username($username) {
		$User = D('User');
		$where=array();
		$where['user_username']=$username;
		$res = $User->where($where)->find();
		return $res;
	}
	
	protected function base_save_user($data) {
		$User = D('User');
		$User->create($data);
		$User->startTrans();
		$res = $User->add();
		if ($res!=false){
			$User->commit();
		}else{
			$User->rollback();
		}
		return $res;
	}
	
	protected function base_update_user($data) {
		$User = D('User');
		$User->create($data);
		$User->startTrans();
		$res = $User->save();
		if ($res!=false){
			$User->commit();
		}else{
			$User->rollback();
		}
		return $res;
	}
	
	protected function base_select_clarify($where) {
		$Clarify = D('Clarify');
		$res = $Clarify->where($where)->select();
		return $res;
	}
	
	protected function base_save_clarify($data) {
		$Clarify = D('Clarify');
		$Clarify->create($data);
		$Clarify->startTrans();
		$res = $Clarify->add();
		if ($res!=false){
			$Clarify->commit();
		}else{
			$Clarify->rollback();
		}
		return $res;
	}
	
	protected function base_update_clarify($data) {
		$Clarify = D('Clarify');
		$Clarify->create($data);
		$Clarify->startTrans();
		$res = $Clarify->save();
		if ($res!=false){
			$Clarify->commit();
		}else{
			$Clarify->rollback();
		}
		return $res;
	}
	
	protected function base_public_clarify($clarify_id) {
		$Clarify = D('Clarify');
		$where=array();
		$where['clarify_id']=$clarify_id;
		$field['clarify_public']=1;
		$Clarify->where($where)->setField($field);
	}
	
	protected function base_Login_log($userID) {
		$Log = D('LoginLog');
		$data['user_id']=$userID;
		$Log->create($data);
		$Log->startTrans();
		$res = $Log->add();
		if ($res!=false){
			$Log->commit();
		}else{
			$Log->rollback();
		}
	}
}
?>