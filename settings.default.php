<?php

// Settings array
$weather_settings = array(

  'debug' => FALSE,  // Debug mode (prints messages)
  'script' => '/[root]/weather',  // Base directory of the script.
  'content' => '/[root]/weather/content',  // Base directory to store downloaded/generated files in.
  'log' => '/[root]/weather/content/log',  // Directory to store logs in

  // Youtube upload settings.
  'youtube' => array(
    'upload' => TRUE,  // Enable/disable Youtube upload
    'email' => '',  // Youtube login email address
    'password' => '',  // Youtube login password
  ),

  // Array of NOAA GOES image feeds to download.
  'goes_feeds' => array(
    'eaus' => array(
      'url' => 'http://www.ssd.noaa.gov/goes/east/eaus',
      'types' => array('wv'),
    ),
  ),
);
