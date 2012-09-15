<?php

/**
 * Base weather class.
 */
class weather {

  // The snapshot timestamp.
  public $timestamp = NULL;

  // The snapshot settings.
  public $settings = array();

  /**
   * Constructor.
   */
  public function __construct($settings, $timestamp=NULL) {

    // Copy the settings array
    if (!empty($settings)) {
      $this->settings = $settings;
    }

    // Set the timestamp. If one isn't provided, use the current time.
    if (is_null($timestamp)) {
      $timestamp = time();
    }
    $this->timestamp = $timestamp;
  }
}