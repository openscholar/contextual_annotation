<?php


class contextual_annotation_context implements annotation_interface {
  /**
   * @return string, machine name of the method.
   */
  public function type(){
    return "context";
  }
  
  /**
   * @return string, key for a term or vocab
   */
  public function get_key($node, $args){
    
  	if(count($args) > 1 && ($value = context_get($args[0],$args[1]))){
      return "{$args[0]}-{$args[1]}-".$value;
  	}
  	return false;
  }
}