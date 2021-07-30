from django.db import models

# Create your models here.
class Settings(models.Model):
    update_time = models.IntegerField()#Tempo che passa tra un invio dei dati dalla stazione meteo e il successivo invio

    def __str__(self) -> str:
        return "Update time: " + str(self.update_time)

    class Meta:
        verbose_name = 'Settings'
        verbose_name_plural = 'Settings'

class Surveys(models.Model):

    longitude = models.FloatField()
    latitude = models.FloatField()
    time = models.DateTimeField(auto_now=True)
    temperature = models.FloatField()
    pressure = models.FloatField()
    humidity = models.FloatField()

    class Meta:
        verbose_name = 'Surveys'
        verbose_name_plural = 'Surveys'