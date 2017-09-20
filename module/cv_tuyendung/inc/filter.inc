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

function timkiem_pages($form, &$form_state) {

  $form = array();


  $form['thongtin'] = array(
    '#type' => 'fieldset',
    '#title' => t('xxxsads'),
    '#collapsible' => TRUE,
    '#prefix' => '<div id="page-search" class="container">',
    '#suffix' => '</div>',
  );

  $form['thongtin']['vt'] = array(
    '#type' => 'textfield',
    '#title' => t(' Vị trí công việc'),
    '#required' => TRUE,
    '#default_value' => null,
    '#id' => 'keyword'
  );

  $form['thongtin']['dd'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Địa điểm'),
    '#required' => FALSE,
    '#options' => array(1,2,3),
    '#size' => 5,
    '#weight' => 0,
    //'#default_value' => 1,
    '#id' => 'location-selector',
  );

  $form['thongtin']['or_select'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t(' Có một trong những từ khoá sau '),
    '#required' => FALSE,
    '#options' => array(4,5,6,'or'),
    '#size' => 5,
    '#weight' => 0,
    //'#default_value' => 1,
    '#id' => 'or-keyword-selector',
  );
  $form['thongtin']['and_select'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Bắt buộc có các từ khoá sau'),
    '#required' => FALSE,
    '#options' => array(11,22,33,'and'),
    '#size' => 5,
    '#weight' => 0,
    //'#default_value' => 1,
    '#id' => 'and-keyword-selector',
  );

  $form['thongtin']['nor_select'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Không có những từ khoá sau'),
    '#required' => FALSE,
    '#options' => array(4,5,6,'not'),
    '#size' => 5,
    '#weight' => 0,
    //'#default_value' => 1,
    '#id' => 'not-keyword-selector',
  );
  $form['thongtin']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Cập nhật'),
  );
 // $form['thongtin']['#attached']['js'] = array(drupal_get_path('module', 'cv_tuyendung') . 'timkiem/filter.js');


  $form['thongtin']['#attached'] = array(
    'js' => array(
      array('data' => drupal_get_path('module', 'cv_tuyendung') . '/js/filter.js'),
      array('data' => drupal_get_path('module', 'test_module') . '/js/vue.js'),
    ),
  );

  return $form;
}
function timkiem_pages_submit($form, &$form_state) {

}