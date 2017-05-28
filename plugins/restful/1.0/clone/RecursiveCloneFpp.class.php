<?php

/**
 * @file
 * Contains RecursiveCloneNode.
 */

/**
 * RESTful class for Cloning Fieldable Panels Panes.
 */
class RecursiveCloneFpp extends \RestfulEntityBase implements \RestfulEntityInterface {

  /**
   * @return: array $public_fields.
   *  An array containing a Fieldable Panles Pane keyed by fpid.
   */
  public function publicFieldsInfo() {
    $public_fields['fieldable_panels_pane'] = array(
      'callback' => array(
        array($this, 'collectCloneData'),
        array($this->path),
      ),
    );
    return $public_fields;
  }

  /**
   * Load and prepare a fieldable panels pane for export.
   *
   * @param: object $object
   *
   * @param: int $fpid
   *  The fpid to load, passed from the url.
   *
   * @return: array $items
   *  An array of results keyed by fpid.
   */
  protected function collectCloneData($object, $fpid) {

    $items = &drupal_static(__CLASS__ . '::' . __FUNCTION__);

    if (!isset($items[$fpid])) {
      $fpps = entity_load('fieldable_panels_pane', array($fpid));
      $items[$fpid] = $fpps[$fpid];
    }

    return $items;
  }
}
