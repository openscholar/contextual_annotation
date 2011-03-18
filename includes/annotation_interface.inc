<?php


interface annotation_interface {
  /**
   * @return string, machine name of the method.
   */
  public function type();
	
  /**
   * @return string, key that defines the "context"
   */
  public function get_key($node, $args);
  
}

