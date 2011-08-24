#!/usr/bin/php
# Download latest image, if it doesn't already exist.
<?php

// Figure out the filename based on the current time
// $filename = '2011112_1345wv.jpg';
$now_z = gmdate('z');  // The current day of the year (0-365)
$now_h = gmdate('H');  // The current hour (0-23)

// NOAA releases the image an hour late
if ($now_h != '0') {
  $now_z++;
  $now_h--;
} else {
  $now_h = 23;
}

// Tack on a leading zero if necessary
if ($now_h < 10) {
 $now_h = '0'.$now_h;
}
$filename = gmdate('Y'.($now_z).'_'.$now_h);

// The noaa images are taken at 15 and 45 past, so...
// If the current minutes is between 0 and 30, set it to 15
// Otherwise set it to 45.
$now_i = gmdate('i');
if ($now_i >= 0 && $now_i < 30) {
  $filename .= '15';
} else {
  $filename .= '45';
}

// Add 'wv.jpg' to finish the filename
$filename .= 'wv.jpg';

$url = 'http://www.ssd.noaa.gov/goes/east/eaus/img/'.$filename;

// Download the file
$file = file_get_contents($url);
$local_path = '/home/mstenta/weather/'.$filename;
if (!file_exists($local_path)) {
  file_put_contents($local_path, $file);
}

?>
