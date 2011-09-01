#!/usr/bin/php
<?php

// Settings
$debug = TRUE;  // Debug mode (prints messages)
$file_dir = '/home/mstenta/weather';  // Base directory to store downloaded/generated files in.
$goes_feeds = array(  // Array of GOES feeds to download
  'eaus' => array(
    'wv' => 'http://www.ssd.noaa.gov/goes/east/eaus/img',
  ),
);
$email = 'weather@mstenta.net';
$password = '72411EOQUZYtv11';

// Set the timezone to UTC
date_default_timezone_set('UTC');

// Load some helpful functions
require_once 'includes/functions.inc';

// Generate directories if necessary
$images_dir = '/' . date('Y') . '/' . date('m') . '/images';
$path = $file_dir . $images_dir;
$dir_exists = is_dir($path);
if (!$dir_exists) {
  $dir_exists = mkdir($path, 0777, TRUE);
}
if ($dir_exists) {
  
  // Download the latest image
  download_latest_image($goes_feeds, $path);
  
  // If it's the first day of the month, generate videos
  if (date('j') == '1') {

    foreach ($goes_feeds as $region => $types) {
      foreach ($types as $type => $url) {

        // Get the previous month and year
        $previous = previous_month_year();

        // Tack a 0 to the month if it's less than 10
        if ($previous['month'] < 10) {
          $month = '0' . $previous['month'];
        } else {
          $month = $previous['month'];
        }

        // Generate the video filename
        $path = $file_dir . '/' . $previous['year'] . '/' . $month;
        $filename = 'goes_' . $region . '_' . $type . '_' . $previous['year'] . $month . '.mp4';

        // If a video hasn't already been generated...
        if (!file_exists($path . '/' . $filename)) {
          
          // Remember the current working directory
          $cwd = getcwd();
          
          // Change the working directory
          chdir($path);
          
          // Make a temporary directory
          mkdir($path . '/temp');
          
          // Generate the video
          exec('x=1; for i in images/*jpg; do counter=$(printf %05d $x); cp "$i" temp/"$counter".jpg; x=$(($x+1)); done');
          exec('ffmpeg -r 20 -qscale 10 -i temp/%05d.jpg ' . $filename);
          
          // Delete the temporary directory
          exec('rm -r temp');
          
          // Change the working directory back
          chdir($cwd);
          
          // Get the month name
          $month_name = date(strtotime($previous['month'] . '/1/' . $previous['year']));
          
          // Upload it to YouTube
          exec('python includes/youtube_upload.py --email=' . $email . ' --password=' . $password . ' --title="' . $month_name . ' ' . $previous['year'] . ' Water Vapor - Eastern US - NOAA GOES" --description="NOAA geostationary satellite eastern US water vapor - ' . $month_name . '/' . $previous['year'] . '" --category=Education --keywords="NOAA, GOES, Water Vapor" ' . $path . '/' . $filename);
        }
      }
    }
  }
}

?>
