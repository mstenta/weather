<?php

/**
 * Weather snapshot class
 */

class weather_snapshot extends weather {

  // The image URLs.
  public $images = array();

  /**
   * Constructor.
   */
  public function __construct($settings, $timestamp=NULL) {
    parent::__construct($settings, $timestamp);

    // Generate the urls from the timestamp.
    $this->generate_images();
  }

  /**
   * Download the images.
   */
  public function download() {

    // If there are filenames...
    if (!empty($this->images)) {
      foreach ($this->images as $year => $months) {
        if (!empty($months)) {
          foreach ($months as $month => $images) {
            if (!empty($images)) {

              // Generate directory if it doesn't exist.
              $destination = $this->settings['content'] . '/' . $year . '/' . $month;
              if (!is_dir($destination . '/img')) {
                mkdir($destination . '/img', 0777, TRUE);
              }

              // Download each of the images.
              foreach ($images as $image) {

                // Build a local path for the download file.
                $local_path = $destination . '/' . $image['filename'];

                // Check to see if the local path already exists.
                if (!file_exists($local_path)) {

                  // Get the contents of the remote file.
                  $remote_file = file_get_contents($image['url'] . '/' . $image['filename']);

                  // If there are contents, put them into the local file.
                  if (!empty($remote_file)) {
                    $success = file_put_contents($local_path, $remote_file);
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  
  /**
   * Generates an array of images to be downloaded.
   *
   * @return
   *   Returns an array of images.
   */
  private function generate_images() {

    // Generate date information from the stored timestamp.
    $date_info = $this->format_date($this->timestamp);

    // If the image doesn't have any filenames, generate them.
    if (empty($this->images)) {

      // If there are GOES feeds...
      if (!empty($this->settings['goes_feeds'])) {

        // Loop through them...
        foreach ($this->settings['goes_feeds'] as $region) {

          // If there is a region...
          if (!empty($region)) {

            // Get the base URL.
            $url = $region['url'];

            // Loop through the types that should be downloaded.
            foreach ($region['types'] as $type) {

              // Fetch the text file containing the filenames from the NOAA server.
              $txtfile = file_get_contents($url . '/txtfiles/' . $type . '_names.txt');

              // If the text file was loaded successfully, parse it.
              if (!empty($txtfile)) {

                // Iterate over each line.
                foreach (preg_split('/((\r?\n)|(\r\n?))/', $txtfile) as $line) {

                  // Find the image name, ie:
                  // img/2012268_1745wv.jpg
                  $matches = array();
                  preg_match('/^(img\/[0-9_]*' . $type . '\.jpg)\s*/', $line, $matches);

                  // If an image name was found, add it to the list of images.
                  if (!empty($matches[1])) {

                    // Set the filename.
                    $filename = $matches[1];

                    // Add an image element to the array of images for download, along with the year and month folders to sort it into.
                    $this->images[$date_info['year']][$date_info['month']][] = array(
                      'url' => $url,
                      'filename' => $filename,
                    );
                  }
                }
              }

              // If the text file wasn't loaded successfully, guess at the filename based on the date.
              else {

                // Generate a filename string using the date.
                $filename = 'img/' . $date_info['string'] . $type . '.jpg';

                // Add an image element to the array of images for download, along with the year and month folders to sort it into.
                $this->images[$date_info['year']][$date_info['month']][] = array(
                  'url' => $url,
                  'filename' => $filename,
                );
              }
            }
          }
        }
      }
    }

    // Return the filename
    return $this->images;
  }

  /**
   * Formats a date based on a timestamp for use in a filename.
   *
   * Filename format example: 2011112_1330wv.jpg (YYYYddd_HHmm[type].jpg)
   *
   * @param $timestamp
   *   The timestamp to use.
   *
   * @return
   *   Returns a string based on the $timestamp paramater that can be used in a filename.
   */
  private function format_date($timestamp) {

    // Get the date pieces (in Greenwich Mean Time)
    $year = gmdate('Y', $timestamp);  // The year
    $month = gmdate('m', $timestamp);  // The month
    $day = gmdate('z', $timestamp);  // The day of the year (0-365)
    $hour = gmdate('H', $timestamp);  // The hour (0-23)
    $minute = gmdate('i', $timestamp);  // The minute (0-59)

    // NOAA uses day values between 1-365 (366 on leap year), so adjust our PHP day by adding 1 to it.
    $day++;

    // NOAA releases the image an hour late, so we need to look for the image that was released BEFORE this one (by subtracting an hour).
    // If we are already in the first hour of the day (0), then we need to set it to the last hour of the day (23).
    if ($hour != '0') {
      $hour--;  // Set the hour to last hour.
    } else {
      
      $hour = '23';  // Set the hour to the last hour of yesterday.
      
      // If we are on the cusp of a new year, we need to adjust for that as well.
      if ($day == '1') {
        $year--;  // Set the year to last year
        $month = '12';  // Set the month to December
        $leapyear = gmdate('L', strtotime('January ' . $year));  // Check to see if last year was a leap year (the use of "January" in strtotime() is arbitrary, but a month is required for it to work.)
        $day = 365 + $leapyear;  // Set the day to the last day of last year (+1 if it was a leap year)
      } else {
        $day--;  // Set the day to yesterday.
      }
    }

    // Tack on leading zeros to the day and hour, if necessary (the filename format uses a 3-digit day number, and a 2-digit hour number)
    while (strlen($day) < 3) {
      $day = '0' . $day;
    }
    while (strlen($hour) < 2) {
      $hour = '0' . $hour;
    }

    // The NOAA images are taken at 15 and 45 past, so...
    // If the minute is between 0 and 30, set it to 15
    // Otherwise set it to 45.
    if ($minute >= 0 && $minute < 30) {
      $minute = '00';
    } else {
      $minute = '30';
    }

    // Return the formatted string in an array with the year and month
    return array(
      'year' => $year,
      'month' => $month,
      'string' => $year . $day . '_' . $hour . $minute,
    );
  }
}
