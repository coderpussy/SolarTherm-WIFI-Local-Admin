<?php
// Get device
$device = '';
if(!empty($_GET['device'])){
    $device = $_GET['device'];

    $localeCode = 'de_DE.UTF-8';
    setlocale(LC_TIME,$localeCode);
    
    echo $device.".bin " . strftime("%a., %d. %b. %Y %T",filemtime("./solartherm-ota/bin/".ucfirst($device).".bin"));
}