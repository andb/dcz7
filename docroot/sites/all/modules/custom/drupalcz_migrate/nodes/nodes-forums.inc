<?php

class DrupalCZNodesForums extends DrupalCZNodes {
  public $source = 'forum';
  public $target = 'forum';

  public function prepare($entity, stdClass $row) {
    $entity->field_body['und'][0]['format'] = 'filtered_html';
    $entity->field_body['und'][0]['value_format'] = 'filtered_html';
    
    $this->setTags($entity, $row, 'taxonomy_forums', 1, 'migrate_map_drupalcztermsforums');
    $path = 'public://forums/attachements/' . date('Y', $row->created) . '/' . date('m', $row->created) . '/dcz6/';
    $this->migrateAttachements($entity, $row, 'field_attachment', $path);
  }
  
  public function complete($entity, stdClass $row) {
    $this->updatePathsInBody($entity, $row, 'field_attachment');
  }
}
