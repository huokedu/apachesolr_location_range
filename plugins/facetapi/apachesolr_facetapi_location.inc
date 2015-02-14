<?php

/**
 * @file
 * Widget that renders form with  autocomplete for Google places and distance.
 *
 * @author Milka Petkova <milka@milka.petkova.88@gmail.com>
 */
class ApacheSolrFacetapiWidgetLocation extends FacetapiWidget {

    /**
     * Override execute().
     */
    public function execute() {
        $field = $this->build['#facet']['field alias'];
        $this->build[$this->facet['field alias']] = drupal_get_form('apachesolr_location_range_widget_location_form', $field);
    }

}

/**
 * Form callback, which generate facet form with autocomplete field.
 * @param type $form
 * @param type $form_state
 * @param type $field_name
 * @return string
 */
function apachesolr_location_range_widget_location_form($form, &$form_state, $field_name) {
    $getmap = getlocations_search_getmap('page');
    $map = $getmap['map'];
    $mapid = $getmap['mapid'];
    $default_latitude = '';
    $default_longitude = '';
    $default_distance = '';
    //If Location form is already submitted or there is another filters setted by facets.
    if (isset($_GET['f'])) {
        $facet_filters = $_GET['f'];
        //Get filters name.
        foreach (array_values($facet_filters) as $filter) {
            //Checking for filter name.
            if (strpos($filter, $field_name . ':', 0) !== FALSE) {
                $array_index = array_keys($facet_filters, $filter);
                $index = reset($array_index);
                list($default_latitude, $default_longitude, $default_distance) = explode(',', str_replace($field_name . ':', '', $facet_filters[$index]));
            }
        }
    }

    //Generate location range form.
    $form['mapid'] = array('#type' => 'value', '#value' => $mapid);
    $form['getlocations_search_' . $mapid] = array(
        '#type' => 'textfield',
        '#title' => t('Search Address'),
        '#maxlength' => 255,
        '#required' => TRUE,
        '#default_value' => isset($_GET['search_location']) ? $_GET['search_location'] : '',
    );

    $form['getlocations_search_distance_' . $mapid] = array(
        '#type' => 'textfield',
        '#title' => t('Search distance'),
        '#size' => 10,
        '#required' => TRUE,
        '#description' => t('The distance away from the center to search for locations.'),
        '#default_value' => $default_distance,
    );
    $form['getlocations_search_lon_' . $mapid] = array(
        '#type' => 'textfield',
        '#default_value' => $default_longitude,
    );
    $form['getlocations_search_lat_' . $mapid] = array(
        '#type' => 'textfield',
        '#default_value' => $default_latitude,
    );

    $form['getlocations_search_submit_' . $mapid] = array(
        '#type' => 'submit',
        '#value' => t('Go')
    );
    $form['#attributes']['name'] = 'apachesolr_location_range_search_form_' . $mapid;

    //Set field name that are using for this facet.
    $form['field_name'] = array('#type' => 'value', '#value' => $field_name);

    return $form;
}

/**
 * Validate handler for apachesolr_location_range_widget_location_form
 */
function apachesolr_location_range_widget_location_form_validate($form, &$form_state) {
    $mapid = $form_state['values']['mapid'];
    if (!$form_state['values']['getlocations_search_lon_' . $mapid] ||
            !$form_state['values']['getlocations_search_lat_' . $mapid]) {
        form_set_error('getlocations_search_' . $mapid, t('Incorrect address.'));
    }
    if (!is_numeric($form_state['values']['getlocations_search_distance_' . $mapid])) {
        form_set_error('getlocations_search_distance_' . $mapid, t('Incorrect distance. Please write a number.'));
    }
}

/**
 * Submit handler of apachesolr_location_range_widget_location_form.
 */
function apachesolr_location_range_widget_location_form_submit($form, &$form_state) {

    $field_name = $form_state['values']['field_name'];
    $mapid = $form_state['values']['mapid'];
    $params = $_GET;

    $query_param = implode(',', array(
        $form_state['input']['getlocations_search_lat_' . $mapid],
        $form_state['input']['getlocations_search_lon_' . $mapid],
        $form_state['input']['getlocations_search_distance_' . $mapid],
            )
    );

    $exits_filter = FALSE;
    unset($params['q']);
    if (isset($params['f'])) {
        foreach (array_values($params['f']) as $filter) {
            if (strpos($filter, $field_name . ':', 0) !== FALSE) {
                $array_index = array_keys($params['f'], $filter);
                $index = reset($array_index);
                $params['f'][$index] = $field_name . ':' . $query_param;
                $exits_filter = TRUE;
            }
        }
    }

    if (!$exits_filter) {
        $params['f'][] = $field_name . ':' . $query_param;
    }
    $params['search_location'] = $form_state['input']['getlocations_search_' . $mapid];
    $form_state['redirect'] = array($_GET['q'], array('query' => $params));
}
