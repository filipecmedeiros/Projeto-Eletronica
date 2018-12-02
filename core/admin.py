from django.contrib import admin
from django.db import models
from django.forms import CheckboxSelectMultiple
from .models import Tipo, Usuario, Aula

# Register your models here.
class TipoAdmin(admin.ModelAdmin):
	list_display = ['nome']
	list_filter = ['nome']
	search_display = ['nome']
	search_fields = ['nome']

admin.site.register(Tipo, TipoAdmin)

class UsuarioAdmin(admin.ModelAdmin):
	list_display = ['nome', 'tag', 'tipo', 'created', 'modified']
	list_filter = ['tipo']
	search_display = ['nome', 'tag']
	search_fields = ['nome', 'tag']

admin.site.register(Usuario, UsuarioAdmin)

class AulaAdmin(admin.ModelAdmin):
	list_display = ['professor', 'finalizada', 'created', 'modified']
	list_filter = ['professor', 'finalizada']
	search_display = ['professor', 'aluno', 'finalizada']
	search_fields = ['professor', 'aluno', 'created']

	formfield_overrides = {
        models.ManyToManyField: {'widget': CheckboxSelectMultiple},
    }

admin.site.register(Aula, AulaAdmin)