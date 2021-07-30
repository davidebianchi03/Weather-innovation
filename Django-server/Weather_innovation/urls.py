from django.urls import path
from . import views
from . import api

app_name = 'Weather_innovation'
urlpatterns = [
    # URLs delle Views
    path('', views.IndexView, name='index'),

    # URLs delle API
    path('api/getsettings', api.getSettingsApi, name="api_getsettings"),
    path('api/recivedata', api.reciveDataApi, name="api_recivedata"),
]
