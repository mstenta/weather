#!/usr/bin/php
<?php

// Load the settings.
require_once 'settings.php';

// Load the weather classes.
require_once 'includes/weather.inc';
require_once 'includes/weather_snapshot.inc';
require_once 'includes/weather_video.inc';

// Set the timezone to UTC.
date_default_timezone_set('UTC');

// Get the current time.
$time = time();

// Generate a new weather snapshot
$weather = new weather_snapshot($weather_settings, $time);

// Download the image
$weather->download();

// If it's the first day of the month, generate a video.
if (date('j') == '1') {

  // Create a new video object.
  $video = new weather_video($weather_settings, $time);

  // Render the video.
  $video->render();

  // Upload the video to YouTube.
  if ($weather_settings['youtube']['upload']) {
    $video->upload();
  }
}

?>