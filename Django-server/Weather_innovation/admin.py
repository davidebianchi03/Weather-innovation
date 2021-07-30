from Weather_innovation.models import Settings
from django.contrib import admin
from .models import *

# Register your models here.
class SettingsAdmin(admin.ModelAdmin):
    list_display = ('update_time',)
admin.site.register(Settings, SettingsAdmin)

class SurveysAdmin(admin.ModelAdmin):
    list_display = ('longitude','latitude','time','temperature','pressure','humidity')
admin.site.register(Surveys,SurveysAdmin)