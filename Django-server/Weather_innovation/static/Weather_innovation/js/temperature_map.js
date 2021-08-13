mapboxgl.accessToken = "pk.eyJ1IjoiZGFkZWJpYTAzIiwiYSI6ImNrcnJ4ajZuZjBsN3Iydm1qdjVvbGFwNHQifQ.s5DsJNEChiE6jMqLvYClcA";

var map = new mapboxgl.Map({
    container: 'map',
    style: 'mapbox://styles/mapbox/streets-v10',
    center: [9.24, 45.759],
    minZoom: 3,
    interactive: true
});

const metersToPixelsAtMaxZoom = (meters, latitude) =>
  meters / 0.075 / Math.cos(latitude * Math.PI / 180)

map.on('style.load', function (e) {
    $.getJSON("/weatherinnovation/getlatestsurveys", function (JSONdata) {
        var marker;
        var popup;

        for (let i = 0; i < JSONdata.length; i++) {
            longitude = JSONdata[i].longitude;
            latitude = JSONdata[i].latitude;
            temperature = JSONdata[i].temperature;
            pressure = JSONdata[i].pressure;
            humidity = JSONdata[i].humidity;
            time = JSONdata[i].time;

            var colors = ["#170396","#425af5","#42d4f5","#42f584","#99f542","#eff542","#f5a742","#f54242","#9e1800","#7d1300"];
            var color_change_values = [-10,-4,2,8,14,20,26,32];
            var color = "";

            if(temperature < color_change_values[0])
                color = colors[0];
            
            if(temperature >= color_change_values[0] && temperature < color_change_values[1])
                color = colors[1];

            if(temperature >= color_change_values[1] && temperature < color_change_values[2])
                color = colors[2];

            if(temperature >= color_change_values[2] && temperature < color_change_values[3])
                color = colors[3];

            if(temperature >= color_change_values[3] && temperature < color_change_values[4])
                color = colors[4];

            if(temperature >= color_change_values[4] && temperature < color_change_values[5])
                color = colors[5];

            if(temperature >= color_change_values[5] && temperature < color_change_values[6])
                color = colors[6];

            if(temperature >= color_change_values[6] && temperature < color_change_values[7])
                color = colors[7];

            if(temperature >= color_change_values[7] && temperature < color_change_values[8])
                color = color[8];

            if(temperature >= color_change_values[8])
                color = color[8];

            
            map.addSource('markers', {
                "type": "geojson",
                "data": {
                    "type": "FeatureCollection",
                    "features": [{
                        "type": "Feature",
                        "geometry": {
                            "type": "Point",
                            "coordinates": [longitude, latitude]
                        },
                        "properties": {
                            "modelId": 1,
                        },
                    }]
                }
            });

            

            map.addLayer({
                "id": "circles1",
                "source": "markers",
                "type": "circle",
                "paint": {
                    "circle-radius": {
                        stops: [
                          [0, 0],
                          [20, metersToPixelsAtMaxZoom(1500, latitude)]
                        ],
                        base: 2
                      },
                    "circle-color": color,
                    "circle-opacity": 0.5,
                    "circle-stroke-width": 0,
                },
                "filter": ["==", "modelId", 1],
            });

            var description = "Temperature: " + temperature + "°C";

            const popup = new mapboxgl.Popup({ closeOnClick: false })
            .setLngLat([longitude,latitude])
            .setHTML('<b>Temperature:</b> ' + temperature + "°C")
            .addTo(map);

            
        }

    });


});

//Ridimensionamento sidebar
$(document).ready(function () {
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
});

$("#btn_collapse").click(function () {
    $("#map").width('150%');
    window.scrollLeft = 0;
    map.resize();
});