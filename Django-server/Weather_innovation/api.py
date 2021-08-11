from django.http import response
from django.http.response import HttpResponse, JsonResponse
from django.views.decorators.csrf import csrf_exempt
from .models import *
import json
import time

@csrf_exempt
def getSettingsApi(request):
    settings = Settings.objects.last()
    response = dict()
    response["Update_time"] = settings.update_time
    return JsonResponse(response, safe=False)

@csrf_exempt
def reciveDataApi(request):
    # {
    #     "longitude": 0.14564465,
    #     "latitude": 9.656,
    #     "temperature": 18,
    #     "pressure": 1013,
    #     "humidity": 72
    # }
    data = json.loads(request.body.decode('UTF-8'))
    
    s_longitude = data["longitude"]
    s_latitude = data["latitude"]
    s_temperature = data["temperature"]
    s_pressure = data["pressure"]
    s_humidity = data["humidity"]
    s_device_id = data["device_id"]

    survey_data = {
        'longitude': s_longitude,
        'latitude' : s_latitude,
        'temperature':s_temperature,
        'pressure':s_pressure,
        'humidity':s_humidity,
        'device_id':s_device_id,
        'time_in_millis': round(time.time() * 1000),
    }

    Surveys.objects.create(**survey_data)
    return HttpResponse("")