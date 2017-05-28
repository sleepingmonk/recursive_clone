<?php

/**
 * @file
 * RESTful entity clone.
 */
$plugin = array(
  'label' => t('Clone Node'),
  'resource' => 'clone/node',
  'name' => 'node',
  'entity_type' => 'node',
  'description' => t('Expose nodes to RESTful API'),
  'class' => 'RecursiveCloneNode',
);
