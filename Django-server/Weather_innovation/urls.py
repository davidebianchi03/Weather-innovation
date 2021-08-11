from django.urls import path
from . import views
from . import api

app_name = 'Weather_innovation'
urlpatterns = [
    # URLs delle Views
    path('', views.IndexView, name='index'),
    path('weatherinnovation/getsurveys', views.getSurveysView, name = "getsurveys"),
    path("login", views.login_request, name="login"),
    path("logout", views.logout_view, name = "logout"),
    # URLs delle API
    path('api/getsettings', api.getSettingsApi, name="api_getsettings"),
    path('api/recivedata', api.reciveDataApi, name="api_recivedata"),
]
