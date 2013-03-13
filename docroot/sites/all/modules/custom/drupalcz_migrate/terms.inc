<?php

class MigrateDrupalCZTerms extends Migration {
  public function __construct() {
    parent::__construct(MigrateGroup::getInstance('examplegroup'));

    $this->description = t('Migrate categories');
    $query = db_select(D6_DB . '.term_data', 't')->fields('t')->condition('vid', $this->vid);
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationTerm($this->voc);

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'tid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'D6 Unique Term ID',
          'alias' => 't',
        )
      ),
      MigrateDestinationTerm::getKeySchema()
    );

    // Create the field mappings
    $this->addFieldMapping('tid', 'tid')->dedupe('taxonomy_term_data', 'tid');
    $this->addFieldMapping('name', 'name');
    $this->addFieldMapping('description', 'description');
  }

  public function prepareRow($row) {}

  public function prepare($entity, stdClass $row) {
    // Over import terms with tid because the row have to exists when the tid is setuped
    db_query('INSERT INTO {taxonomy_term_data} (tid) VALUES (:tid)', array(':tid' => $entity->tid));
    
    $entity->weight = $row->weight;
  }
}
