<?php

/**
 * @return mixed
 */
function uit_lichthi_trongso_monhoc() {
  $build['phancong_warapper2'] = drupal_get_form('uit_lichthi_trongso_monhoc_danhsach_form');
  $build['phancong_warapper'] = drupal_get_form('uit_lichthi_trongso_monhoc_form');
  $build['#attached']['css'] = array(drupal_get_path('module', 'descv') . '/css/lichthi.css' => array('group' => CSS_THEME));
  $build['#attached']['js'] = array(drupal_get_path('module', 'descv') . '/css/lichthi.js' => array('group' => JS_THEME));
  return $build;
}
/**
 * @param $filter
 * @return array
 */
function uit_common_field_from_filter($filter) {
  $tmp = array();
  foreach ($filter as $k => $v) {
    $tmp['#' . $k] = $v;
  }
  return $tmp;
}
/**
 * @param $form
 * @param $form_state
 * @return mixed
 */
function uit_lichthi_trongso_monhoc_danhsach_form($form, &$form_state) {
  $form['phancong_warapper2'] = array(
    '#type' => 'fieldset',
    '#title' => t('DANH SÁCH THÔNG TIN MÔN HỌC ĐÃ IMPORT'),
    '#collapsible' => TRUE,
  );

  $filters = trongso_monhoc_filters();
  foreach ($filters as $key => $filter) {
    $form['phancong_warapper2'][$key] = uit_common_field_from_filter($filter);
  }

  $form['phancong_warapper2']['actions'] = array(
    '#type' => 'actions',
  );

  $form['phancong_warapper2']['viewlist'] = array(
    '#type' => 'submit',
    '#prefix' => '<div id="button_import">',
    '#suffix' => '</div>',
    '#value' => t('Xem danh sách'),
  );
  $form['phancong_warapper2']['export_chk'] = array(
    '#type' => 'submit',
    '#prefix' => '<div id="button_check_export">',
    '#suffix' => '</div>',
    '#value' => t('KT Chưa Nhập'),
    '#submit' => array('uit_thongtin_monhoc_check'),
  );
  $form['phancong_warapper2']['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => 'Delete',
    '#submit' => array('uit_thongtin_monhoc_delete'),
    '#attributes' => array('onclick' => 'if(!confirm("Quý Thầy Cô muốn xóa trọng số đã chọn?")){return false;}')
  );

  $rows = uit_lichthi_trongso_monhoc_danhsach();
  global $pager_total_items;
  $form['phancong_warapper2']['qt'] = array('#markup' => t('<div class="total_row clearboth">Tổng : @soluot</div>', array('@soluot' => $pager_total_items[0])));
  $header = array('STT', 'Mã môn học', 'Tên môn học','HT GK', 'TG GK', 'HT TH', 'HT CK', 'TG CK', '% QT', '% TH', '% GK', '% CK', 'Tổng % TS', 'Hệ ĐT', 'CDIO');
  $options = array();
  $i = 1;
  foreach ($rows as $r) {
    $tongts = $r->trongso_quatrinh + $r->trongso_thuchanh + $r->trongso_giuaky + $r->trongso_cuoiky;
    $options[$r->id] = array($i++, $r->mamh, $r->tenmh, $r->hinhthucthi_giuaky, $r->thoigianthi_giuaky, $r->hinhthucthi_thuchanh_cuoiky,
                                   $r->hinhthucthi_cuoiky, $r->thoigianthi_cuoiky, $r->trongso_quatrinh, $r->trongso_thuchanh, $r->trongso_giuaky,
                                    $r->trongso_cuoiky, $tongts, $r->hedt, $r->cdio);
  }

  $form['phancong_warapper2']['ids'] = array(
    '#type' => 'tableselect',
    '#caption' => 'DANH SÁCH THÔNG TIN MÔN HỌC',
    '#header' => $header,
    '#options' => $options,
    '#empty' => 'Chưa có trọng số nào.',
  );
  $form['phancong_warapper2']['pager'] = array('#markup' => theme('pager'));
  drupal_add_library('system', 'drupal.form');
  return $form;
}

