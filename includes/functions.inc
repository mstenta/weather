<?php

// Download the latest image
function download_latest_image($feeds, $path) {
  
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

  // Tack on leading zeros to the day and hour, if necessary (the filename format uses a 3-digit day number, and a 2-digit hour number)
  while (strlen($now_z) < 3) {
    $now_z = '0' . $now_z;
  }
  while (strlen($now_h) < 2) {
    $now_h = '0' . $now_h;
  }

  // Start the filename
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

  // Download each of the files
  foreach ($feeds as $region) {
    foreach ($region as $type => $url) {

      // Generate the image's URL
      $image_url = $url . '/' . $filename . $type . '.jpg';

      // Download the file
      $local_path = $path . '/' . $filename . $type . '.jpg';
      if (!file_exists($local_path)) {
        $file = file_get_contents($image_url);
        if (!empty($file)) {
          file_put_contents($local_path, $file);
        }
      }
    }
  }
}

// Calculate the previous month/year
function previous_month_year() {
  
  // Figure out what the previous month/year was
  $month = date('n') - 1;
  $year = date('Y');
  if ($month == 0) {
    // If the previous month was zero, then it was December of last year.
    $month = 12;
    $year--;
  }
  
  return array(
    'month' => $month,
    'year' => $year,
  );
}

// Calculate the first and last days of a given month/year
function month_days($month, $year) {
  
  // Calculate the first day of the month
  $first_day = 0;
  for ($i=1;$i<$month;$i++) {

    $day_count = date('t', strtotime($i . '/1/' . $year));

    // Add the number of days in this month to the count
    $first_day += $day_count;
  }
  $first_day++;  // The above loop gives us the last day of the month before the previous month, so we add 1 here to put us on the the first day of the previous month.

  // Calculate the last of the previous month
  $last_day = $first_day + date('t', strtotime($month . '/1/' . $year));
  
  return array(
    'first' => $first_day,
    'last' => $last_day,
  );
}
