<?php
/**
 * Created by PhpStorm.
 * User: UHPD Games
 * Date: 09/19/17
 * Time: 4:46 PM
 */
#This is common
///search
/// BY TOP co trinh do nhieu nhất.
///
///
/// DEBUG
/// /////
error_reporting(E_ALL);
ini_set('display_errors', 1);

//end debug
define('TIMKIEM_UNGVIEN', 'timungvien_sessions');

if (!module_load_include('inc', 'cv_tuyendung', 'inc/filter-form.lib')) die('Not found libs/UHPDGAMES_FILTER');

/**
 * @param $form
 * @param $form_state
 *
 * @return array
 */
function timkiem_pages($form, &$form_state) {
  global $timkiem_ungvien;
  $form = array();

  $timkiem_ungvien = new UHPDGAMES_FILTER();
  array_push($timkiem_ungvien->field_options, 'multiple');
  array_push($timkiem_ungvien->field_options, 'select2');

  $timkiem_ungvien->display_more_pages = False;
  $timkiem_ungvien->name_filter = $timkiem_ungvien->name_session = TIMKIEM_UNGVIEN;

  $field  = array('vitrimongmuon', 'diachi', 'or_kynang', 'and_kynang', 'not_kynang');
  $header = array('Vị trí công việc', 'Địa điểm', 'Có một trong những từ khoá sau', 'Bắt buộc có các từ khoá sau', 'Không có những từ khoá sau');
  $timkiem_ungvien->SET_filter($field, $header, 'filter');
  $timkiem_ungvien->SET_filter_options(array(
    'textfield',
    'user/autocomplete',
    array(
      'data' => array(1=>'photo', 2=>'abc'),
      'multiple' => 1,
      'select2' => array(),
    ),
    array(
      'data' => array(11=>'xxa',22=>'a',33=>'b'),
      'multiple' => 1,
      'select2' => array(),
      ),
    array(
      'data' => array(333=>'fdsfds',444=>'dsds',555=>'b'),
      'multiple' => 1,
      'select2' => array(
        'width' => '150',
        'placeholder' => 'Placeholder text',
        'allowClear'   => TRUE,
      ),
    ),
    ));

  /*$field = array('thoigian', 'nguoidung', 'TenHanhVi', 'khuvuc', 'noidung');
  $timkiem_ungvien->SET_filter($field, $header, 'list');*/
  $timkiem_ungvien->create_form_filter($form, $form_state);

  $data = render_data_tim_ung_vien($form_state);

  dpm($data, 'd');
  //SET SESSION
  //$rows = $timkiem_ungvien->uhpdgames_set_session(TIMKIEM_UNGVIEN, $data);

  $form['11212'] = array(
    '#type' => 'textfield',
    '#title' => t(' Vị trí công việc'),
    //'#required' => TRUE,
    '#default_value' => null,
    '#id' => 'keyword',
    '#autocomplete_path' => 'user/autocomplete',
    '#select2' => array()
  );
}

  //$form['list'] = array('#markup' => $timkiem_ungvien->GET_render_list($form_state, $rows));
/*  $form['thongtin'] = array(
    '#type' => 'fieldset',
    '#title' => t('TIm` kiem'),
    '#collapsible' => TRUE,
    '#prefix' => '<div id="page-search" class="container">',
    '#suffix' => '</div>',
  );

  $form['thongtin']['vitrimongmuon'] = array(
    '#type' => 'textfield',
    '#title' => t(' Vị trí công việc'),
    //'#required' => TRUE,
    '#default_value' => null,
    '#id' => 'keyword'
  );

  $form['thongtin']['diachi'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Địa điểm'),
    '#required' => FALSE,
    '#options' => array(1,2,3),
    //'#default_value' => 1,
    '#id' => 'location-selector',
    '#select2' => array(),

  );

  $form['thongtin']['or_kynang'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t(' Có một trong những từ khoá sau '),
    '#required' => FALSE,
    '#options' => array(4,5,6,'or'),
    //'#default_value' => 1,
    '#id' => 'or-keyword-selector',
    '#select2' => array(
      'width' => '150',
      'placeholder' => 'Placeholder text',
      'allowClear'   => TRUE,

),
  );
  $form['thongtin']['and_kynang'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Bắt buộc có các từ khoá sau'),
    '#required' => FALSE,
    '#options' => array(11,22,33,'and'),
    //'#default_value' => 1,
    '#id' => 'and-keyword-selector',
    '#select2' => array(
      'width' => '150',
      'placeholder' => 'Placeholder text',
      'allowClear'   => TRUE,

),
  );

  $form['thongtin']['not_kynang'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Không có những từ khoá sau'),
    '#required' => FALSE,
    '#options' => array(4,5,6,'not'),
    //'#default_value' => 1,
    '#id' => 'not-keyword-selector',
    '#select2' => array(),
  );*/
  /*$form['thongtin']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Cập nhật'),
  );*/
 // $form['thongtin']['#attached']['js'] = array(drupal_get_path('module', 'cv_tuyendung') . 'timkiem/filter.js');

  $form['thongtin']['ketqua_timkiem'] = array(
    '#markup' => render_list_users_as_pages($data),
    '#id' => 'add-id-ketqua' //css or add bar

  );
  $form['thongtin']['#attached'] = array(
    /*'js' => array(
      array('data' => drupal_get_path('module', 'cv_tuyendung') . '/js/filter.js'),
  //    array('data' => drupal_get_path('module', 'cv_tuyendung') . '/js/vue.js'),
    ),*/
    'css' => array(drupal_get_path('module', 'cv_tuyendung') . 'css/filter.css')
  );

  return $form;
}
function timkiem_pages_validate($form, &$form_state) {
  $values = $form_state['values'];

  timkiem_set_session(TIMKIEM_UNGVIEN, $values);
/*  global $timkiem_ungvien;
  $timkiem_ungvien->uhpdgames_set_session(TIMKIEM_UNGVIEN, $values);*/

}
function timkiem_pages_submit($form, &$form_state) {
  global $timkiem_ungvien;
  $values = $form_state['values'];
  dpm($_SESSION, 'ss');
  dpm($timkiem_ungvien, 'fssd');

  //$timkiem_ungvien->GET_submit_filters($values);

  $form_state['rebuild'] = TRUE;
}

