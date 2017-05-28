<?php

/**
 * @file Import functions for taxonomy_term.
 */

/**
 * Entity plugin info.
 *
 * @return array
 *   Array containing plugin info needed for different phases of entity import.
 */
function recursive_clone_taxonomy_term_info() {
  $info = array(
    'entity_type' => 'taxonomy_term',
    // Callback that determines the reuse of existing content.
    'reusable_callback' => 'recursive_clone_taxonomy_term_reusable',
    // The revision table for the entity type if needed for import.
    'revision_table' => NULL,
    // The primariy id field for the entity type.
    'id_field' => 'tid',
    // The local entity path if viewable by id. i.e. node/%nid
    'local_entity_path' => 'taxonomy/term',
    // Code for table aliases and encoding cloned log messages where possible.
    'type_code' => 't',
    // Array keys to recast as objects after converting from JSON.
    'object_keys' => array(
      'translations',
    ),
    // Whether or not to recast entity array as object prior to final prepare.
    'objectify_entity' => TRUE,
  );

  return $info;
}

/**
 * Clone or save function for the entity type.
 *
 * @param object &$clone
 *   Object to save.
 */
function recursive_clone_taxonomy_term_clone(&$clone) {
  if (!empty($clone->vid)) {
    taxonomy_term_save($clone);
  }
  else {
    drupal_set_message(t('Vocabulary @vocabulary, does not exist. Term @term not created.',
      array(
        '@vocabulary' => $clone->vocabulary_machine_name,
        '@term' => $clone->name,
      )
    ), 'status');
  }
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
function recursive_clone_taxonomy_term_prepare(&$entity, $original, $origin_base) {
  $account = $GLOBALS['user'];
  $vocabulary = taxonomy_vocabulary_machine_name_load($entity->vocabulary_machine_name);

  if ($vocabulary) {
    $entity->vid = $vocabulary->vid;
  }
  else {
    $entity->vid = NULL;
    drupal_set_message(
      t('Missing target vocabulary: @vocabulary', array('@vocabulary' => $entity->vocabulary_machine_name)),
    'status', FALSE);
  }
  $entity->tid = NULL;
  // Also handle modules that attach a UUID to the taxonomy_term.
  $entity->uuid = NULL;
  $entity->vuuid = NULL;

  $entity->path = NULL;
  $entity->files = array();
  // Update with clone message.
  $entity->description = t('Cloned from !origin', array('!origin' => $origin_base));
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
function recursive_clone_taxonomy_term_reusable($entity, $info, $origin, $local) {
  // Reuse term if local.
  $tid = $local ? $entity['tid'] : FALSE;
  // We always reuse taxonomy terms so we do not check for
  // $entity['clone_data']->override <> RECURSIVE_CLONE_FORCE.
  $vocabulary = taxonomy_vocabulary_machine_name_load($entity['vocabulary_machine_name']);
  if (!$tid && $vocabulary) {
    // Set specified language to check existing terms.
    // Catches vocabularies that don't use language in terms.
    $check_language = (!empty($vocabulary->i18n_mode) && ($vocabulary->i18n_mode == I18N_LANGUAGE_EXTENDED || $vocabulary->i18n_mode == I18N_MODE_LANGUAGE));
    $term_language = $entity['language'] == LANGUAGE_NONE && $check_language ? language_default('language') : $entity['language'];
    // Load any terms by name and set $tid if language matches.
    $existing = taxonomy_get_term_by_name($entity['name'], $vocabulary->machine_name);
    foreach ($existing as $tid => $term) {
      if($term->language == $term_language) {
        break;
      }
      $tid = FALSE;
    }
  }
  if ($tid) {
    // Update the cloned_id to this entity id in recursive_clone.
    db_update('recursive_clone')
      ->fields(array(
        'cloned_id' => $tid,
      ))
      ->condition('import_id', $entity['clone_data']->import_id)
      ->condition('entity_url', $entity['clone_data']->entity_url)
      ->execute();
    $link = l($entity['clone_data']->entity_type . ' ' . $tid, $info['local_entity_path'] . '/' . $tid);
    drupal_set_message(t('!type !entity_url not cloned. !link used instead', array(
      '!type' => $entity['clone_data']->entity_type,
      '!entity_url' => $entity['clone_data']->entity_url,
      '!link' => $link,
    )), 'warning');
    return TRUE;
  }
}
