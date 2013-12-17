<?php
class KeditorAction extends BaseAction{
	public function upload(){
		import('ORG.Net.UploadFile');
		$upload = new UploadFile();
		$upload->__set('saveRule', $this->base_find_new_pid($pid).'_'.date("Y-m-d_H:i:s", time()));
		$upload->__set('maxSize', 3145728);
		$upload->__set('savePath', './Public/Image/picture/');
		$upload->__set('allowExts', array('jpg','gif','png','jpeg','bmp'));
		$info = $upload->uploadOne($_FILES['imgFile']);
		if ($info){
			$ret['error']=0;
			$ret['url']="Public/Image/picture/".$info[0]['savename'];
			$this->ajaxReturn($ret,'JSON');
		}
		else{
			$ret['error']=1;
			$this->ajaxReturn($ret,'JSON');
		}
	}
}