/**
 * @param $form
 * @param $form_state
 */
function uit_lichthi_trongso_monhoc_danhsach_form_submit($form, &$form_state) {
  trongso_monhoc_set_filter_form($form_state['values']);
  $form['redirect'] = 'admin/lichthi/trongso-monhoc';
  return;
}

/**
 * @param $form
 * @param $form_state
 * @return mixed
 */
function uit_lichthi_trongso_monhoc_form($form, &$form_state) {
  $form['phancong_warapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('IMPORT THÔNG TIN MÔN HỌC'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );
  $form['phancong_warapper']['hocky'] = array(
    '#type' => 'select',
    '#title' => t('Học kỳ'),
    '#options' => array(1 => t('HK 1'), 2 => t('HK 2'), 3 => t('HK 3')),
    '#default_value' => variable_get('SYSTEM_HOCKY')
  );

  $form['phancong_warapper']['namhoc'] = array(
    '#type' => 'select',
    '#title' => t('Năm học'),
    '#options' => array(2014),
    '#default_value' => variable_get('SYSTEM_NAMHOC')
  );

  $form['phancong_warapper']['file_trongso'] = array(
    '#type' => 'file',
    '#title' => t('File Trọng số'),
  );

  $form['phancong_warapper']['import'] = array(
    '#type' => 'submit',
    '#prefix' => '<div class="clearboth button_import">',
    '#suffix' => '</div>',
    '#value' => t('Nhập Bảng trọng số'),
  );
  $form['phancong_warapper']['download'] = array(
    '#type' => 'submit',
    '#prefix' => '<div class="button_check_export">',
    '#suffix' => '</div>',
    '#value' => t('Tải Trọng số Từ DM Môn học'),
    '#submit' => array('lichthi_export_thongtin_monhoc_hocky_tu_monhoc'),
    '#limit_validation_errors' => array(array('hocky'), array('namhoc'))
  );

  $form['phancong_warapper']['template'] = array(
    '#type' => 'markup',
    '#prefix' => '<div class="clearboth button_import">',
    '#suffix' => '</div>',
    '#markup' => l('Tải file File mẫu', /*uit_common_create_url_template('PhieuThongTinMonhoc.xlsx'), */array('target' => '_blank')),
  );
  return $form;
}


/**
 * @param $form
 * @param $form_state
 */
function uit_lichthi_trongso_monhoc_form_validate($form, &$form_state) {
  $dir = 'public://trongso/import';
  if (!file_prepare_directory($dir, FILE_CREATE_DIRECTORY)) {
    form_set_error('file_trongso', t('Có lỗi xảy ra,Thầy / Cô vui lòng liên hệ với quản trị.'));
  }
  $file = file_save_upload('file_trongso', array('file_validate_extensions' => array('xls xlsx')));
  if ($file) {
    if ($file = file_move($file, $dir)) {
      $form_state['storage']['file'] = $file;
    }
    else {
      form_set_error('file_trongso', t('Không thể tải lên hệ thống, vui lòng kiểm tra lại.'));
    }
  }
  else {
    form_set_error('file_trongso', t('Xin vui lòng chọn file phân công.'));
  }
}

/**
 * @param $form
 * @param $form_state
 */
function uit_lichthi_trongso_monhoc_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $hocky = $values['hocky'];
  $namhoc = $values['namhoc'];
  $file = $form_state['storage']['file'];
  if (!$file) {
    form_set_error('file_trongso', t('Quá trình tải tập tin lên hệ thống bị lỗi.'));
    return;
  }
  $results = import_trongso_monhoc_get_value_from_excel_path(drupal_realpath($file->uri), $hocky, $namhoc);
  $errors = array();
  $datas = array();
  if (isset($results['errors']))
    $errors = $results['errors'];
  if (isset($results['data']))
    $datas = $results['data'];
  $i = 0;
  foreach ($datas as $ts) {
    try {
      $id = import_trongso_monhoc_trongso($ts['mamh'], $ts['hedt'], $ts['hocky'], $ts['namhoc'], $ts['cdio']);
      import_trongso_monhoc_save_update($ts, $id);
      $i++;
    }
    catch (Exception $e) {
      $errors[] = $e->getMessage();
    }
  }
  drupal_set_message(t('Đã cập nhật / thêm mới @count trọng số vào dữ liệu.', array('@count' => $i)));
  if (count($errors) > 0) {
    $html = theme('item_list', array('items' => $errors));
    drupal_set_message($html, 'error');
  }
}

