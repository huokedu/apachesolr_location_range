
/**
 * @file apachesolr_location_range_google_place
 *
 * @author Milka Petkova <milka@milka.petkova.88@gmail.com>
 * 
 * Javascript functions for apachesolr location range module
 */
(function($) {

    function apachesolr_location_range_search_init() {

        // each map has its own settings
        $.each(Drupal.settings.getlocations_search, function(key, searchsettings) {
            var mapid = key;
            var mapid2 = key.replace("_", "-");
            var form = $('form[name="apachesolr_location_range_search_form_-' + mapid2 + '"]');
            var longitude = $('#edit-getlocations-search-lon-' + mapid2);
            var latitude = $('#edit-getlocations-search-lat-' + mapid2);
            //Hide lotitude and longitude inputs.
            longitude.hide();
            latitude.hide();
            var input_adrs = $("#edit-getlocations-search-" + mapid2).get(0);
            var opts = {};
            var ac_adrs = new google.maps.places.Autocomplete(input_adrs, opts);
            var distance = $('#edit-getlocations-search-distance-' + mapid2);
            //Add event listener if address if filled in.
            google.maps.event.addListener(ac_adrs, 'place_changed', function() {
                var place_adrs = ac_adrs.getPlace();
                //Set latitute and longitude from google places.
                latitude.val(place_adrs.geometry.location.k);
                longitude.val(place_adrs.geometry.location.D);
            });

        });
    }
    Drupal.behaviors.ApachesolrLocationRange_search = {
        attach: function() {
            //Initialize search.
            apachesolr_location_range_search_init();
        }
    };

}(jQuery));
