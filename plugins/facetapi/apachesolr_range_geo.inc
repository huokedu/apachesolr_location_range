<?php

/**
 * @file
 * Plugin for "apachesolr_range_geo" query types.
 *
 * @author Milka Petkova <milka@milka.petkova.88@gmail.com>
 */
class ApacheSolrFacetapiRangeGeo extends FacetapiQueryType implements FacetapiQueryTypeInterface {

    /**
     * Returns the query type associated with the plugin.
     *
     * @return string
     *   The query type.
     */
    static public function getType() {
        return 'range_geo';
    }

    /**
     * Adds the filter to the query object.
     *
     * @param DrupalSolrQueryInterface $query
     *   An object containing the query in the backend's native API.
     */
    public function execute($query) {
        $active_items = $this->adapter->getActiveItems($this->facet);
        $lat = $lon = $distance = FALSE;
        if ($active_items) {
            list($lat, $lon, $distance) = explode(',', key($active_items));
            if ($lat && $lon && $distance) {
                //Alter apachesolr query.
                $query->addParam('fq', '{!geofilt}');
                $query->addParam('sfield', $this->facet['field']);
                $query->addParam('pt', $lat . ',' . $lon);
                $query->addParam('d', $distance);
                $query->addParam('sort', 'geodist() asc');
            }
        }
    }

    /**
     * Initializes the facet's build array.
     *
     * @return array
     *   The initialized render array.
     */
    public function build() {
        $build = array();
        //Set true, because facet needs to be shown always, not with
        //standart functiolity - only when there is a result.
        $build['true'] = array();
        return $build;
    }

}
