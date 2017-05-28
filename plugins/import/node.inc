<?php

/**
 * @file Import functions for node.
 */

/**
 * Entity plugin info.
 *
 * @return array
 *   Array containing plugin info needed for different phases of entity import.
 */
function recursive_clone_node_info() {
  $info = array(
    'entity_type' => 'node',
    // Callback that determines the reuse of existing content.
    'reusable_callback' => 'recursive_clone_node_reusable',
    // The revision table for the entity type if needed for import.
    'revision_table' => 'node_revision',
    // The primariy id field for the entity type.
    'id_field' => 'nid',
    // The local entity path if viewable by id. i.e. node/%nid
    'local_entity_path' => 'node',
    // Code for table aliases and encoding cloned log messages where possible.
    'type_code' => 'n',
    // Array keys to recast as objects after converting from JSON.
    'object_keys' => array(
      'translations',
      'page_manager',
      'display',
    ),
    // Whether or not to recast entity array as object prior to final prepare.
    'objectify_entity' => TRUE,
  );

  return $info;
}

/**
 * Clone or save function for the entity type.
 *
 * @param mixed &$clone
 *   Object or array to save.
 */
function recursive_clone_node_clone(&$clone) {
  node_save($clone);
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
function recursive_clone_node_prepare(&$entity, $original, $origin_base) {
  $account = $GLOBALS['user'];

  $entity->nid = NULL;
  $entity->vid = NULL;
  $entity->log = "clone:$origin_base:n:" . $original['nid'];
  // Do not publish the node/revision.
  $entity->status = 0;
  $entity->is_pending = TRUE;
  // Also handle modules that attach a UUID to the node.
  $entity->uuid = NULL;
  $entity->vuuid = NULL;

  // Anyonmymous users don't have a name.
  // @see: drupal_anonymous_user()
  $entity->name = isset($account->name) ? $account->name : NULL;
  $entity->uid = $account->uid;

  $entity->created = NULL;
  $entity->menu = NULL;
  if (isset($entity->book['mlid'])) {
    $entity->book['mlid'] = NULL;
    $entity->book['has_children'] = 0;
  }
  $entity->path = array('pathauto' => FALSE);
  $entity->files = array();
  // Update title with clone message.
  $entity->title = t('Clone of !title', array('!title' => $entity->title));
}

/**
 * Check for reusability of entity and update item's cloned_id if reusable.
 *
 * Check if there's a reusable entity. Update cloned_id if exists and set to
 * reuse. Set a drupal message with info about the reused entity and a link.
 *
 * @param array $entity
 *   The entity for review.
 *
 * @param array $info
 *   The entity type plugin info.
 *
 * @param string $origin
 *   The originating site url.
 *
 * @return bool
 *   TRUE if an existing entity should be reused. Else FALSE.
 */
function recursive_clone_node_reusable($entity, $info, $origin) {
  // Only reuse if override is not set to force clone.
  if ($entity['clone_data']->override <> RECURSIVE_CLONE_FORCE) {
    $q = db_select($info['revision_table'], $info['type_code'])
      ->fields($info['type_code'], array($info['id_field']))
      ->condition('log', "%clone:$origin:" . $info['type_code'] . ":" . $entity['clone_data']->entity_id . "%", 'LIKE')
      ->orderBy('timestamp', 'DESC')
      ->execute();
    while ($id = $q->fetchField()) {
      // Override: reuse. (ASK option has been removed.)
      switch ($entity['clone_data']->override) {
        case RECURSIVE_CLONE_REUSE:
          // Update the cloned_id to this entity id in recursive_clone.
          db_update('recursive_clone')
            ->fields(array(
              'cloned_id' => $id,
            ))
            ->condition('import_id', $entity['clone_data']->import_id)
            ->condition('entity_url', $entity['clone_data']->entity_url)
            ->execute();
          $link = l($entity['clone_data']->entity_type . ' ' . $id, $info['local_entity_path'] . '/' . $id);
          drupal_set_message(t('!type !entity_url not cloned. !link used instead', array(
            '!type' => $entity['clone_data']->entity_type,
            '!entity_url' => $entity['clone_data']->entity_url,
            '!link' => $link,
          )));
          // Remove the children of this item if duplicate reused.  It will
          // have it's own existing children.
          recursive_clone_node_remove_orphans($entity, $info);
          return TRUE;
      }
    }
  }
}

function recursive_clone_node_remove_orphans($entity, $info) {
  // Entity types that are ok to delete.
  $orphan_types = array('paragraphs_item');
  // Get cloned_id for all items who's parent is this entity's original id.
  $q = db_select('recursive_clone', 'c')
    ->fields('c', array('entity_type', 'cloned_id'))
    ->condition('parent_id', $entity[$info['id_field']])
    ->condition('parent_type', $info['entity_type'])
    ->execute();

  while ($orphan = $q->fetchAssoc()) {
    // Delete the cloned id.
    if (in_array($orphan['entity_type'], $orphan_types) && entity_delete($orphan['entity_type'], $orphan['cloned_id']) !== FALSE) {
      drupal_set_message(t('Entity: @orphan_type: @orphan_id has been orphaned due to the reuse of @parent_type: @parent_id and has been deleted.',
        array(
          '@parent_id' => $entity[$info['id_field']],
          '@parent_type' => $info['entity_type'],
          '@orphan_type' => $orphan['entity_type'],
          '@orphan_id' => $orphan['cloned_id'],
        )
      ), 'warning');
    }
    else {
      drupal_set_message(t('Entity: @orphan_type: @orphan_id has been orphaned due to the reuse of @parent_type: @parent_id but could NOT be deleted due to it\'s type.',
        array(
          '@parent_id' => $entity[$info['id_field']],
          '@parent_type' => $info['entity_type'],
          '@orphan_type' => $orphan['entity_type'],
          '@orphan_id' => $orphan['cloned_id'],
        )
      ), 'error');
    }
  }
}
