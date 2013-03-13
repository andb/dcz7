<?php

function drupalcz_migrate_migrate_api() {
  $api = array(
    'api' => 2,
    'migrations' => array(
      'MigrateDrupalCZUsers' => array('class_name' => 'MigrateDrupalCZUsers'),
      'MigrateDrupalCZCategories' => array('class_name' => 'MigrateDrupalCZCategories'),
      'MigrateDrupalCZBlogposts' => array('class_name' => 'MigrateDrupalCZBlogposts'),
      'MigrateDrupalCZForumTerms' => array('class_name' => 'MigrateDrupalCZForumTerms'),
      'MigrateDrupalCZTags' => array('class_name' => 'MigrateDrupalCZTags'),
    ),
  );

  return $api;
}
