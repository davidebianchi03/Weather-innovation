from django.db.models.expressions import F
from django.http import response
from Weather_innovation.models import Surveys, UsersDevices
from django.http.response import HttpResponse, JsonResponse
from django.shortcuts import render, redirect
from django.views import generic
from django.views.decorators.csrf import csrf_exempt
from django.contrib.auth import login, authenticate
from django.contrib import messages
from django.contrib.auth.forms import AuthenticationForm
from django.contrib.auth.decorators import login_required
from django.contrib.auth import logout
import time
import json

# Create your views here.
@login_required(login_url='/login')
def IndexView(request):
    return render(request=request, template_name = 'Weather_innovation/index.html')

@login_required(login_url='/login')
def TemperatureView(request):
    return render(request=request, template_name = 'Weather_innovation/temperature.html')

@login_required(login_url='/login')
def PressureView(request):
    return render(request=request, template_name = 'Weather_innovation/pressure.html')

@login_required(login_url='/login')
def HumidityView(request):
    return render(request=request, template_name = 'Weather_innovation/humidity.html')

@login_required(login_url='/login')
def GraphsView(request):
    return render(request=request, template_name = 'Weather_innovation/graphs.html')

@csrf_exempt
def GetUserDevices(request):
	devices_list =  UsersDevices.objects.filter(username = request.user.username).values_list('id', flat=True)
	response = list()
	for device in devices_list:
		response.append(UsersDevices.objects.get(id = device).device_id)
	return JsonResponse(response, safe=False)

@csrf_exempt
def getSurveysView(request):
	response = list()
	devices_list = UsersDevices.objects.filter(username = request.user.username).values_list('device_id', flat=True) 
	for device in devices_list:
		surveys_id_list = Surveys.objects.filter(device_id = device).values_list('id', flat=True)
		for i in surveys_id_list:
			data = Surveys.objects.get(id = i)
			s_longitude = data.longitude
			s_latitude = data.latitude
			s_temperature = data.temperature
			s_pressure = data.pressure
			s_humidity = data.humidity
			s_time = data.time
			response.append({'longitude':s_longitude,'latitude':s_latitude,'temperature':s_temperature,'pressure':s_pressure,'humidity':s_humidity,'time':s_time})

	return JsonResponse(response,safe=False)

@csrf_exempt
def getGraphSurveysView(request):
	json_dict = json.loads(request.body.decode('UTF-8'))
	device = json_dict["device"]
	survey_type = json_dict["survey_type"]#temperature, humidity....
	response = list()

	surveys_id_list = Surveys.objects.filter(device_id = device).values_list('id', flat=True)
	
	for i in surveys_id_list:
		data = Surveys.objects.get(id = i)
		survey_value = 0
		if survey_type == "temperature":
			survey_value = data.temperature

		if survey_type == "humidity":
			survey_value = data.humidity

		if survey_type == "pressure":
			survey_value = data.pressure
		
		survey = list()
		survey.append(data.time_in_millis)
		survey.append(survey_value)
		response.append(survey)

	return JsonResponse(response, safe=False)
	

@csrf_exempt
def getLatestSurveysView(request):
	response = list()
	devices_list = UsersDevices.objects.filter(username = request.user.username).values_list('device_id', flat=True) 
	for device in devices_list:
		data = Surveys.objects.filter(device_id = device).last()
		if round(time.time() * 1000) - data.time_in_millis < 3600000:
			s_longitude = data.longitude
			s_latitude = data.latitude
			s_temperature = data.temperature
			s_pressure = data.pressure
			s_humidity = data.humidity
			s_time = data.time
			response.append({'longitude':s_longitude,'latitude':s_latitude,'temperature':s_temperature,'pressure':s_pressure,'humidity':s_humidity,'time':s_time})
	
	return JsonResponse(response,safe=False)

@csrf_exempt
def login_request(request):
	if request.method == "POST":
		form = AuthenticationForm(request, data=request.POST)
		if form.is_valid():
			username = form.cleaned_data.get('username')
			password = form.cleaned_data.get('password')
			user = authenticate(username=username, password=password)
			if user is not None:
				login(request, user)
				messages.info(request, f"You are now logged in as {username}.")
				return redirect("Weather_innovation:index")
			else:
				messages.error(request,"Invalid username or password.")
		else:
			messages.error(request,"Invalid username or password.")
	form = AuthenticationForm()
	return render(request=request, template_name="Weather_innovation/login.html", context={"login_form":form})

def logout_view(request):
	logout(request)
	messages.info(request, "Logged out successfully!")
	return redirect("Weather_innovation:index")