<?php
function trinhchinhsua_cv() {
    $a =  file_get_contents('http://drupal.uit/sites/all/modules/descv/descv_test.php');
    return '<div id="edit-cv" next-id="">'.$a.'</div>';
}
?>