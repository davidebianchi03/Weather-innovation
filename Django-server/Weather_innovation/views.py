from django.http import response
from Weather_innovation.models import Surveys
from django.http.response import HttpResponse, JsonResponse
from django.shortcuts import render, redirect
from django.views import generic
from django.views.decorators.csrf import csrf_exempt
from django.contrib.auth import login, authenticate
from django.contrib import messages
from django.contrib.auth.forms import AuthenticationForm
from django.contrib.auth.decorators import login_required
from django.contrib.auth import logout

# Create your views here.
@login_required(login_url='/login')
def IndexView(request):
    return render(request=request, template_name = 'Weather_innovation/index.html')

@csrf_exempt
def getSurveysView(request):
    
    response = list()

    surveys_id_list = Surveys.objects.values_list('id', flat=True)
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