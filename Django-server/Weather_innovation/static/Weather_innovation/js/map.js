$(document).ready(function() {
    mapboxgl.accessToken = "pk.eyJ1IjoiZGFkZWJpYTAzIiwiYSI6ImNrcnJ4ajZuZjBsN3Iydm1qdjVvbGFwNHQifQ.s5DsJNEChiE6jMqLvYClcA";
    var map = new mapboxgl.Map({
        container: 'map',
        style: 'mapbox://styles/mapbox/streets-v10',
        center: [9.24, 45.759],
        minZoom: 3
    });

    if (/Android|webOS|iPhone|iPad|Mac|Macintosh|iPod|BlackBerry|IEMobile|Opera Mini/i.test(navigator.userAgent)) {
        $("#map").width('150%');
        window.scrollLeft = 0;
        map.resize();
    }

    if (window.width < 800) {
        $("#map").width('150%');
        window.scrollLeft = 0;
        map.resize();
    }

    $.getJSON("/weatherinnovation/getlatestsurveys", function(JSONdata) {
        var marker;
        var popup;

        for (let i = 0; i < JSONdata.length; i++) {
            longitude = JSONdata[i].longitude;
            latitude = JSONdata[i].latitude;
            temperature = JSONdata[i].temperature;
            pressure = JSONdata[i].pressure;
            humidity = JSONdata[i].humidity;
            time = JSONdata[i].time;

            var description = "Temperature: " + temperature + "°C, Pressure: " + pressure + "mBar\nHumidity: " + humidity + "%, Time: " + time;

            popup = new mapboxgl.Popup()
                .setText(description)
                .addTo(map);

            marker = new mapboxgl.Marker()
                .setLngLat([longitude,latitude])
                .addTo(map).setPopup(popup);

            map.fire('closeAllPopups');
            popup.remove(); //Chiudo tutti i popup
        }


        $("#btn_collapse").click(function() {
            $("#map").width('150%');
            window.scrollLeft = 0;
            map.resize();
        });

    });
});