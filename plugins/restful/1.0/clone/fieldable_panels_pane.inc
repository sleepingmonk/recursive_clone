<?php

/**
 * @file
 * RESTful entity clone.
 */
$plugin = array(
  'label' => t('Clone Fieldabe Panels Panes'),
  'resource' => 'clone/fieldable_panels_pane',
  'name' => 'fieldable_panels_pane',
  'entity_type' => 'fieldable_panels_pane',
  'description' => t('Expose Fieldable Panels Panes to RESTful API'),
  'class' => 'RecursiveCloneFpp',
);
