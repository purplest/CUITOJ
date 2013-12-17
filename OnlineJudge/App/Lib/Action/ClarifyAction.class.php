<?php
class ClarifyAction extends BaseAction {
	public function list_clarify() {
		$param = $this->base_receive_JSON();
		if (null==$this->_session('userID')){
			$this->ajaxReturn("",'JSON');
		}
		$cid = $param['cid'];
		$where = array();
		$where['contest_id']=$cid;
		$result['show_add']=1;
		if (3>$this->_session('userPower')){
			$child = array();
			$child['user_id']=$this->_session('userID');
			$child['clarify_public'] = 1;
			$child['_logic'] = 'or';
			$where['_complex'] = $child;
			$result['show_add']=0;
		}
		$result['list'] = $this->base_select_clarify($where);		
		$this->ajaxReturn(json_encode($result),'JSON');
	}
	
	public function add_clarify() {
		$param = $this->base_receive_JSON();
		if (null==$this->_session('userID')){
			$this->ajaxReturn("don't login",'JSON');
		}
		else{
			$cid = $param['cid'];
			$quertion = htmlspecialchars($param['context'],ENT_QUOTES,"UTF-8");
			$data['contest_id']=$cid;
			$data['user_id']=$this->_session('userID');
			$data['clarify_question']=$quertion;
			$this->base_save_clarify($data);
			$this->ajaxReturn("success",'JSON');
		}
	}
	
	public function modify_clarify() {
		$param = $this->base_receive_JSON();
		if (null==$this->_session('userID')){
			$this->ajaxReturn("don't login.",'JSON');
		}
		if (3>$this->_session('userPower')){
			$this->ajaxReturn("You can't modify.",'JSON');
		}
		else{
			$cid = $param['cid'];
			$clarify_id = $param['clarify_id'];
			$answer = htmlspecialchars($param['context'],ENT_QUOTES,"UTF-8");
			$data['clarify_id']=$clarify_id;
			$data['contest_id']=$cid;
			$data['user_id']=$this->_session('userID');
			$data['clarify_answer']=$answer;
			$data['clarify_public']=0;
			$this->base_update_clarify($data);
			$this->ajaxReturn("success",'JSON');
		}
	}
	
	public function public_clarify() {
		$param = $this->base_receive_JSON();
		if (null==$this->_session('userID')){
			$this->ajaxReturn("don't login.",'JSON');
		}
		else if (3>$this->_session('userPower')){
			$this->ajaxReturn("You can't modify.",'JSON');
		}
		else{
			$clarify_id = $param['clarify_id'];
			$this->base_public_clarify($clarify_id);
			$this->ajaxReturn("success",'JSON');
		}
	}
}