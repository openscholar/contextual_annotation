<?php


class contextual_annotation_taxonomy implements annotation_interface {
	private static $a_cached_children;
	
  /**
   * @return string, machine name of the method.
   */
  public function type(){
  	return "taxonomy";
  }
  
  /**
   * @return string, key for a term or vocab
   */
  public function get_key($node, $arg_type){
  	
  	if(is_string($arg_type) && strlen($arg_type)) $arg_type = array($arg_type);
  	
  	$taxonomy_types = (is_array($arg_type) && count($arg_type))? $arg_type : array('tid','vid');
  	
  	foreach ($taxonomy_types as $taxonomy_type){
  	
	  	$func = "get_key_{$taxonomy_type}";
	  	if(method_exists($this,$func)){
	      $return = $this->$func($node);
	      if($return) return $return;
	  	}
  	}
  	return false;
  }
  
  /**
   * @return string, key for a vocab
   */
  private function get_key_vid($node){
    
  	if(!is_array($node->taxonomy)) return false; //Is this node part of a taxonomy?
  	
  	$router_item = menu_get_item();
    if(strpos($router_item['path'], 'taxonomy/term') !== 0) return false; //On a taxonomy term page?

    $o_term = taxonomy_get_term(current($router_item['page_arguments']));
    
    if(is_object($o_term) && $o_term->vid){
    	foreach ($node->taxonomy as $node_term) {
    		if($node_term->vid == $o_term->vid){
    			return "vid-".$o_term->vid;
    		} //If this node is in the vocabulary return the key for this vocab
    	}
    }
  }
  
  /**
   * @return string, key for a term
   */
  private function get_key_tid($node){
    
    if(!is_array($node->taxonomy)) return false; //Is this node part of a taxonomy?
    
    $router_item = menu_get_item();
    if(strpos($router_item['path'], 'taxonomy/term') !== 0) return false; //On a taxonomy term page?

    $o_term = taxonomy_get_term(current($router_item['page_arguments']));
    $a_children = $this->get_term_children($o_term->vid, $o_term->tid);
    
    if(!is_array($a_children)) $a_children = array();
    
    foreach ($node->taxonomy as $node_term) {
      if($node_term->tid == $o_term->tid || in_array($node_term->tid, $a_children)){
        return "tid-".$node_term->tid;
      } //If this node is in the vocabulary return the key for this term
    }
    
  }
  
  private function get_term_children($vid,$tid){
  	if(!isset(self::$a_cached_children[$vid."_".$tid])){
  		 $a_tree = taxonomy_get_tree($vid, $tid);
  		 $a_tids = array();
  		 if(is_array($a_tree)){
  		 	foreach ($a_tree as $term) $a_tids[] = $term->tid;
  		 }
  		 
  		 self::$a_cached_children[$vid."_".$tid] = $a_tids;
  	}
  	return self::$a_cached_children[$vid."_".$tid];
  }
}