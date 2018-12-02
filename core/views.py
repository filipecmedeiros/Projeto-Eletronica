from django.shortcuts import render
from datetime import datetime

from .models import Usuario, Aula

# Create your views here.
def presenca (request, slug):
    now = datetime.now()
    usuario = None
    mensagem =''
    try:
        usuario = Usuario.objects.get(tag=slug)

        if (usuario.tipo.nome == 'Professor'):
            aula = Aula.objects.filter(professor=usuario, finalizada=False)
            print(aula)
            if (aula.count()==0):
                Aula.objects.create(professor=usuario, finalizada=False)
                mensagem = 'Aula iniciada'
            else:
                Aula.objects.filter(professor=usuario, finalizada = False).update(finalizada=True)
                mensagem = 'Aula finalizada'
        else:
            Aula.objects.get(finalizada=False).aluno.add(usuario)
            mensagem = 'Aluno presente'

    except:
        mensagem = 'Usuario nao cadastrado'
    context={
        'mensagem':mensagem,
    }
    return render (request, 'presenca.html', context)