<?php

//define('logo_image_resolution', '124x124');
define('logo_file_size', (2048 * 1024) ); // 2MB

function them_congty_form($form, &$form_state) {
  $form['thong_tin'] = array(
    '#type' => 'fieldset',
    '#title' => t('Thông tin công ty'),
    '#collapsible' => TRUE,
  );
  //autocomplete
  $form['thong_tin']['congty'] = array(
    '#type' => 'textfield',
    '#title' => t('Tìm kiếm công ty'),
    '#required' => TRUE,
    //'#default_value' => isset($cty->tencongty) ? $cty->tencongty : NULL,
  );
  // Nếu không tìm thấy thì thêm mới.
  //Ngược lại . load id_cty , update uid.

}
function them_congty_form_submit($form, &$form_state) {
  try{
    $id_city = 1;
    $uid_origin = load_uid_from_cty($id_city);
    //$uid = << $uid_origin + $uid;

    db_update('thongtin_congty')
      ->fields(array('uid' => $uid))
      ->condition('id', $id_city)
      ->execute();

  }catch (Exception $e) {

  }
}

function thongtin_congty_form($form, &$form_state, $id_cty = NULL) {
  global $user;

  $form = array();
  //check rule

  if($id_cty) {
    $cty = load_cty_info($id_cty);
  }

  $form['id_cty'] = array(
    '#type' => 'value',
    '#value' => $id_cty,
  );

  $form['thong_tin_cong_ty'] = array(
    '#type' => 'fieldset',
    '#title' => t('Thông tin công ty'),
    '#collapsible' => TRUE,
  );
  $form['thong_tin_cong_ty']['tencongty'] = array(
    '#type' => 'textfield',
    '#title' => t('Tên công ty'),
    '#required' => TRUE,
    '#default_value' => isset($cty->tencongty) ? $cty->tencongty : NULL,
  );
  $form['thong_tin_cong_ty']['diachi'] = array(
    '#type' => 'textarea',
    '#title' => t('Địa chỉ'),
    '#required' => TRUE,
    '#default_value' => isset($cty->diachi) ? $cty->diachi : NULL,
  );
  $form['thong_tin_cong_ty']['sodienthoai'] = array(
    '#type' => 'textfield',
    '#title' => t('Số điện thoại'),
    '#required' => TRUE,
    '#default_value' => isset($cty->sdt) ? $cty->sdt : NULL,
  );
  $form['thong_tin_cong_ty']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#required' => TRUE,
    '#default_value' => isset($cty->email) ? $cty->email : NULL,
  );
  $form['thong_tin_cong_ty']['website'] = array(
    '#type' => 'textfield',
    '#title' => t('Website'),
    '#required' => TRUE,
    '#default_value' => isset($cty->website) ? $cty->website : NULL,
  );
  $form['thong_tin_cong_ty']['quymo'] = array(
    '#type' => 'textfield',
    '#title' => t('Quy mô'),
    '#required' => TRUE,
    '#default_value' => isset($cty->quymo) ? $cty->quymo : NULL,
  );
  $form['thong_tin_cong_ty']['logo'] = array(
    '#type' => 'managed_file',
    '#title' => t('Logo'),
    '#required' => TRUE,
    '#default_value' => isset($cty->fid_logo) ? $cty->fid_logo : 0,
    '#upload_validators' => array(
      ///'file_validate_image_resolution' => array(logo_image_resolution),
      'file_validate_size' => array(logo_file_size),
      'file_validate_is_image' => array(),
      'file_validate_extensions' => array('png gif jpg jpeg'),
    ),
    '#upload_location' => "public://" . $user->name .'/logo/',
  );
  $form['thong_tin_cong_ty']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Cập nhật'),
  );

  //$form['#attached']['css']
  //$form['#attached']['js']
  return $form;
}
function thongtin_congty_form_validate($form, &$form_state) {
  $value = $form_state['values'];

  $mail = isset($value['email']) ? $value['email'] : NULL;
  $logo = isset($value['logo']) ? $value['logo'] : NULL;

  //check phone
  $phone = preg_replace("/[^0-9]/", '', trim($value['sodienthoai']));

  if(!$phone || strlen($phone) > 11 || strlen($phone) < 10) {
    form_set_error('sodienthoai', t('Số điện thoại không hợp lệ.'));
  }
  //check email
  if (!valid_email_address($mail)) {
    form_set_error('email', t('Định dạng e-mail không hợp lệ.'));
  }
  //check image
 if($logo) {
   $validate = array(
     ///'file_validate_image_resolution' => array(logo_image_resolution),
     //'file_validate_size' => array(logo_file_size),
     'file_validate_is_image' => array(),
     'file_validate_extensions' => array('png gif jpg jpeg'),
   );
   file_save_upload('logo', $validate);
  }else {
    form_set_error('logo', t('Định dạng logo không hợp lệ.'));
  }
  // Update the file status into the database
  //file_save($file);
}
function thongtin_congty_form_submit($form, &$form_state) {
  global $user;

  $value = $form_state['values'];
  $logo = isset($value['logo']) ? $value['logo'] : NULL;
  //$logo = isset($form_state['storage']['logo_file']) ? $form_state['storage']['logo_file'] : NULL;

  $transaction = db_transaction();
  try {
    $id_cty = isset($value['id_cty']) ? $value['id_cty'] : NULL;

    $file = '';
    if($logo) {
      $file = file_load($logo);
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      //file_usage_add($file, 'cv_tuyendung', 'logo');
    }
    $fields = array(
      'uid' => intval($user->uid),
      'tencongty' => isset($value['tencongty']) ? $value['tencongty'] : NULL,
      'diachi' => isset($value['diachi']) ? $value['diachi'] : NULL,
      'sdt' => isset($value['sodienthoai']) ? $value['sodienthoai'] : NULL,
      'email' => isset($value['email']) ? $value['email'] : NULL,
      'website' => isset($value['website']) ? $value['website'] : NULL,
      'quymo' => isset($value['quymo']) ? $value['quymo'] : NULL,
      'fid_logo' => intval($file->fid),
      'created' => date('Y-m-d H:i:s')
    );
    if($id_cty) {
      db_update('thongtin_congty')->fields($fields)->condition('id', $id_cty)->execute();
    }else{
      $id_cty = db_insert('thongtin_congty')->fields($fields)->execute();
    }

    drupal_set_message(t('Đã cập nhật @id_cty', array('@id_cty' => $id_cty)));
  } catch(Exception $e) {
    drupal_set_message(t('Có lỡi xảy ra'), 'error');
    $transaction->rollback();
    exit();
  }
}