/**
 * @param $filepath
 * @return array
 */
function import_trongso_monhoc_get_value_from_excel_path($filepath, $hocky, $namhoc) {
  require_once PHPEXCEL_LIBRARIES_PATH;
  $objPHPExcel = PHPExcel_IOFactory::load($filepath);
  $activeSheet = $objPHPExcel->setActiveSheetIndex(0);
  $row_start = 7;
  $row_end = $activeSheet->getHighestRow();
  $errors = array();
  $a_data = array();
  $hedts = uit_common_option_hedt();
  $hedts = array_keys($hedts);
  for ($r = $row_start; $r <= $row_end; $r++) {
    $stt = uit_common_get_cell_value($activeSheet, 'A', $r);
    if ($stt && is_numeric($stt)) {
      $mamh = trim(uit_common_get_cell_value($activeSheet, 'C', $r));
      if (!import_trongso_monhoc_mamh_tkb($mamh, $hocky, $namhoc)) {
        $errors[] = t('Dòng @dong có môn học (@mamh) không có trong học kỳ này, vui lòng nhập lại.', array('@dong' => $r, '@mamh' => $mamh));
        continue;
      }
      $cdio = trim(uit_common_get_cell_value($activeSheet, 'N', $r));
      if (!in_array($cdio, array(0, 1))) {
        $errors[] = t('Dòng @dong có CDIO không đúng, vui lòng nhập 1 nếu là lớp thuộc CT CDIO ngược lại nhập 0', array('@dong' => $r));
        continue;
      }
      $quatrinh = trim(uit_common_get_cell_value($activeSheet, 'I', $r));
      $thuchanh = trim(uit_common_get_cell_value($activeSheet, 'J', $r));
      $giuaky = trim(uit_common_get_cell_value($activeSheet, 'K', $r));
      $cuoiky = trim(uit_common_get_cell_value($activeSheet, 'L', $r));
      $tong = (int) ($giuaky + $thuchanh + $quatrinh + $cuoiky);
      if ( 100 != $tong) {
        $errors[] = t('Dòng @dong có mã tổng % = @phantram, vui lòng nhập lại tổng các trọng số phải = 100%.', array('@dong' => $r, '@phantram' => $tong));
        continue;
      }

      $hinhthucthi_giuaky = strtoupper(trim(uit_common_get_cell_value($activeSheet, 'D', $r)));
      $thoigianthi_giuaky = uit_common_get_cell_value($activeSheet, 'E', $r);
      $hinhthucthi_cuoiky = strtoupper(trim(uit_common_get_cell_value($activeSheet, 'F', $r)));
      $thoigianthi_cuoiky = uit_common_get_cell_value($activeSheet, 'G', $r);
      $hinhthucthi_thuchanh_cuoiky = strtoupper(trim(uit_common_get_cell_value($activeSheet, 'H', $r)));

      if ($thuchanh > 0 && !in_array($hinhthucthi_thuchanh_cuoiky, array('TH', 'DA', 'VD')) ) {
        $errors[] = t('Dòng @dong có hình thức thi TH Cuối kỳ không đúng, vui lòng nhập lại thức thi TH Cuối kỳ là một trong: TH, DA, VD', array('@dong' => $r, '@hedts' => drupal_implode_tags($hedts)));
        continue;
      }
      if ($giuaky > 0 && !in_array($hinhthucthi_giuaky, array('TL', 'TN', 'VD', 'DA')) ) {
        $errors[] = t('Dòng @dong có hình thức thi Giữa kỳ không đúng, vui lòng nhập lại thức thi TH Cuối kỳ là một trong: TL, TN', array('@dong' => $r, '@hedts' => drupal_implode_tags($hedts)));
        continue;
      }
      if ($cuoiky > 0 && !in_array($hinhthucthi_cuoiky, array('TL', 'TN', 'DA', 'VD')) ) {
        $errors[] = t('Dòng @dong có hình thức thi Cuối kỳ không đúng, vui lòng nhập lại thức thi Cuối kỳ là một trong: TL, DA, TN, VD', array('@dong' => $r, '@hedts' => drupal_implode_tags($hedts)));
        continue;
      }

      $hedt = trim(uit_common_get_cell_value($activeSheet, 'M', $r));
      if (empty($hedt) || !in_array($hedt, $hedts) ) {
        $errors[] = t('Dòng @dong có hệ đào tạo không đúng, vui lòng nhập lại hệ đào tạo là một trong: @hedts', array('@dong' => $r, '@hedts' => drupal_implode_tags($hedts)));
        continue;
      }

      if ($giuaky > 0 && empty($hinhthucthi_giuaky)) {
        $errors[] = t('Dòng @dong có hình thức thi GK không đúng, vui lòng nhập vào hình thức thi giữa kỳ nếu tỷ lệ đánh giá giữa kỳ > 0', array('@dong' => $r));
        continue;
      }
      if ($cuoiky > 0 && empty($hinhthucthi_cuoiky)) {
        $errors[] = t('Dòng @dong có hình thức thi CK không đúng, vui lòng nhập vào hình thức thi cuối kỳ nếu tỷ lệ đánh giá cuối kỳ > 0', array('@dong' => $r));
        continue;
      }

      $a_data[] = array(
        'mamh' => $mamh,
        'trongso_quatrinh' => (int)$quatrinh,
        'trongso_thuchanh' => (int)$thuchanh,
        'trongso_giuaky' => (int)$giuaky,
        'trongso_cuoiky' => (int)$cuoiky,
        'hinhthucthi_giuaky' => $hinhthucthi_giuaky,
        'thoigianthi_giuaky' => $thoigianthi_giuaky,
        'hinhthucthi_cuoiky' => $hinhthucthi_cuoiky,
        'thoigianthi_cuoiky' => $thoigianthi_cuoiky,
        'hinhthucthi_thuchanh_cuoiky' => $hinhthucthi_thuchanh_cuoiky,
        'hocky' => $hocky,
        'namhoc' => $namhoc,
        'hedt' => $hedt,
        'cdio' => ($cdio ? $cdio : 0),
      );
    }
  }
  return array('errors' => $errors, 'data' => $a_data);
}

