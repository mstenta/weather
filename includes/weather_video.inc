<?php

/**
 * Weather video class
 */
class weather_video extends weather {

  // Previous month timestamp (calculated in this::render()).
  public $previous_month = 0;

  // The video's path.
  public $path = NULL;

  // The video's filename.
  public $filename = NULL;

  /**
   * Constructor.
   */
  public function __construct($settings, $timestamp=NULL) {
    parent::__construct($settings, $timestamp);
  }

  public function render() {

    // Figure out what the previous month/year was
    $last_month = gmdate('n', $this->timestamp) - 1;
    $last_year = gmdate('Y', $this->timestamp);

    // Store the previous month as a timestamp on the video object for other functions to use (like this::upload()).
    $this->previous_month = strtotime($last_month . '/1/' . $last_year);

    // If the previous month was zero, then it was December of the year before
    if ($last_month == 0) {
      $last_month = 12;
      $last_year--;
    }

    // Tack a 0 to the month if it's less than 10
    if ($last_month < 10) {
      $last_month = '0' . $last_month;
    }

    // Generate the video path and filename
    /**
     * @todo
     * Currently the region (eaus) and type (wv) are hard coded. In the future, this script should support generating multiple videos for each region and type.
     * To do this, we'll need to change the way that image files are stored, so that they are in region and type subfolders. Either that, or change the way the 
     * code below works so that it can isolate only the appropriate files in the month directory and copy them to the temp directory.
     */
    $this->path = $this->settings['content'] . '/' . $last_year . '/' . $last_month;
    $this->filename = 'goes_eaus_wv_' . $last_year . $last_month . '.mp4';

    // If a video hasn't already been generated...
    if (!file_exists($this->path . '/' . $this->filename)) {

      // Remember the current working directory
      $cwd = getcwd();

      // Change the working directory
      chdir($this->path);

      // Make a temporary directory
      mkdir($this->path . '/temp');

      // Copy images to temporary folder with numbered filenames
      exec('x=1; for i in img/*jpg; do counter=$(printf %05d $x); cp "$i" temp/"$counter".jpg; x=$(($x+1)); done');

      // Use ffmpeg to generate the video
      exec('ffmpeg -r 20 -qscale 10 -i temp/%05d.jpg ' . $this->filename);

      // Delete the temporary directory
      exec('rm -r temp');

      // Change back to the original working directory
      chdir($cwd);
    }
  }

  public function upload() {

    // Get the month and year.
    $month = date('F', $this->previous_month);
    $year = date('Y', $this->previous_month);

    $command = 'python lib/youtube_upload.py --email=' . $this->settings['youtube']['email'] . ' --password=' . $this->settings['youtube']['password'] . ' --title="' . $month . ' ' . $year . ' Water Vapor - Eastern US - NOAA GOES" --description="NOAA geostationary satellite eastern US water vapor - ' . $month . ' ' . $year . '" --category=Education --keywords="NOAA, GOES, Water Vapor" ' . $this->path . '/' . $this->filename;

    // Log the command so that it can be manually executed later if this doesn't work
    file_put_contents($this->settings['log'] . '/' . $year . $month . '.log', $command);

    // Upload it to YouTube
    exec($command);
  }
}