//HIển thị tt cty
function thongtin_congty_chitiet() {
  //
}

function dangtin_tuyendung_form($form, &$form_state, $id_cv = null) {

  $form = array();

  //check rule

  $info = array();
  if($id_cv) {
    $info = load_tintuyendung($id_cv);
    if(isset($info->id)) {
      $baotin = load_baotintuyendung($info->id);
    }
  }
  $form['id_cv'] = array(
    '#type' => 'value',
    '#value' => $id_cv,
  );

  //uid thuoc trong | 1,2,3,4,5,22,223
  global $user;
  $uid = $user->uid;

  $form['id_cty'] = array(
    '#type' => 'value',
    '#value' => load_id_cty($uid),
  );

  $form['thongtin_coban'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tạo tin tuyển dụng mới'),
    '#collapsible' => TRUE,
  );
  $form['thongtin_coban']['tieude'] = array(
    '#type' => 'textfield',
    '#title' => t('Tiêu đề'),
    '#required' => TRUE,
    '#default_value' => isset($info->tieude) ? $info->tieude : NULL,
  );
  $form['thongtin_coban']['nganh'] = array(
    '#type' => 'textarea',
    '#title' => t('Ngành nghề'),
    '#required' => TRUE,
    '#default_value' => isset($info->nganh) ? $info->nganh : NULL,
  );
  $form['thongtin_coban']['noilamviec'] = array(
    '#type' => 'textfield',
    '#title' => t('Nơi làm việc'),
    '#required' => TRUE,
    '#default_value' => isset($info->noilamviec) ? $info->noilamviec : NULL,
  );
  $form['thongtin_coban']['soluong'] = array(
    '#type' => 'textfield',
    '#title' => t('Số lượng cần tuyển'),
    '#required' => TRUE,
    '#default_value' => isset($info->soluong) ? $info->soluong : NULL,
  );
  $form['thongtin_coban']['capbac'] = array(
    '#type' => 'textfield',
    '#title' => t('Cấp bậc'),
    '#required' => TRUE,
    '#default_value' => isset($info->capbac) ? $info->capbac : NULL,
  );
  $form['thongtin_coban']['tglamviec'] = array(
    '#type' => 'textfield',
    '#title' => t('Loại hình làm việc'),
    '#required' => TRUE,
    '#default_value' => isset($info->tglamviec) ? $info->tglamviec : NULL,
  );
  $form['thongtin_coban']['gioitinh'] = array(
    '#type' => 'textfield',
    '#title' => t('Giới tính'),
    '#required' => TRUE,
    '#default_value' => isset($info->gioitinh) ? $info->gioitinh : NULL,
  );
  $form['thongtin_coban']['kinhnghiem'] = array(
    '#type' => 'textfield',
    '#title' => t('Kinh nghiệm'),
    '#required' => TRUE,
    '#default_value' => isset($info->kinhnghiem) ? $info->kinhnghiem : NULL,
  );

  //fix luong
  $luong = array(
    '' => 'Thỏa thuận',
    1 => 'Từ',
    2 => 'Trong khoảng',
  );
  $form['thongtin_coban']['luong'] = array(
    '#type' => 'select',
    '#options' => $luong,
    '#title' => t('Lương'),
    '#required' => TRUE,
    '#default_value' => isset($info->luong) ? $info->luong : NULL,
  );
  $form['thongtin_coban']['luong_tu'] = array(
    '#type' => 'textfield',
    '#title' => 'Mức lương:',
    '#default_value' => t(''),
    '#states' => array(
      'visible' => array(
        ':input[name="luong"]' => array('value' => 1),
      ),
    ),
  );
  //khoang luong
  $form['thongtin_coban']['luongbd'] = array(
    '#type' => 'textfield',
    '#title' => 'Từ lương:',
    '#default_value' => t(''),
    '#states' => array(
      'visible' => array(
        ':input[name="luong"]' => array('value' => 2),
      ),
    ),
  );  $form['thongtin_coban']['luongkt'] = array(
    '#type' => 'textfield',
    '#title' => 'Đến lương:',
    '#default_value' => t(''),
    '#states' => array(
      'visible' => array(
        ':input[name="luong"]' => array('value' => 2),
      ),
    ),
  );
  //end _khoang luong

  $form['thongtin_baotin'] = array(
    '#type' => 'fieldset',
    '#title' => t(''),
    '#collapsible' => TRUE,
  );
  $form['thongtin_baotin']['het_han'] = array(
    '#type' => 'textfield',
    '#title' => t('Hạn chót nộp hồ sơ'),
    '#required' => TRUE,
    '#default_value' => isset($baotin->het_han) ? $baotin->het_han : NULL,
  );
  $form['thongtin_baotin']['nguoitao'] = array(
    '#type' => 'textfield',
    '#title' => t('Tên người nhận hồ sơ'),
    '#required' => TRUE,
    '#default_value' => isset($baotin->nguoitao) ? $baotin->nguoitao : NULL,
  );
  $form['thongtin_baotin']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail nhận hồ sơ'),
    '#required' => TRUE,
    '#default_value' => isset($baotin->email) ? $baotin->email : NULL,
  );
  $form['thongtin_baotin']['sdt'] = array(
    '#type' => 'textfield',
    '#title' => t('Điện thoại'),
    '#required' => TRUE,
    '#default_value' => isset($baotin->sdt) ? $baotin->sdt : NULL,
  );
  $form['thongtin_baotin']['mota_cty'] = array(
    '#type' => 'textarea',
    '#title' => t('Giới thiệu ngắn về công ty'),
    '#required' => TRUE,
    '#default_value' => isset($baotin->mota_cty) ? $baotin->mota_cty : NULL,
  );
  $form['thongtin_baotin']['mota_congviec'] = array(
    '#type' => 'textarea',
    '#title' => t('Mô tả công việc'),
    '#required' => TRUE,
    '#default_value' => isset($baotin->mota_congviec) ? $baotin->mota_congviec : NULL,
  );
  $form['thongtin_baotin']['mota_ungvien'] = array(
    '#type' => 'textarea',
    '#title' => t('Yêu cầu ứng viên'),
    '#required' => TRUE,
    '#default_value' => isset($baotin->mota_ungvien) ? $baotin->mota_ungvien : NULL,
  );
  $form['thongtin_baotin']['mota_quyenloi'] = array(
    '#type' => 'textarea',
    '#title' => t('Quyền lợi ứng viên'),
    '#required' => TRUE,
    '#default_value' => isset($baotin->mota_quyenloi) ? $baotin->mota_quyenloi : NULL,
  );
  $form['thongtin_baotin']['kynang_can'] = array(
    '#type' => 'textarea',
    '#title' => t('Kỹ năng'),
    '#required' => TRUE,
    '#default_value' => isset($baotin->kynang_can) ? $baotin->kynang_can : NULL,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Cập nhật'),
  );

  //$form['#attached']['css']
  //$form['#attached']['js']
  return $form;
}
function dangtin_tuyendung_form_validate($form, &$form_state) {
}
function dangtin_tuyendung_form_submit($form, &$form_state) {
  $value = $form_state['values'];

  $transaction = db_transaction();
  try {

    $id_cv = isset($value['id_cv']) ? $value['id_cv'] : NULL;
    $id_cty = isset($value['id_cty']) ? $value['id_cty'] : NULL;

    $luong = isset($value['luong']) ? $value['luong'] : NULL;
    switch ($luong) {
      case '' :
        break;
      case 1:
        break;
      case 2:
        break;
      default:
        $luong = '';
    }

    $fields_dangtin = array(
      'id_cty' => $id_cty,
      'tieude' => isset($value['tieude']) ? $value['tieude'] : NULL,
      'nganh' => isset($value['nganh']) ? $value['nganh'] : NULL,
      'noilamviec' => isset($value['noilamviec']) ? $value['noilamviec'] : NULL,
      'soluong' => isset($value['soluong']) ? $value['soluong'] : NULL,
      'capbac' => isset($value['capbac']) ? $value['capbac'] : NULL,
      'tglamviec' => isset($value['tglamviec']) ? $value['tglamviec'] : NULL,
      'gioitinh' => isset($value['gioitinh']) ? $value['gioitinh'] : NULL,
      'kinhnghiem' => isset($value['kinhnghiem']) ? $value['kinhnghiem'] : NULL,
      'luong' => $luong
    );

    if($id_cv) {
      db_update('tintuyendung')->fields($fields_dangtin)->condition('id', $id_cv)->execute();
      $id_baotin = $id_cv;
    }else{
      $id_baotin = db_insert('tintuyendung')->fields($fields_dangtin)->execute();
    }

    if($id_baotin) {
      $fields_baotin = array(
        'id_cv' => $id_baotin,
        'het_han' => isset($value['het_han']) ? $value['het_han'] : NULL,
        'uid' => 1,
        'email' => isset($value['email']) ? $value['email'] : NULL,
        'sdt' => isset($value['sdt']) ? $value['sdt'] : NULL,
        'mota_cty' => isset($value['mota_cty']) ? $value['mota_cty'] : NULL,
        'mota_congviec' => isset($value['mota_congviec']) ? $value['mota_congviec'] : NULL,
        'mota_ungvien' => isset($value['mota_ungvien']) ? $value['mota_ungvien'] : NULL,
        'mota_quyenloi' => isset($value['mota_quyenloi']) ? $value['mota_quyenloi'] : NULL,
        'kynang_can' => isset($value['kynang_can']) ? $value['kynang_can'] : NULL,
      );

      if(empty($id_cv)) {
        db_insert('baotintuyendung')->fields($fields_baotin)->execute();
      }else{
        db_update('baotintuyendung')->fields($fields_baotin)->condition('id_cv', $id_baotin)->execute();
      }
    }

   // drupal_set_message(t('Đã cập nhật @id_cv', array('@id_cv' => $id_cv)));
  } catch(Exception $e) {
    drupal_set_message(t('Có lỡi xảy ra'), 'error');
    $transaction->rollback();
    exit();
  }
}

