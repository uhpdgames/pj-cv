<?php

//define('logo_image_resolution', '124x124');
define('logo_file_size', (2048 * 1024) ); // 2 MB

function them_congty_form($form, &$form_state) {
  $form['thong_tin'] = array(
    '#type' => 'fieldset',
    '#title' => t('Thông tin công ty'),
    '#collapsible' => TRUE,
  );

  $form['thong_tin']['congty'] = array(
    '#type' => 'textfield',
    '#title' => t('Tìm kiếm công ty'),
    '#autocomplete_path' => 'timcongty/autocomplete',
  );
  // Nếu không tìm thấy thì thêm mới.
  //Ngược lại . load công ty , update lại uid.

  if($form_state['values']['congty'] == null){
    $form['thong_tin']['themmoi_cty'] = array(
      '#markup' => t('<br>Nếu bên trên không tìm thấy thông tin công tin của bạn: ') . l('Thêm mới 1 công ty', 'quanly/thong-tin-cong-ty')
    );
  }
  return $form;
}
function them_congty_form_validate($form, &$form_state) {
  if($form_state['values']['congty'] == null || !isset($form_state['values']['congty'])) {
    form_set_error('congty', t('Tên công ty không được để trống.'));
  }
}
function them_congty_form_submit($form, &$form_state) {
  global $user;
  $tencty = isset($form_state['values']['congty']) ? $form_state['values']['congty'] : '';
  
  $transaction = db_transaction();
  try{
    $uid_origin = load_uid_from_cty($tencty);
    if(isset($uid_origin)) {
      $uid_origin .= t(',@uid_new', array('@uid_new'=>$user->uid));
    }else{
      $uid_origin = $user->uid;
    }

    db_update('thongtin_congty')
      ->fields(array('uid' => $uid_origin))
      ->condition('tencongty', $tencty)
      ->execute();

    drupal_set_message(t('Đã thêm vào @cty', array('@cty' => $tencty)));
  }catch (Exception $e) {
    drupal_set_message(t('Có lổi xảy ra'), 'error');
    $transaction->rollback();
    exit();
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

  $tencongty = isset($form_state['values']['tencongty']) ? $form_state['values']['tencongty'] : NULL;

  if(isset($tencongty)) {
    if($tcytontai = kiemtra_toitai_congty($tencongty)) {
      $form['congtydaco'] = array(
        '#prefix' => '<div id="congtydatontai-div">',
        '#suffix' => '</div>',
        '#markup' => t('@title có phải là công ty của bạn muốn tìm<br>', array(
            '@title' => $tcytontai)).l('Ấn vào đây ', 'them-moi/thong-tin-cong-ty').
          t('để thêm vào tự động')
      );
    }
  }
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
    '#ajax' => array(
      'callback' => 'ajax_tencongtintontai_callback',
      'wrapper' => 'congtydatontai-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
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
function ajax_tencongtintontai_callback($form, &$form_state) {
  return $form['congtydaco'];
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
    drupal_set_message(t('Có lổi xảy ra'), 'error');
    $transaction->rollback();
    exit();
  }
}

//HIển thị thông tin cty x
function thongtin_congty_chitiet($id_cty) {
  if(empty($id_cty)) {
    return t('Đường dẫn thông tin công tin chưa đúng');
  }

  $info = load_cty_info($id_cty);

  //$tintuyendung  = load_tintuyendung_cuacongty_x($id_cty);
  //$load_them_thongtintaiday  = load_abc();


  $html = <<<HTML
  <div class="thongtincty-banner">@banner</div>
  <div class="thongtincty-logo"> @logo </div>
  
  <hr>
  
  Tên công ty: @tencty
  <br>Số điện thoại: @sdt
  <br>Số nhân viên: @soluongnhanvien
  <br>Website: @website
  <br>Mô tả : @mota
  <br> <a href="/danhgiacongtin/">đánh giá</a>
  <br> <a href="/theodoicongty/">theo dõi</a>

  <hr>

  Vị trí tuyển dụng
  @danhsach_dangtin_tuyendung
  
  <hr>
  
  Đánh giá
  @danhsach_danhgia
  
  <hr>
  Địa chỉ : @maps
HTML;

  return t($html, array(
    '@banner' => 'đặt banner công ty',
    '@logo' => 'đặt logo',
    '@tencty' => $info->tencongty,
    '@sdt' => $info->sdt,
    '@soluongnhanvien' => $info->quymo,
    '@website' => $info->website,
    '@mota' => 'abc',
    '@danhsach_dangtin_tuyendung' => 'list abc',//list(),
    '@danhsach_danhgia' => 'chưa có đánh giá',
    '@maps' => $info->diachi,
  ));

}

//đăng tin mới
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

  $luong = array(
    -1 => 'Thỏa thuận',
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
  );
  $form['thongtin_coban']['luongkt'] = array(
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
    '#type' => 'textfield',
    '#title' => t('Kỹ năng'),
    '#required' => TRUE,
    '#default_value' => isset($baotin->kynang_can) ? $baotin->kynang_can : NULL,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Đăng tin'),
  );

  //$form['#attached']['css']
  //$form['#attached']['js']
  return $form;
}
function dangtin_tuyendung_form_validate($form, &$form_state) {
}
function dangtin_tuyendung_form_submit($form, &$form_state) {
  global $user;
  $value = $form_state['values'];

  $transaction = db_transaction();
  try {

    $id_cv = isset($value['id_cv']) ? $value['id_cv'] : NULL;
    $id_cty = isset($value['id_cty']) ? $value['id_cty'] : NULL;

    $luong = isset($value['luong']) ? $value['luong'] : NULL;
    switch ($luong) {
      case '' :
        $luong_tu = $luong;
        break;
      case 1:
        $luong_tu = isset($value['luong_tu']) ? $value['luong_tu'] : NULL;
        break;
      case 2:
        $luong_tu = isset($value['luongbd']) ? $value['luongbd'] : NULL;
        $luong_den = isset($value['luongkt']) ? $value['luongkt'] : NULL;
        break;
      default:
        $luong_tu = $luong_den = '';
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
      'luong_tu' => $luong_tu,
      'luong_den' => $luong_den
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
        'uid' => $user->uid,
        'email' => isset($value['email']) ? $value['email'] : NULL,
        'sdt' => isset($value['sdt']) ? $value['sdt'] : NULL,
        'mota_cty' => isset($value['mota_cty']) ? $value['mota_cty'] : NULL,
        'mota_congviec' => isset($value['mota_congviec']) ? $value['mota_congviec'] : NULL,
        'mota_ungvien' => isset($value['mota_ungvien']) ? $value['mota_ungvien'] : NULL,
        'mota_quyenloi' => isset($value['mota_quyenloi']) ? $value['mota_quyenloi'] : NULL,
        'kynang_can' => isset($value['kynang_can']) ? $value['kynang_can'] : NULL,
        'trangthai' => 0
      );

      if(empty($id_cv)) {
        db_insert('baotintuyendung')->fields($fields_baotin)->execute();
      }else{
        db_update('baotintuyendung')->fields($fields_baotin)->condition('id_cv', $id_baotin)->execute();
      }
    }

   // drupal_set_message(t('Đã cập nhật @id_cv', array('@id_cv' => $id_cv)));
  } catch(Exception $e) {
    drupal_set_message(t('Có lổi xảy ra'), 'error');
    $transaction->rollback();
    exit();
  }
}

