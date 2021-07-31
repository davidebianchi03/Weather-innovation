from django.urls import path
from . import views
from . import api

app_name = 'Weather_innovation'
urlpatterns = [
    # URLs delle Views
    path('', views.IndexView.as_view(), name='index'),
    path('weatherinnovation/getsurveys', views.getSurveysView, name = "getsurveys"),

    # URLs delle API
    path('api/getsettings', api.getSettingsApi, name="api_getsettings"),
    path('api/recivedata', api.reciveDataApi, name="api_recivedata"),
]