//Tải dữ liệu
/**
 * @param $uid array(1,2,3)
 *
 * @return id cty
 */
function load_id_cty($uid) {
  $id = array();
  if(!is_array($uid)) {
    array_push($id, $uid);
  }else{
    $id = $uid;
  }
  return $id_cty = db_select('thongtin_congty', 'cty')
    ->fields('cty',array('id'))
    ->condition('uid', $id, 'IN')
    ->execute()
    ->fetchField();
}

/**
 * @param $id
 *
 * @return thong tin dang tin tuyen dung
 */
function load_tintuyendung($id) {
  return $id_cty = db_select('tintuyendung', 'td')
    ->fields('td')
    ->condition('id', $id)
    ->execute()
    ->fetchObject();
}

/**
 * @param $id_cv
 *
 * @return thong tin bao tin tuyen dung
 */
function load_baotintuyendung($id_cv) {
  return $id_tt = db_select('baotintuyendung', 'baotin')
    ->fields('baotin')
    ->condition('id_cv', $id_cv)
    ->execute()
    ->fetchObject();
}

/**
 * @param $id_cty
 *
 * @return thong tin co ban cua cong ty
 */
function load_cty_info($id_cty) {
  return $id = db_select('thongtin_congty', 'cty')
    ->fields('cty')
    ->condition('id', $id_cty)
    ->execute()
    ->fetchObject();
}

//kiemtra ton tai