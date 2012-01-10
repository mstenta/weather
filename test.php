#!/usr/bin/php
<?php

// Load the settings.
require_once 'settings.php';

// Load the weather classes.
require_once 'includes/weather.inc';

// Set the timezone to UTC.
date_default_timezone_set('UTC');

// Test
$times = array(
  time(),
  // strtotime('January 2nd, 2013 12:50am'),  // Test new day
  // strtotime('January 1st, 2013 12:10am'),  // Test the new year (previous year was leap year)
  // strtotime('January 1st, 2014 12:10am'),  // Test the new year (previous year was not leap year)
);
foreach ($times as $time) {
  
  // Test snapshot
  $weather = new weather_snapshot($weather_settings, $time);
  print_r($weather->download());
  print("\n");
}

// Test video
$time = strtotime('February 1st, 2012');

// Create a new video object.
$video = new weather_video($weather_settings, $time);

// Render the video.
$video->render();

// Upload the video to YouTube.
if ($weather_settings['youtube']['upload']) {
  $video->upload();
}

?>