/**
 * @param $fields
 * @param $id
 * @return \DatabaseStatementInterface|int
 * @throws \Exception
 */
function import_trongso_monhoc_save_update($fields, $id) {
  global $user;
  $fields['updated_by'] = $user->name;
  if ($id) {
    $fields['updated'] = date('Y-m-d H:i:s');
    return db_update('uit_lichthi_thongtin_monhoc')->fields($fields)->condition('id', $id)->execute();
  }
  $fields['created'] = date('Y-m-d H:i:s');
  return db_insert('uit_lichthi_thongtin_monhoc')->fields($fields)->execute();
}

/**
 * @param $malop
 * @return mixed
 */
function import_trongso_monhoc_mamh_tkb($mamh, $hocky, $namhoc) {
  return db_select('uit_tkb', 's')
      ->fields('s', array('id'))
      ->condition('mamh', $mamh)
      ->condition('hocky', $hocky)
      ->condition('namhoc', $namhoc)
      ->condition('trangthai', 2)
      ->execute()->fetchField();
}

/**
 * @param $malop
 * @return mixed
 */
function import_trongso_monhoc_trongso($mamh, $hedt, $hocky, $namhoc, $cdio) {
  return db_select('uit_lichthi_thongtin_monhoc', 's')
    ->fields('s', array('id'))
    ->condition('mamh', $mamh)
    ->condition('hedt', $hedt)
    ->condition('hocky', $hocky)
    ->condition('namhoc', $namhoc)
    ->condition('cdio', $cdio)
    ->execute()->fetchField();
}

