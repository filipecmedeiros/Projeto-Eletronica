from django.db import models

# Create your models here.


class Tipo(models.Model):
    nome = models.CharField('Nome', max_length=255)

    class Meta:
        verbose_name = 'Tipo'
        verbose_name_plural = 'Tipos'

    def __str__(self):
        return self.nome


class Usuario (models.Model):
    nome = models.CharField(max_length=255)
    tag = models.SlugField('Tag', max_length=255)
    tipo = models.ForeignKey(
        Tipo, on_delete=models.CASCADE, verbose_name='Tipo')

    created = models.DateTimeField('Criado em', auto_now_add=True)
    modified = models.DateTimeField('Modificado em', auto_now=True)

    class Meta:
        verbose_name = 'Usuario'
        verbose_name_plural = 'Usuarios'
        ordering = ['nome']

    def __str__(self):
        return self.nome


class Aula(models.Model):
    professor = models.ForeignKey(
        Usuario, on_delete=models.Model, related_name='Professor')
    aluno = models.ManyToManyField(
        Usuario, blank=True, related_name='Aluno')
    finalizada = models.BooleanField('Finalizada', null=True, blank=True)

    created = models.DateTimeField('Iniciado em', auto_now_add=True)
    modified = models.DateTimeField('Finalizado em', auto_now=True)

    class Meta:
        verbose_name = 'Aula'
        verbose_name_plural = 'Aulas'
        ordering = ['created']

    def __str__(self):
        return 'Aula ' + str(self.id) + ':' + str(self.created)
