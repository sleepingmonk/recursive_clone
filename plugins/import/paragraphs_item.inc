<?php

/**
 * @file Import functions for paragraphs.
 */

/**
 * Entity plugin info.
 *
 * @return array
 *   Array containing plugin info needed for different phases of entity import.
 */
function recursive_clone_paragraphs_item_info() {
  $info = array(
    'entity_type' => 'paragraphs_item',
    // Callback that determines the reuse of existing content.
    'reusable_callback' => FALSE,
    // The revision table for the entity type if needed for import.
    'revision_table' => 'paragraphs_item_revision',
    // The primariy id field for the entity type.
    'id_field' => 'item_id',
    // The local entity path if viewable by id. i.e. node/%nid
    'local_entity_path' => 'api/v1.0/clone/paragraphs_item',
    // Code for table aliases and encoding cloned log messages where possible.
    'type_code' => 'p',
    // Array keys to recast as objects after converting from JSON.
    'object_keys' => array(
      'translations',
    ),
    // Whether or not to recast entity array as object prior to final prepare.
    'objectify_entity' => FALSE,
  );

  return $info;
}

/**
 * Clone or save function for the entity type.
 *
 * @param mixed &$clone
 *   Object or array to save.
 */
function recursive_clone_paragraphs_item_clone(&$clone) {
  $clone->save($clone);
}

/**
 * Function to prepare/clear entity variables so we can save as new.
 *
 * @param mixed &$entity
 *   The entity to prepare for save.
 *
 * @param mixed $original
 *   The original entity.
 *
 * @param string $origin_base
 *   The base url from the entity origin.
 */
function recursive_clone_paragraphs_item_prepare(&$entity, $original, $origin_base) {
  $entity['item_id'] = NULL;
  $entity['revision_id'] = NULL;
  $entity['log'] = "clone:$origin_base:p:" . $original['item_id'];
  // Also handle modules that attach a UUID to the node.
  $entity['uuid'] = NULL;
  $entity['vuuid'] = NULL;
  // Clear field_promo_pricing. Should not be imported and breaks import
  // with referenced offer id.
  if (array_key_exists('field_promo_price', $entity)) {
    $entity['field_promo_price'] = array();
  }
  $entity = new ParagraphsItemEntity($entity);
  $entity->is_new = TRUE;
}