/**
 * @param $total
 */
function uit_lichthi_trongso_monhoc_danhsach() {
  $session = isset($_SESSION['trongso_monhoc_filters']) ? $_SESSION['trongso_monhoc_filters'] : array();
  $hocky = !empty($session['hocky']) ? $session['hocky'] : variable_get('SYSTEM_HOCKY', 2);
  $namhoc = !empty($session['namhoc']) ? $session['namhoc'] : variable_get('SYSTEM_NAMHOC', 2014);
  $cdio = !empty($session['cdio']) ? $session['cdio'] : '';
  $mamh = !empty($session['mamh']) ? $session['mamh'] : '';
  $khoaql = !empty($session['khoaql']) ? $session['khoaql'] : '';

  $q = db_select('uit_lichthi_thongtin_monhoc', 't');
  $q->join('uit_monhoc', 'm', 'm.mamh = t.mamh');
  $q->condition('hocky', $hocky)
    ->condition('namhoc', $namhoc);
  if ($cdio == 1) {
    $q->condition('t.cdio', 1);
  }
  if ($cdio == -1) {
    $q->condition('t.cdio', 1, '<>');
  }
  if ($mamh) {
    $q->condition('t.mamh', $mamh);
  }
  if ($khoaql) {
    $q->condition('m.khoaql', $khoaql);
  }
  $count_query = clone $q;
  $count_query->addExpression('COUNT(t.id)');
  $q = $q->extend('PagerDefault');
  $q->fields('t');
  $q->fields('m', array('tenmh'));
  $q->limit(variable_get('SYSTEM_SODONG', 25))
    ->orderBy('t.id', 'DESC')
    ->setCountQuery($count_query);
  return $q->execute();
}

/**
 * @return array
 */
function trongso_monhoc_filters() {
  $filters = array();
  $session = isset($_SESSION['trongso_monhoc_filters']) ? $_SESSION['trongso_monhoc_filters'] : array();
  $hocky = !empty($session['hocky']) ? $session['hocky'] : variable_get('SYSTEM_HOCKY', 2);
  $namhoc = !empty($session['namhoc']) ? $session['namhoc'] : variable_get('SYSTEM_NAMHOC', 2014);
  $cdio = !empty($session['cdio']) ? $session['cdio'] : '';
  $mamh = !empty($session['mamh']) ? $session['mamh'] : '';
  $khoaql = !empty($session['khoaql']) ? $session['khoaql'] : '';

  $filters['mamh'] = array(
    'type' => 'textfield',
    'title' => t('Mã môn học'),
    'default_value' => $mamh
  );
  $filters['khoaql'] = array(
    'type' => 'select',
    'title' => t('Khoa QL'),
    'default_value' => $khoaql,
    'options' => array('' => '-Tất cả-', 'KHMT' => 'KHMT', 'MMT&TT' => 'MMT&TT', 'KTMT' => 'KTMT', 'HTTT' => 'HTTT', 'CNPM' => 'CNPM', 'KTTT' => 'KTTT'),
  );
  $filters['cdio'] = array(
    'type' => 'select',
    'title' => t('CDIO'),
    'options' => array('' => ('-Tất cả-'), 1 => 'Lớp CDIO', -1 => 'Lớp thường'),
    'default_value' => $cdio
  );

  $filters['hocky'] = array(
    'type' => 'select',
    'title' => t('Học kỳ'),
    'options' => array(1 => 'HK 1',2 => 'HK 2',3 => 'HK 3'),
    'default_value' => $hocky
  );
  $filters['namhoc'] = array(
    'type' => 'select',
    'title' => t('Năm học'),
    'options' => array(2014),
    'default_value' => $namhoc
  );
  return $filters;
}

/**
 * @param $form_state
 */
