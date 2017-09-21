<?php 
function descv_add_cv($form, &$form_state) {
      $form = array();

  $form['add'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Tiêu đề CV'),
  );
  $form['add']['tieude'] = array(
    '#type'  => 'textfield',
    '#title' => t('Tiêu đề'),
    '#required' => TRUE,
  );
//// 1. THÔNG TIN CÁ NHÂN    
  $form['info'] = array(
    '#type'  => 'fieldset',
    '#title' => t('THÔNG TIN CÁ NHÂN'),
    '#size'  => '100%',
    '#attributes' => array('class' => array('row')),
  );
  $form['info']['anhdaidien'] = array(
    '#type'  => 'file',
    '#title' => t('Hình đại điện'),
  );
  $form['info']['hoten'] = array(
    '#type'  => 'textfield',
    '#title' => t('Họ Tên'),
    '#required' => TRUE,
  );  
  $form['info']['vitrimongmuon'] = array(
    '#type'  => 'textfield',
    '#title' => t('Vị Trí Ứng Tuyển'),
  );      
   $form['info']['ngaysinh'] = array(
    '#type'  => 'textfield',
    '#title' => t('Ngày sinh'),
  );  
    $form['info']['gioitinh'] = array(
    '#type'  => 'textfield',
    '#title' => t('Giới tính'),
  );  
    $form['info']['sdt'] = array(
    '#type'  => 'textfield',
    '#title' => t('Số điện thoại'),
    '#required' => TRUE,
  );   
    $form['info']['email'] = array(
    '#type'  => 'textfield',
    '#title' => t('Email'),
    '#required' => TRUE,
  );   
    $form['info']['diachi'] = array(
    '#type'  => 'textfield',
    '#title' => t('Địa chỉ'),
  );   
    $form['info']['website'] = array(
    '#type'  => 'textfield',
    '#title' => t('Facebook, Website, Blog'),
  );  
    $form['info']['sapxep'] = array(
    '#type'  => 'textfield',
    '#title' => t('Sắp xếp'),
  );  
/////// 2. MỤC TIÊU NGHỀ NGHIỆP
    $form['muctieu'] = array(
    '#type'  => 'fieldset',
    '#title' => t('MỤC TIÊU NGHỀ NGHIỆP'),
    '#size'  => '100%',
    '#attributes' => array('class' => array('row')),
  );
    if (empty($form_state['num_muctieu'])) {
        $form_state['num_muctieu'] = 1;
      }
    for ($i = 0; $i < $form_state['num_muctieu']; $i++) {
        $form['muctieu']['muctieu_value'][$i] = array(
        '#type'  => 'textfield',
        '#title' => t('Mục tiêu'),
        '#required' => TRUE,
        );     
    }
        $form['muctieu']['add_muctieu'] = array(
        '#type' => 'submit',
        '#value' => t('Thêm mục tiêu'),
        '#submit' => array('ajax_example_add_more_add_one'),
        // See the examples in ajax_example.module for more details on the
        // properties of #ajax.
        '#ajax' => array(
          'callback' => 'ajax_example_add_more_callback',
          'wrapper' => 'names-fieldset-wrapper',
        ),
      );
      if ($form_state['num_muctieu'] > 1) {
        $form['muctieu']['remove_muctieu'] = array(
          '#type' => 'submit',
          '#value' => t('Remove one'),
          '#submit' => array('ajax_example_add_more_remove_one'),
          '#ajax' => array(
            'callback' => 'ajax_example_add_more_callback',
            'wrapper' => 'names-fieldset-wrapper',
          ),
        );
      }
/////// 3. KỸ NĂNG 
    $form['kynang'] = array(
    '#type'  => 'fieldset',
    '#title' => t('KỸ NĂNG'),
    '#size'  => '100%',
    '#attributes' => array('class' => array('row')),
  );
    $form['kynang']['kynang_loai'] = array(
    '#type'  => 'textfield',
    '#title' => t('Tên kỹ năng'),
    '#required' => TRUE,
  ); 
    $form['kynang']['kynang_mucdo'] = array(
    '#type'  => 'select',
    '#title' => t('Mức độ'),
    '#options'=> array(t('1'), t('2'), t('3'), t('4'), t('5')),
    '#required' => TRUE,
  ); 
/////// 4. SỞ THÍCH
    $form['sothich'] = array(
    '#type'  => 'fieldset',
    '#title' => t('SỞ THÍCH'),
    '#size'  => '100%',
    '#attributes' => array('class' => array('row')),
  );
    $form['sothich']['sothich_value'] = array(
    '#type'  => 'textfield',
    '#title' => t('Sở thích'),
    '#required' => TRUE,
  );
 /////// 5. NGƯỜI THAM CHIẾU 
    $form['thamchieu'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Tham chiếu'),
    '#size'  => '100%',
    '#attributes' => array('class' => array('row')),
  );
    $form['thamchieu']['thamchieu_value'] = array(
    '#type'  => 'textfield',
    '#title' => t('Tham chiếu'),
    '#required' => TRUE,
  );     
 /////// 6. HỌC VẤN
    $form['hocvan'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Học vấn'),
    '#size'  => '100%',
    '#attributes' => array('class' => array('row')),
  );
    $form['hocvan']['hocvan_batdau'] = array(
    '#type'  => 'textfield',
    '#title' => t('Bắt đầu'),
    '#required' => TRUE,
  );     
    $form['hocvan']['hocvan_ketthuc'] = array(
    '#type'  => 'textfield',
    '#title' => t('Kết thúc'),
    '#required' => TRUE,
  );  
    $form['hocvan']['hocvan_tentruong'] = array(
    '#type'  => 'textfield',
    '#title' => t('Tên trường'),
    '#required' => TRUE,
  );  
    $form['hocvan']['hocvan_nganh'] = array(
    '#type'  => 'textfield',
    '#title' => t('Ngành học'),
    '#required' => TRUE,
  );  
    $form['hocvan']['hocvan_chitiet'] = array(
    '#type'  => 'textfield',
    '#title' => t('Chi tiết'),
    '#required' => TRUE,
  );  
 /////// 7. KINH NGHIỆM LÀM VIỆC
    $form['kinhnghiem'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Kinh nghiệm làm việc'),
    '#size'  => '100%',
    '#attributes' => array('class' => array('row')),
  );
    $form['kinhnghiem']['kinhnghiem_batdau'] = array(
    '#type'  => 'textfield',
    '#title' => t('Bắt đầu'),
    '#required' => TRUE,
  );     
    $form['kinhnghiem']['kinhnghiem_ketthuc'] = array(
    '#type'  => 'textfield',
    '#title' => t('Kết thúc'),
    '#required' => TRUE,
  );  
    $form['kinhnghiem']['kinhnghiem_congty'] = array(
    '#type'  => 'textfield',
    '#title' => t('Tên công ty'),
    '#required' => TRUE,
  );  
    $form['kinhnghiem']['kinhnghiem_vitri'] = array(
    '#type'  => 'textfield',
    '#title' => t('Vị trí làm việc'),
    '#required' => TRUE,
  );  
    $form['kinhnghiem']['kinhnghiem_chitiet'] = array(
    '#type'  => 'textfield',
    '#title' => t('Chi tiết'),
    '#required' => TRUE,
  );  
  /////// 8. HOẠT ĐỘNG
    $form['hoatdong'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Hoạt động'),
    '#size'  => '100%',
    '#attributes' => array('class' => array('row')),
  );
    $form['hoatdong']['hoatdong_batdau'] = array(
    '#type'  => 'textfield',
    '#title' => t('Bắt đầu'),
    '#required' => TRUE,
  );     
    $form['hoatdong']['hoatdong_ketthuc'] = array(
    '#type'  => 'textfield',
    '#title' => t('Kết thúc'),
    '#required' => TRUE,
  );  
    $form['hoatdong']['hoatdong_ten'] = array(
    '#type'  => 'textfield',
    '#title' => t('Tên hoạt động'),
    '#required' => TRUE,
  );  
    $form['hoatdong']['hoatdong_vitri'] = array(
    '#type'  => 'textfield',
    '#title' => t('Vị trí tham gia'),
    '#required' => TRUE,
  );  
    $form['hoatdong']['hoatdong_chitiet'] = array(
    '#type'  => 'textfield',
    '#title' => t('Chi tiết'),
    '#required' => TRUE,
  );  
  /////// 9. CHỨNG CHỈ
    $form['chungchi'] = array(
    '#type'  => 'fieldset',
    '#title' => t('CHỨNG CHỈ'),
    '#size'  => '100%',
    '#attributes' => array('class' => array('row')),
  );
    $form['chungchi']['chungchi_ten'] = array(
    '#type'  => 'textfield',
    '#title' => t('Tên chứng chỉ'),
    '#required' => TRUE,
  );     
    $form['chungchi']['chungchi_thoigian'] = array(
    '#type'  => 'textfield',
    '#title' => t('Thời gian'),
  );     
      /////// 10. GIẢI THƯỞNG
    $form['giaithuong'] = array(
    '#type'  => 'fieldset',
    '#title' => t('GIẢI THƯỞNG'),
    '#size'  => '100%',
    '#attributes' => array('class' => array('row')),
  );
    $form['giaithuong']['giaithuong_ten'] = array(
    '#type'  => 'textfield',
    '#title' => t('Tên giải thưởng'),
    '#required' => TRUE,
  );     
    $form['giaithuong']['giaithuong_thoigian'] = array(
    '#type'  => 'textfield',
    '#title' => t('Thời gian'),
  );     
  /////// 11. THÔNG TIN THÊM
    $form['them'] = array(
    '#type'  => 'fieldset',
    '#title' => t('THÔNG TIN THÊM'),
    '#size'  => '100%',
    '#attributes' => array('class' => array('row')),
  );
    $form['them']['them_value'] = array(
    '#type'  => 'textfield',
    '#title' => t('Thông tin'),
    '#required' => TRUE,
  );     
           
/////// SUBMIT    
  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Add'),
  );

  return $form;
}

?>