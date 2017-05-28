<?php

/**
 * @file
 * Contains RecursiveCloneNode.
 */

/**
 * RESTful class for Cloning Nodes.
 */
class RecursiveCloneNode extends \RestfulEntityBase implements \RestfulEntityInterface {

  /**
   * @return: array $public_fields.
   *  An array containing a Node keyed by nid.
   */
  public function publicFieldsInfo() {
    $public_fields['node'] = array(
      'callback' => array(
        array($this, 'collectCloneData'),
        array($this->path),
      ),
    );
    return $public_fields;
  }

  /**
   * Load and prepare a node for export.
   *
   * @param: object $object
   *
   * @param: int $nid
   *  The fpid to load, passed from the url.
   *
   * @return: array $items
   *  An array of results keyed by fpid.
   */
  protected function collectCloneData($object, $nid) {

    $items = &drupal_static(__CLASS__ . '::' . __FUNCTION__);

    if (!isset($items[$nid])) {
      $node = node_load($nid);
      $items[$nid] = $node;
    }

    return $items;
  }
}
