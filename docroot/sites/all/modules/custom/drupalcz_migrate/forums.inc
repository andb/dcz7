<?php

class MigrateDrupalCZForumTerms extends MigrateDrupalCZTerms {
  public $voc = 'forums';
  public $vid = 1;
  
  public function prepare($entity, stdClass $row) {
    parent::prepare($entity, $row);
    
    $parent = db_query('select t.parent from ' . D6_DB . '.term_hierarchy t where tid = :tid', 
        array(':tid' => $row->tid)
      )->fetch();    
 
    $entity->parent = array($parent->parent => $parent->parent);
  }
}