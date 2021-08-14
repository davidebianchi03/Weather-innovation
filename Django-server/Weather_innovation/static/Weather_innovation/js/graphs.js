$(document).ready(function () {
    $.getJSON("/weatherinnovation/getuserdevices", function (JSONdata) {
        for (var i in JSONdata) {
            device = JSONdata[i];
            $("#device_list").append(new Option(device, i));
            setInterval(refreshData(), 60000);
            if (i == 0) {
                //Disegno il grafico della temperatura
                drawTemperatureGraph(device);
                drawHumidityGraph(device);
                drawPressureGraph(device);
            }
        }
        
    });
});

function refreshData() {
    $.getJSON("/weatherinnovation/getuserdevices", function (JSONdata) {
        device = JSONdata[document.getElementById(device_list).value]
        //Disegno il grafico della temperatura
        drawTemperatureGraph(device);
        drawHumidityGraph(device);
        drawPressureGraph(device);
        

    });
}

function drawTemperatureGraph(device_id) {
    var data_to_send = {
        device: device_id,
        survey_type: "temperature"
    };
    $.ajax({
        type: 'POST',
        processData: false, // important
        contentType: false, // important
        data: JSON.stringify(data_to_send),
        url: "/weatherinnovation/getgraphsurvey",
        dataType: 'json',

        success: function (jsonData) {
            Highcharts.stockChart('temperatureContainer', {


                rangeSelector: {
                    selected: 1
                },

                title: {
                    text: 'Temperature'
                },

                series: [{
                    name: 'Temperature',
                    data: jsonData,
                    tooltip: {
                        valueDecimals: 2
                    }
                }]
            });
        }
    });
}

function drawHumidityGraph(device_id) {
    var data_to_send = {
        device: device_id,
        survey_type: "humidity"
    };
    $.ajax({
        type: 'POST',
        processData: false, // important
        contentType: false, // important
        data: JSON.stringify(data_to_send),
        url: "/weatherinnovation/getgraphsurvey",
        dataType: 'json',

        success: function (jsonData) {
            Highcharts.stockChart('humidityContainer', {


                rangeSelector: {
                    selected: 1
                },

                title: {
                    text: 'Humidity'
                },

                series: [{
                    name: 'Humidity',
                    data: jsonData,
                    tooltip: {
                        valueDecimals: 2
                    }
                }]
            });
        }
    });
}

function drawPressureGraph(device_id) {
    var data_to_send = {
        device: device_id,
        survey_type: "pressure"
    };
    $.ajax({
        type: 'POST',
        processData: false, // important
        contentType: false, // important
        data: JSON.stringify(data_to_send),
        url: "/weatherinnovation/getgraphsurvey",
        dataType: 'json',

        success: function (jsonData) {
            Highcharts.stockChart('pressureContainer', {


                rangeSelector: {
                    selected: 1
                },

                title: {
                    text: 'Pressure'
                },

                series: [{
                    name: 'Pressure',
                    data: jsonData,
                    tooltip: {
                        valueDecimals: 2
                    }
                }]
            });
        }
    });
}



