<?php
/**
 * Created by PhpStorm.
 * User: UHPD Games
 * Date: 09/19/17
 * Time: 4:46 PM
 */
///search
/// TOP co trinh do nhieu nhất.
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
function timkiem_ungvien_form($form, &$form_state) {
  global $timkiem_ungvien;
  $form = array();

  $timkiem_ungvien = new UHPDGAMES_FILTER();
  array_push($timkiem_ungvien->field_options, 'multiple');
  array_push($timkiem_ungvien->field_options, 'select2');

  $timkiem_ungvien->display_more_pages = False;//True để hiển thị có phân trang
  $timkiem_ungvien->name_filter = $timkiem_ungvien->name_session = TIMKIEM_UNGVIEN;

  //thêm tiêu chí tìm kiếm từ cv (hoặc khác) tại $field và $header
  $field  = array(
    'vitrimongmuon',
    'diachi',
    'or_kynang',
    'and_kynang',
    'not_kynang',
    'uutien'
  );
  $header = array(
    'Vị trí công việc',
    'Địa điểm',
    'Có một trong những từ khoá sau',
    'Bắt buộc có các từ khoá sau',
    'Không có những từ khoá sau',
    'Ưu tiên hiển thị ứng viên'
  );
  $timkiem_ungvien->SET_filter($field, $header, 'filter');

  //$kynang = load_kynangAllKey_array();
  $timkiem_ungvien->SET_filter_options(array(
    'textfield',
    'textfield',
    array(
      'data' => array(1=>'photo', 2=>'abc'),//$kynang
      'multiple' => 1,
      'select2' => array(),
    ),
    array(
      'data' => array(11=>'xyz',22=>'may anh',33=>'chup hinh'),
      'multiple' => 1,
      'select2' => array(),
      ),
    array(
      'data' => array(333=>'fdsfds',444=>'dsds',555=>'bca'),
      'multiple' => 1,
      'select2' => array(
        'width' => '250',
        'placeholder' => 'Gõ để tìm kiếm',
        'allowClear'   => TRUE,
      ),
    ),
    array('data' => array(1=> 'Cập nhật mới nhất', 2=>'đang tìm việc', 3=>'Có kinh nghiệm làm việc'))
  ));
  $timkiem_ungvien->create_form_filter($form, $form_state);

  //test
  $data = render_data_tim_ung_vien();
  //end test

  $form['ketqua_timkiem'] = array(
    '#type' => 'fieldset',
    '#title' => t(''),
    '#collapsible' => TRUE,
    '#prefix' => '<div id="ketqua-timkiemungvien" class="container-search">',
    '#suffix' => '</div>',
  );
  $form['ketqua_timkiem']['chitiet'] = array(
    '#markup' => render_list_users_as_pages(),//$data
  );
  $form['#attached'] = array(
    'css' => array(drupal_get_path('module', 'cv_tuyendung') . 'css/filter.css'),
    'js' => array(drupal_get_path('module', 'cv_tuyendung') . 'js/filter.js'),
  );

  return $form;
}
function timkiem_ungvien_form_validate($form, &$form_state) {

}
function timkiem_ungvien_form_submit($form, &$form_state) {
  //dpm($_SESSION, 'ss');

  $values = $form_state['values'];
  timkiem_set_session(TIMKIEM_UNGVIEN, $values);

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
  $uutien = $vitricv = $ddlamviec = '';

  if(isset($_SESSION[TIMKIEM_UNGVIEN])) {
    $session = $_SESSION[TIMKIEM_UNGVIEN];

    $or_kynang = $session['or_kynang'];
    $and_kynang = $session['and_kynang'];
    $not_kynang = $session['not_kynang'];
    $uutien = $session['uutien'];
    $vitricv = $session['vitrimongmuon'];
    $ddlamviec = $session['diachi'];
  }

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
/*  if(!empty($uutien)) {
    $q->condition('', $uutien);
  }
  if(!empty($vitricv)) {
    $q->condition('', $vitricv);
  }
  if(!empty($ddlamviec)) {
    $q->condition('', $ddlamviec);
  }*/
//end
}

function render_list_users_as_pages() {//$data
  //html + css here!

  $html = t('');

  //test
  $data = array(
    0 => array(
      'mota' => 'abc xyz',
      'anh' => 1,
    ),
    1 => array(
      'mota' => 'day la mo ta',
      'anh' => 2,
    ),
    2 => array(
      'mota' => 'mo ta thu 2',
      'anh' => 3,
    ),
  );
  //end test

  if (count($data) > 1) {
    //add perfect_scrollbar here!
    foreach ($data as $key => $value) {
      $t = <<< HTML
        <div class="anhdaidien">Ảnh đại diện: @anhdaidien</div>
    <hr>
        <br>Mô tả: @mota
        <br>Kỹ năng : @kynang
    <hr>
HTML;

      $html .= t($t, array(
        '@anhdaidien' => $value['anh'],
        '@mota' => $value['mota'],
        '@kynang' => 'test',
      ));
    }
    $html .= theme('pager');//phân trang
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

//danh sách kỹ năng trong database
function load_kynangAllKey_array() {
  return array('id' => 'tenkynang');
}