function trongso_monhoc_set_filter_form($values) {
  $filters = trongso_monhoc_filters();
  //Apply every filter that has a choice selected other than 'any'.
  foreach ($filters as $filter => $options) {
    if (isset($values[$filter]) && $values[$filter] != '') {
      // Flatten the options array to accommodate hierarchical/nested options.
      if (isset($filters[$filter]['options'])) {
        $flat_options = form_options_flatten($filters[$filter]['options']);
        // Only accept valid selections offered on the dropdown, block bad input.
        if (isset($flat_options[$values[$filter]])) {
          $_SESSION['trongso_monhoc_filters'][$filter] = $values[$filter];
        }
      }
      else {
        $_SESSION['trongso_monhoc_filters'][$filter] = $values[$filter];
      }
    }
    else {
      unset($_SESSION['trongso_monhoc_filters'][$filter]);
    }
  }
}

/**
 * @param $form
 * @param $form_state
 */
function uit_thongtin_monhoc_delete($form, &$form_state) {
  $values = $form_state['values'];
  $ids = array_filter($values['ids']);
  if (count($ids) == 0) {
    form_set_error('ids', 'Thầy Cô vui lòng chọn ít nhất 1 dòng để xóa.');
    return;
  }
  $trans = db_transaction();
  try {
    $mamhs = db_select('uit_lichthi_thongtin_monhoc', 'm')->condition('id', $ids, 'IN')->fields('m', array('mamh'))->execute()->fetchCol();
    db_delete('uit_lichthi_thongtin_monhoc')->condition('id', $ids, 'IN')->execute();
    drupal_set_message('Đã xóa thành công các thông tin môn học: ' . drupal_implode_tags($mamhs));
    ghilogs('Đã xóa thành công các thông tin môn học: ' . drupal_implode_tags($mamhs), 'Lichthi-PhanCong', 'PHANCONG');
    drupal_goto('admin/lichthi/thongtin-monhoc');
  }
  catch (Exception $e) {
    $trans->rollback();
    uit_common_mail_to_webmaster('TTMH', 'Xóa thông tin môn học bị lỗi: ' . $e->getMessage());
  }
}

/**
 * @param $form
 * @param $form_state
 */
function uit_thongtin_monhoc_check($form, &$form_state) {
  $hocky = $form_state['values']['hocky'];
  $namhoc = $form_state['values']['namhoc'];
  $rs = uit_thongtin_monhoc_mamh_khoa($hocky, $namhoc);
  if (count($rs) > 0) {
    //drupal_set_message('Các môn chưa có thông tin trọng số: ' . drupal_implode_tags($rs));
    $path = uit_thongtin_monhoc_export_miss($rs, $hocky, $namhoc);
    if (file_exists($path)) {
      if (filesize($path)) {
        drupal_add_http_header('Pragma', 'public');
        drupal_add_http_header('Expires', '0');
        drupal_add_http_header('Cache-Control', 'must-revalidate, post-check=0, pre-check=0');
        drupal_add_http_header('Content-Type', 'application/application/x-zip');
        drupal_add_http_header('Content-Disposition', 'attachment; filename=' . basename($path) . ';');
        drupal_add_http_header('Content-Transfer-Encoding', 'binary');
        drupal_add_http_header('Content-Length', filesize($path));
        ob_clean();
        flush();
        readfile($path);
        unlink($path);
        drupal_set_message(t('Xuất dữ liệu thành công.'));
      }
    }
  }
  else {
    drupal_set_message('Thông tin trọng số đã được nhập đầy đủ.');
  }
  drupal_goto('admin/lichthi/thongtin-monhoc');
}

/**
 * @param $hocky
 * @param $namhoc
 * @return array|bool
 */
