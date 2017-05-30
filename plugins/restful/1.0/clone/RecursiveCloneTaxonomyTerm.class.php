<?php

/**
 * @file
 * Contains RecursiveCloneTaxonomyTerm
 */

/**
 * RESTful class for Cloning Taxonomy Terms.
 */
class RecursiveCloneTaxonomyTerm extends \RestfulEntityBase implements \RestfulEntityInterface {

  /**
   * @return: array $public_fields.
   *  An array containing a term keyed by tid.
   */
  public function publicFieldsInfo() {
    $public_fields['taxonomy_term'] = array(
      'callback' => array(
        array($this, 'collectCloneData'),
        array($this->path),
      ),
    );
    return $public_fields;
  }

  /**
   * Load and prepare a taxonomy_term for export.
   *
   * @param: object $object
   *
   * @param: int $item_id
   *  The item to load, passed from the url.
   *
   * @return: array $items
   *  An array of results keyed by item_id.
   */
  protected function collectCloneData($object, $item_id) {

    $items = &drupal_static(__CLASS__ . '::' . __FUNCTION__);

    if (!isset($items[$item_id])) {
      $taxonomy_term = taxonomy_term_load($item_id);
      $items[$item_id] = $taxonomy_term;
    }

    return $items;
  }
}
