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
        /*var popup = new mapboxgl.Popup()
            .setText('Description')
            .addTo(map);

        var marker = new mapboxgl.Marker()
            .setLngLat([9.24, 45.759])
            .addTo(map).setPopup(popup);*/


        // {
        //     "longitude": 0.14564465,
        //     "latitude": 9.656,
        //     "temperature": 18,
        //     "pressure": 1013,
        //     "humidity": 72
        // }

        var marker;
        var popup;

        for (let i = 0; i < JSONdata.length; i++) {
            longitude = JSONdata[i].longitude;
            latitude = JSONdata[i].latitude;
            temperature = JSONdata[i].temperature;
            pressure = JSONdata[i].pressure;
            humidity = JSONdata[i].humidity;
            time = JSONdata[i].time;

            var description = "<b>Temperature:<b> " + temperature + ", Pressure: " + pressure + "\nHumidity: " + humidity + ", Time: " + time;

            popup = new mapboxgl.Popup()
                .setText(description)
                .addTo(map);

            marker = new mapboxgl.Marker()
                .setLngLat([longitude, latitude])
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