function uit_thongtin_monhoc_mamh_khoa($hocky, $namhoc) {
  $q = db_select('uit_tkb', 't')->distinct();
  $q->join('uit_monhoc', 'm', 'm.mamh = t.mamh');
  $q->condition('t.hocky', $hocky)
      ->condition('t.namhoc', $namhoc)
      ->condition('t.hedt', 'VB2CQ' , '<>')
      ->condition('t.trangthai', 2);
  $q->fields('t', array('mamh', 'cdio', 'hedt', 'khoaql'));
  $q->fields('m', array('tenmh'));
  $q->orderBy('m.mamh');
  $tkbs = $q->execute()->fetchAll();
  if (count($tkbs) == 0) {
    return FALSE;
  }
  $rs = array();
  foreach ($tkbs as &$t) {
    $hedts = explode(",", $t->hedt);
    foreach ($hedts as $hedt) {
      $obj = _uit_service_diemthi_trongtin_monhoc($t->mamh, $t->cdio, $hocky, $namhoc, $hedt);
      if (!$obj) {
        $k = $t->mamh . $t->cdio . $hedt;
        $t->hedt = $hedt;
        $rs[$k] = array(
          'mamh' => $t->mamh,
          'tenmh' => $t->tenmh,
          'cdio' => $t->cdio,
          'khoaql' => $t->khoaql,
          'hedt' => $hedt,
          'hocky' => $hocky,
          'namhoc' => $namhoc
        );
      }
    }
  }
  return $rs;
}

/**
 *
 * @param type $rows
 * @param type $hocky
 * @param type $namhoc
 * @return string
 */
function uit_thongtin_monhoc_export_miss($rows, $hocky, $namhoc) {
  require_once PHP_EXCEL_LIBRARIES;
  $num_rows = count($rows);
  if ($num_rows == 0)
    return FALSE;
  $filename = 'ThongTinMonHoc_HK' . $hocky . '_NH' . $namhoc . '.xlsx';
  $path = variable_get('file_public_path', conf_path() . '/files') . '/exports/lichthi/' . $filename;
  $templatefile = drupal_get_path('module', 'uit_lichthi') . '/exports/templates/PhieuThongTinMonhoc.xlsx';

  $objPHPExcel = PHPExcel_IOFactory::load($templatefile);
  $objPHPExcel->getSheet(0)->setTitle('TTMH');
  $objPHPExcel->setActiveSheetIndex(0);
  $r = 7;
  $objPHPExcel->getActiveSheet()->insertNewRowBefore($r + 1, ($num_rows - 1));
  $i = 1;
  foreach ($rows as $row) {
    $objPHPExcel->setActiveSheetIndex(0)
      ->setCellValue('A' . $r, $i++)
      ->setCellValue('B' . $r, $row['khoaql'])
      ->setCellValue('C' . $r, $row['mamh'])
      ->setCellValue('M' . $r, $row['hedt'])
      ->setCellValue('N' . $r, $row['cdio'])
      ->setCellValue('O' . $r, $row['hocky'])
      ->setCellValue('P' . $r, $row['namhoc'])
      ->setCellValue('Q' . $r, $row['tenmh'])
    ;
    $r++;
  }
  $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel2007');
  $objWriter->save($path);
  return $path;
}

/**
 *
 */