function danhsach_dangtin_tuyendung_form($form, &$form_state) {
  $form['thongtin_coban'] = array(
    '#type' => 'fieldset',
    '#title' => t('Danh sách tin tuyển dụng'),
    '#collapsible' => TRUE,
  );
  global $user;

  $id_cty = load_id_cty($user->uid);

  if($id_cty) {
    //query
    $q = db_select('tintuyendung', 'td')
      ->extend('PagerDefault')//phân trang
      ->fields('td')
      ->condition('id_cty', $id_cty)
      ->execute();

    //render
    $output = '<table>';
    while ($value = $q->fetchAssoc()) {

      $load_baotin = load_baotintuyendung($value['id']);

      $html = <<< HTML
    <tr>
      <td style="color: red">@tieude</td>
      <td>Mã tin: @matin</td>
      <td>Hạn chót: @hethan</td>
      <td colspan="2"> (count) CV Mới</td>
    </tr>
        
    <tr>
        <td>Link chinh sửa | Xóa | Yêu cầu hiển thị</td>
        <td>Trạng thái @trangthai</td>
        <td>Ngày đăng tin @ngaydangtin</td>
    </tr>

HTML;
      $output .= t($html, array(
        '@tieude' => $value['tieude'],
        '@matin' => $value['id'],
        '@hethan' => $load_baotin->het_han,
        '@trangthai' => 1,
        '@ngaydangtin' => $value['created'],

      ));

    }
//(count) CV Mới : link CV đã nộp cho công tin này
    $output .= '</table>';
    //$output .= theme('pager'); //phân trang
  }

  $form['thongtin_coban']['danhsach'] = array('#markup' => $output);

  return $form;
}

//ứng tuyển
function danhsach_ungtuyen_tuyendung_form($form, &$form_state) {
  $form['thongtin_coban'] = array(
    '#type' => 'fieldset',
    '#title' => t('Danh sách hồ sơ ứng tuyển'),
    '#collapsible' => TRUE,
  );
  $id_cty = 5;
  $form['thongtin_coban']['tentintuyendung'] = array(
    '#type' => 'select',
    '#options' => array(''=> 'Tất cả') + load_AllKey_tintuyendung($id_cty),
    '#title' => t('Tin tuyển dụng'),
   // '#default_value' => NULL,
  );
  $op_trangthai = array(
    0=>'Chưa duyệt',
    1=>'Đạt yêu cầu',
    -1=>'Không đạt yêu cầu',
  );
  $form['thongtin_coban']['trangthai'] = array(
    '#type' => 'select',
    '#options' => $op_trangthai,
    '#title' => t('Tin tuyển dụng'),
   // '#default_value' => NULL,
  );

  $form['thongtin_coban']['danhsach'] = array();

  return $form;
}
//Tải dữ liệu
/**
 * @param $uid array(1,2,3)
 *
 * @return id cty
 */
function load_id_cty($uid) {
 // $id = array();
/*  if(!is_array($uid)) {
    array_push($id, $uid);
  }else{
    $id = $uid;
  }*/

  return $id_cty = db_select('thongtin_congty', 'cty')
    ->fields('cty', array('id'))
    ->condition('uid', '%' . db_like($uid) . '%', 'LIKE')
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

function load_uid_from_cty($tencty) {
  return $uid = db_select('thongtin_congty', 'cty')
    ->fields('cty', array('uid'))
    ->condition('tencongty', $tencty) //not like
    ->execute()
    ->fetchField();
}

function load_AllKey_tintuyendung($id_cty) {
  return $tintd = db_select('tintuyendung', 'td')
    ->fields('td', array('id', 'tieude'))
    ->condition('id', $id_cty)
    ->execute()
    ->fetchAllKeyed();
}
//kiemtra ton tai
function kiemtra_toitai_congty($tencty = ''){
  return $cty = db_select('thongtin_congty', 'cty')
    ->fields('cty', array('tencongty'))
    ->condition('tencongty', db_like($tencty) . '%', 'LIKE')
    ->execute()
    ->fetchField();
}