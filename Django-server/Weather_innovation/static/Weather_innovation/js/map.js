$(document).ready(function() {
    mapboxgl.accessToken = "pk.eyJ1IjoiZGFkZWJpYTAzIiwiYSI6ImNrcnJ4ajZuZjBsN3Iydm1qdjVvbGFwNHQifQ.s5DsJNEChiE6jMqLvYClcA";
    var map = new mapboxgl.Map({
        container: 'map',
        style: 'mapbox://styles/mapbox/streets-v10',
        center: [9.24, 45.759],
        minZoom: 3
    });

    var popup = new mapboxgl.Popup()
        .setText('Description')
        .addTo(map);

    var marker = new mapboxgl.Marker()
        .setLngLat([9.24, 45.759])
        .addTo(map).setPopup(popup);

    marker1 = new mapboxgl.Marker()
        .setLngLat([39.24, 45.759])
        .addTo(map);
});