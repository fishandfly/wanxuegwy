<?php 
define('VBO001_TEACHERCHANGE_SL','1');	//申论指导老师变更
define('VBO001_TEACHERCHANGE_XC','2');	//行测指导老师变更
define('VBO001_TEACHERCHANGE_ZG','3');	//主管咨询师变更

/**
 * VBO动作定义 hook_action_info
 * @return multitype:multitype:string boolean multitype:string
 */
function wanxueapi_action_info() {
	return array(
			'wanxueapi_vbo001_action' => array(
					'type' => 'node',
					'label' => 'VBO001',
					'behavior' => array('vbo001_action'),
					'configurable' => TRUE,
					'vbo_configurable' => FALSE,
					'triggers' => array('any'),
			),
	);
}

/**
 * 老师变更 VBO动作
 * @param unknown $settings
 * @return multitype:multitype:string boolean multitype:string
 */
function wanxueapi_vbo001_action_form($settings) {
	$form = array();
	//变更类型
	$changeTypeList[0]=t('vbo00101');//"无";
	$changeTypeList[VBO001_TEACHERCHANGE_SL]=t('vbo00102');//"申论指导老师变更";
	$changeTypeList[VBO001_TEACHERCHANGE_XC]=t('vbo00103');//"行测指导老师变更";
	
	//获取主管咨询师用户列表
	$result = views_get_view_result("ref003","entityreference_3");
	$userlist_001[0]='无';
	foreach($result as $record){
		$userlist_001[$record->uid]=$record->users_name;
	}
	
	//获取申论指导老师用户列表
	$result = views_get_view_result("ref003","entityreference_4");
	$userlist_002[0]='无';
	foreach($result as $record){
		$userlist_002[$record->uid]=$record->users_name;
	}
	
	//获取行测指导老师用户列表
	$result = views_get_view_result("ref003","entityreference_5");
	$userlist_003[0]='无';
	foreach($result as $record){
		$userlist_003[$record->uid]=$record->users_name;
	}

	//加载主管咨询师用户列表
	$form['vbo00105'] = array(
			'#type' => 'select',
			'#title' => t('vbo00105'),//'变更主管咨询师',
			'#options' => $userlist_001,
			'#required' => FALSE,
	);
	
// 	//加载申论用户列表
// 	$form['vbo00106'] = array(
// 			'#type' => 'select',
// 			'#title' => '变更申论指导老师',
// 			'#options' => $userlist_002,
// 			'#required' => FALSE,
// 	);
	
// 	//加载行测指导老师变更用户列表
// 	$form['vbo00107'] = array(
// 			'#type' => 'select',
// 			'#title' => '变更行测指导老师',
// 			'#options' => $userlist_003,
// 			'#required' => FALSE,
// 	);
	
	//变更类型
	$form['vbo00101'] = array(
			'#type' => 'select',
			'#title' => t('vbo00106'),//'变更指导老师',
			'#options' => $changeTypeList,
			'#required' => FALSE,
	);
	
	$form['vbo00103'] = array(
			'#type' => 'textarea',
			'#title' => t('vbo00107'),//'申请理由',
	);
	
	
	return $form;
}

/**
 * 验证老师变更单设置数据的正确性
 * @param unknown $form
 * @param unknown $form_state
 */
function wanxueapi_vbo001_action_validate($form, &$form_state) {
	if($form_state['values']['vbo00105'] == 0 && $form_state['values']['vbo00101'] == 0 )
		form_set_error('vbo00105',t('vbo00108'));//'至少要选择一个要变更的老师');
}

/**
 * 老师变更 自定义参数获取
 * @param unknown $form
 * @param unknown $form_state
 * @return multitype:NULL
 */
function wanxueapi_vbo001_action_submit($form, $form_state) {
	$return = array();
	
	if($form_state['values']['vbo00105'] != 0){
		//主管咨询师
		$return['vbo00101'] = VBO001_TEACHERCHANGE_ZG;
		$return['vbo00102'] = $form_state['values']['vbo00105'];
	}
	
	if($form_state['values']['vbo00101'] != 0){
		$return['vbo00101'] = $form_state['values']['vbo00101'];
		//$return['vbo00102'] = $form_state['values']['vbo00106'];
	}
	
// 	if($form_state['values']['vbo00107'] != 0){
// 		//行测老师
// 		$return['vbo00101'] = VBO001_TEACHERCHANGE_XC;
// 		$return['vbo00102'] = $form_state['values']['vbo00107'];
// 	}
	
	$return['vbo00103'] = $form_state['values']['vbo00103'];
	
	return $return;
}

/**
 * 老师变更动作处理
 * @param unknown $node
 * @param unknown $context
 */
function wanxueapi_vbo001_action(&$node, $context) {
	//$message = "Node=".$node->title.",01=".$context['vbo00101'].",02=".$context['vbo00102'].",理由=".$context['vbo00103'];
	//drupal_set_message($message);
	
	global $user;
	
	//Create a node object, and add node properties.
	$tr13 = (object)NULL;
	$tr13->type = "tr13";
	$tr13->nid = 0;
	$tr13->uid = $user->uid;
	$tr13->created = $tr13->changed = strtotime("now");
	$tr13->status = 1;
	$tr13->comment = 0;
	$tr13->promote = 0;
	$tr13->moderate = 0;
	$tr13->sticky = 0;
	$tr13->language = 'und';
	
	node_object_prepare($tr13);
	
	$tr13->field_f13001['und'][0]['target_id'] = $node->nid;
	$tr13->field_f13002['und'][0] = $context['vbo00101'];
	if($context['vbo00101']==VBO001_TEACHERCHANGE_ZG){
		//只有主管咨询师在申请时就明确需要变更的老师
		$tr13->field_f13005['und'][0]['target_id'] = $context['vbo00102'];
	}
	
	$tr13->body['und'][0]['value'] = $context['vbo00103'];
	$tr13->body['und'][0]['summary'] = 0;
	$tr13->body['und'][0]['safe_value'] = $context['vbo00103'];
	$tr13->body['und'][0]['safe_summary'] = 0;
	
	$tr13->og_group_ref['und'][0]['target_id']=$node->og_group_ref['und'][0]['target_id'];
	
	if($context['vbo00101'] == VBO001_TEACHERCHANGE_SL){
		//申论指导老师
		$tr13->field_f13003['und'][0]['target_id'] = $node->field_f01037['und'][0]['target_id'];
		$tr13->field_f13006['und'][0] = $node->field_f01053['und'][0];		//申论服务启动时间
	}
	
	if($context['vbo00101'] == VBO001_TEACHERCHANGE_XC){
		//行测指导老师
		$tr13->field_f13003['und'][0]['target_id'] = $node->field_f01036['und'][0]['target_id'];
		$tr13->field_f13006['und'][0] = $node->field_f01055['und'][0];		//行测服务启动时间
	}
	
	if($context['vbo00101'] == VBO001_TEACHERCHANGE_ZG){
		//主管咨询师
		$tr13->field_f13003['und'][0]['target_id'] = $node->field_f01035['und'][0]['target_id'];
		$tr13->field_f13006['und'][0] = $node->field_f01052['und'][0];		//主管咨询师服务启动时间
		
	}
	
	node_save($tr13);
	$nid=$tr13->nid;
	return $nid;
}

?>