function lichthi_export_thongtin_monhoc_hocky_tu_monhoc($form, &$form_state) {
  require_once PHP_EXCEL_LIBRARIES;
  $hocky = $form_state['values']['hocky'];
  $namhoc = $form_state['values']['namhoc'];
  $tkbs = lichthi_danhsach_tkb($hocky, $namhoc);
  if (count($tkbs) == 0) {
    drupal_set_message('Không có dữ liệu: học kỳ ' . $hocky . ' năm học ' . $namhoc);
    drupal_goto('admin/lichthi/thongtin-monhoc');
    return;
  }
  $filename = 'ThongTinMonHoc_HK' . $hocky . '_NH' . $namhoc . '.xlsx';
  $path = variable_get('file_public_path', conf_path() . '/files') . '/exports/lichthi/' . $filename;
  $templatefile = drupal_get_path('module', 'uit_lichthi') . '/exports/templates/PhieuThongTinMonhoc.xlsx';

  $objPHPExcel = PHPExcel_IOFactory::load($templatefile);
  $objPHPExcel->getSheet(0)->setTitle('TTMH');
  $objPHPExcel->setActiveSheetIndex(0);
  $r = 7;
  $i = 1;
  foreach ($tkbs as $tkb) {
    $hedts = explode(',', $tkb->hedt);
    foreach ($hedts as $hedt) {
      if ($hedt == 'VB2CQ') continue;
      $objPHPExcel->getActiveSheet()->insertNewRowBefore($r + 1, 1);
      $ttmonhoc = lichthi_thongtin_trongso_monhoc_tkb($tkb->mamh, $hedt);
      $objPHPExcel->setActiveSheetIndex(0)
        ->setCellValue('A' . $r, $i++)
        ->setCellValue('B' . $r, $tkb->khoaql)
        ->setCellValue('C' . $r, $tkb->mamh)
        ->setCellValue('D' . $r, ($ttmonhoc->htthi_lt_gk == 'TL-TN' ? 'TL' : $ttmonhoc->htthi_lt_gk))
        ->setCellValue('E' . $r, $ttmonhoc->htthi_lt_gk_tg)
        ->setCellValue('F' . $r, ($ttmonhoc->htthi_lt_ck == 'TL-TN' ? 'TL' : $ttmonhoc->htthi_lt_ck))
        ->setCellValue('G' . $r, $ttmonhoc->htthi_lt_ck_tg)
        ->setCellValue('H' . $r, $ttmonhoc->htthi_th_ck)
        ->setCellValue('I' . $r, $ttmonhoc->tldanhgia_qt)
        ->setCellValue('J' . $r, $ttmonhoc->tldanhgia_th)
        ->setCellValue('K' . $r, $ttmonhoc->tldanhgia_gk)
        ->setCellValue('L' . $r, $ttmonhoc->tldanhgia_ck)
        ->setCellValue('M' . $r, $hedt)
        ->setCellValue('N' . $r, 0)
        ->setCellValue('O' . $r, $hocky)
        ->setCellValue('P' . $r, $namhoc)
        ->setCellValue('Q' . $r, $ttmonhoc->tenmh)
      ;
      $r++;
    }
  }
  $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel2007');
  $objWriter->save($path);
  if (file_exists($path)) {
    if (filesize($path)) {
      drupal_add_http_header('Pragma', 'public');
      drupal_add_http_header('Expires', '0');
      drupal_add_http_header('Cache-Control', 'must-revalidate, post-check=0, pre-check=0');
      drupal_add_http_header('Content-Type', 'application/application/x-zip');
      drupal_add_http_header('Content-Disposition', 'attachment; filename=' . basename($path) . ';');
      drupal_add_http_header('Content-Transfer-Encoding', 'binary');
      drupal_add_http_header('Content-Length', filesize($path));
      ob_clean();
      flush();
      readfile($path);
      unlink($path);
      drupal_set_message(t('Xuất dữ liệu thành công.'));
    }
  }
}

/**
 * @param $hocky
 * @param $namhoc
 */
function lichthi_thongtin_trongso_monhoc_tkb($mamh, $hedt) {
  $q = db_select('uit_monhoc', 'm');
  $q->condition('mamh', $mamh);
  $q->condition('hedt', '%' . db_like($hedt) . '%', 'LIKE');
  $q->fields('m', array('mamh', 'tenmh', 'tldanhgia_qt', 'tldanhgia_th', 'tldanhgia_gk', 'tldanhgia_ck', 'htthi_lt_gk', 'htthi_lt_gk_tg', 'htthi_lt_ck', 'htthi_lt_ck_tg', 'htthi_th_ck', 'hedt', 'khoaql'));
  return $q->execute()->fetchObject();
}

/**
 * @param $hocky
 * @param $namhoc
 * @param int $trangthai
 */
function lichthi_danhsach_tkb($hocky, $namhoc, $trangthai = 2) {
  $q = db_select('uit_tkb', 't')->distinct()->fields('t', array('mamh', 'hedt', 'khoaql'));
  $q->condition('t.trangthai', $trangthai)
    ->condition('t.hocky', $hocky)
    ->condition('t.namhoc', $namhoc);
  return $q->execute()->fetchAll();
}