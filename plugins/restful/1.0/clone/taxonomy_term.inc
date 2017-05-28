<?php

/**
 * @file
 * RESTful entity clone.
 */
$plugin = array(
  'label' => t('Clone Taxonomy Term'),
  'resource' => 'clone/taxonomy_term',
  'name' => 'taxonomy_term',
  'entity_type' => 'taxonomy_term',
  'description' => t('Expose taxonomy terms to RESTful API'),
  'class' => 'RecursiveCloneTaxonomyTerm',
);
