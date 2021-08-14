from django.urls import path
from . import views
from . import api

app_name = 'Weather_innovation'
urlpatterns = [
    # URLs delle Views
    path('', views.IndexView, name='index'),
    path('temperature',views.TemperatureView, name='temperature'),
    path('pressure',views.PressureView, name='pressure'),
    path('humidity',views.HumidityView, name='humidity'),
    path('graphs',views.GraphsView, name='graphs'),
    path('weatherinnovation/getsurveys', views.getSurveysView, name = "getsurveys"),
    path('weatherinnovation/getlatestsurveys',views.getLatestSurveysView, name = "getlatestsurveys"),
    path('weatherinnovation/getuserdevices',views.GetUserDevices, name = "getuserdevices"),
    path('weatherinnovation/getgraphsurvey',views.getGraphSurveysView, name = "getgraphsurvey"),
    path("login", views.login_request, name="login"),
    path("logout", views.logout_view, name = "logout"),

    # URLs delle API
    path('api/getsettings', api.getSettingsApi, name="api_getsettings"),
    path('api/recivedata', api.reciveDataApi, name="api_recivedata"),
]
