<?php

class MigrateDrupalCZBlogposts extends Migration {
  public function __construct() {
    parent::__construct(MigrateGroup::getInstance('examplegroup'));

    $this->description = t('Migrate categories');
    $query = db_select(D6_DB . '.node', 'n');
    $query->join(D6_DB . '.node_revisions', 'r', 'r.nid = n.nid');
    $query->fields('n')
      ->fields('r', array('body'))
      ->condition('n.type', 'blog');
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationNode('blog');
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'nid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'D6 Unique Node ID',
          'alias' => 'n',
        )
      ),
      MigrateDestinationTerm::getKeySchema()
    );

    // Create the field mappings
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('uid', 'uid');
    $this->addFieldMapping('field_body', 'body');
    $this->addFieldMapping('status', 'status');
    $this->addFieldMapping('created', 'created');
    $this->addFieldMapping('changed', 'changed');
    $this->addFieldMapping('promote', 'promote');
    $this->addFieldMapping('description', 'description');
  }

  public function prepare($entity, stdClass $row) {
    // SET CATEGORY
    $query = db_select(D6_DB . '.term_node', 't')
      ->fields('t')
      ->condition('t.nid', $row->nid);
    $result = $query->execute();
    $tids = array();
    while ($row2 = $result->fetch()) {
      $tids[] = $row2->tid;
    }

    $entity->field_tax_category = array(LANGUAGE_NONE => array());
    foreach ($tids as $tid) {
      $entity->field_tax_category[LANGUAGE_NONE][] = array('tid' => $tid); 
    } 
    
return;
// @TODO

    // ATACHEMENTS
    $query = db_select(D6_DB . '.upload', 'u');
    $query->join(D6_DB . '.files', 'f', 'f.fid = u.fid');
    $query->fields('u')
      ->fields('f')
      ->condition('u.nid', $row->nid);
    $result = $query->execute();

    $files = array();
    while ($row2 = $result->fetch()) {
      $files[] = $row2;
    }

    if (FALSE and $files) {
      $fields = field_info_instances('node', 'blog');
      $path = 'public://' . $fields['field_attachment']['settings']['file_directory'];
      $path = token_replace($path);
      file_prepare_directory($path, FILE_CREATE_DIRECTORY);

      $entity->field_attachment = array();
      $entity->field_attachment[LANGUAGE_NONE] = array();
      foreach ($files as $file) {
        $filepath = $file->filepath;
        $filename = drupal_basename($filepath);
        $handle = fopen($filepath, "r");
        $contents = fread($handle, filesize($filepath));
        fclose($handle);

        $file = file_save_data($contents, "$path/$filename", FILE_EXISTS_REPLACE);
        if ($file) {
          $entity->field_attachment[LANGUAGE_NONE][] = (array) $file;
        }
        else {
          drupal_set_message(t('File was not uploaded.'), 'error');
        }
      }
    }
  }
}
