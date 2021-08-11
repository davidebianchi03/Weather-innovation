from Weather_innovation.models import Settings
from django.contrib import admin
from .models import *

# Register your models here.
class SettingsAdmin(admin.ModelAdmin):
    list_display = ('update_time',)
admin.site.register(Settings, SettingsAdmin)

class SurveysAdmin(admin.ModelAdmin):
    list_display = ('id','longitude','latitude','time','temperature','pressure','humidity','device_id','time_in_millis')
admin.site.register(Surveys,SurveysAdmin)

class UsersDevicesAdmin(admin.ModelAdmin):
    list_display = ('username','device_id')
admin.site.register(UsersDevices, UsersDevicesAdmin)