function render_data_tim_ung_vien($all_results = false) {
  $q_kynang = db_select('cv_kynang', 'kn');
  $q_kynang->extend('PagerDefault');//->extend('TableSort');
  tim_ung_vien_boi_kynang($q_kynang);

  $q_kynang->addExpression('COUNT(kn.cvid)', 'count_kynang_co');
  //$q_kynang->join('cv', 'cv', 'cv.cvid = kn.cvid'); //JOIN cv w kynang
  $q_kynang->groupBy('kn.cvid');
  $q_kynang->orderBy('count_kynang_co');

  $limit = 25;
  if($GLOBALS['timkiem_ungvien']->uhpdgames_is_value_session('pages')) {
    $limit = $GLOBALS['timkiem_ungvien']->uhpdgames_is_value_session('pages');
  }

  $field = array('cvid', 'kynang_id');
  if ($all_results) $field = $header = array();
  $q_kynang->fields('kn', $field);
  //if (!1) $limit = $q_kynang->countQuery()->execute()->fetchField();
 // $q_kynang->limit($limit);

  $result = $q_kynang->execute();

  return $result;
}
function tim_ung_vien_boi_kynang(SelectQueryInterface &$q) {
  $or = db_or();
  $and = db_and();
  $not = db_and();
  $or_kynang = $and_kynang = $not_kynang = array();
  if(isset($_SESSION[TIMKIEM_UNGVIEN])) {
    $session = $_SESSION[TIMKIEM_UNGVIEN];

    $or_kynang = $session['or_kynang'];
    $and_kynang = $session['and_kynang'];
    $not_kynang = $session['not_kynang'];
  }
//  $value = isset($target['values']) ? $target['values'] : '';
/*  if($value) {
    $or_kynang = isset($value['or_kynang']) ? $value['or_kynang'] : '';
    $and_kynang = isset($value['and_kynang']) ? $value['and_kynang'] : '';
    $not_kynang = isset($value['not_kynang']) ? $value['not_kynang'] : '';
  }*///else _session

  if(count($or_kynang) > 0) {
    foreach ($or_kynang as $id) {
      $or->condition('kynang_id', $id);
    }
    $q->condition($or);
  }
  if(count($and_kynang) > 0) {
    foreach ($and_kynang as $id) {
      $and->condition('kynang_id', $id);
    }
    $q->condition($and);
  }
  if(count($not_kynang) > 0) {
    foreach ($not_kynang as $id) {
      $not->condition('kynang_id', $id, '<>');
    }
    $q->condition($not);
  }

  //other condition

}

function render_list_users_as_pages($data) {
  //html
  //css

  $html = 'Không có dữ liệu';

  $data = array();

  if (count($data) > 1) {
    //add pefect bar
    foreach ($data as $key => $value) {
      $t = <<< HTML
        <br>Ảnh đại diện: @anhdaidien
    <hr>
        <br>Mô tả: @mota
        <br>Kỹ năng : @kynang
    <hr>
HTML;

      $html .= t($t, array(

      ));
    }
    $html .= theme('pager');
  }
  return $html;
}

function timkiem_set_session($key, $value = null){
  if (isset($value)) {
    if(isset($_SESSION[$key])) unset($_SESSION[$key]);
    $_SESSION[$key] = $value;
  }
  if (isset($_SESSION[$key])) return $_SESSION[$key];
}