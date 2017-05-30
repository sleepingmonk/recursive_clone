<?php

/**
 * @file
 * Contains RecursiveCloneParagraphs.
 */

/**
 * RESTful class for Cloning Paragraphs Items.
 */
class RecursiveCloneParagraphsItem extends \RestfulEntityBase implements \RestfulEntityInterface {

  /**
   * @return: array $public_fields.
   *  An array containing a Paragraphs Item keyed by item id.
   */
  public function publicFieldsInfo() {
    $public_fields['paragraphs_item'] = array(
      'callback' => array(
        array($this, 'collectCloneData'),
        array($this->path),
      ),
    );
    return $public_fields;
  }

  /**
   * Load and prepare a paragraphs_item for export.
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
      $paragraphs = paragraphs_item_load($item_id);
      $items[$item_id] = $paragraphs;
    }

    return $items;
  }
}
