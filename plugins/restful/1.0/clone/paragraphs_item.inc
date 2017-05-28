<?php

/**
 * @file
 * RESTful entity clone.
 */
$plugin = array(
  'label' => t('Clone Paragraphs'),
  'resource' => 'clone/paragraphs_item',
  'name' => 'paragraphs_item',
  'entity_type' => 'paragraphs_item',
  'description' => t('Expose paragraphs to RESTful API'),
  'class' => 'RecursiveCloneParagraphsItem',
);
