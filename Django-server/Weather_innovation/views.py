from django.http import response
from Weather_innovation.models import Surveys
from django.http.response import HttpResponse, JsonResponse
from django.shortcuts import render
from django.views import generic
from django.views.decorators.csrf import csrf_exempt

# Create your views here.
class IndexView(generic.TemplateView):
    template_name = 'Weather_innovation/index.html'

@csrf_exempt
def getSurveysView(request):
    n_elements = Surveys.objects.all().count()
    response = list()

    for i in range(1,n_elements+1):
        data = Surveys.objects.get(id = 1)
        s_longitude = data.longitude
        s_latitude = data.latitude
        s_temperature = data.temperature
        s_pressure = data.pressure
        s_humidity = data.humidity
        s_time = data.time
        response.append({'longitude':s_longitude,'latitude':s_latitude,'temperature':s_temperature,'pressure':s_pressure,'humidity':s_humidity,'time':s_time})

    print(n_elements)
    print(response)
    return JsonResponse(response,safe=False)
