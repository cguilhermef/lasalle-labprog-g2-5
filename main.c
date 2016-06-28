#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DB "database.db"

typedef struct{
	int matricula;
	char nome[255];
	float notas[4];
	int ativo;
} Aluno;

FILE * openDB() {
	FILE * db = fopen(DB, "r+");
	
	if (!db) {
		db = fopen(DB, "w+");
	}
	
	return db;
}

FILE * openDBMode(char mode[2]) {
	FILE * db = fopen(DB, mode);
	
	if (!db) {
	  printf("\n Falha ao abrir o arquivo!\n");
	  getch();
  	fflush(stdin);
  	return;
	}
	
	return db;
}

void closeDB(FILE * db) {
  fclose(db);
  return;
}

int countRegisters() {
  FILE * db = openDBMode("r");
  int count = 0;
  
  if (db) {
    while(!feof(db) && count < 1000) {
      if (getc(db) == ';') {
        count++;
      }
    }
  }
  closeDB(db);
  return count;
}
Aluno line2struct(char param[]) {
  Aluno tmp;
  char buffer[1000];
  int c = 0;
  while(param[c] != ';') {
    buffer[c] = param[c];
    c++;
  }
  buffer[c] = '\0';
  char * aux = strtok(buffer, ",");
  tmp.matricula = atoi(aux);
  aux = strtok(NULL, ",");
  strcpy(tmp.nome, aux);
  aux = strtok(NULL, ",");
  tmp.notas[0] = atof(aux);
  aux = strtok(NULL, ",");
  tmp.notas[1] = atof(aux);
  aux = strtok(NULL, ",");
  tmp.notas[2] = atof(aux);
  aux = strtok(NULL, ",");
  tmp.notas[3] = atof(aux);
  aux = strtok(NULL, ",");
  tmp.ativo = atoi(aux);
  
  return tmp;
}

cadastrarAluno(int *id, Aluno *alunos){
  int i = 0;
  Aluno tmpAluno;
  FILE * db;
  int counter = countRegisters();
	system("cls");
	printf("_______________________________________________________________________________\n");
	printf("| CADASTRAR ALUNO                                                             |\n");
	printf("| Nome do aluno: ");
	gets(tmpAluno.nome);
	fflush(stdin);
	while(i < 4) {
	  printf("| Nota %d: ", i+1);
	  scanf("%f", &tmpAluno.notas[i]);
	  fflush(stdin);
	  i++;
	}	
	printf("_______________________________________________________________________________\n");
	printf("| ID: %d                                                                      |\n", *id);
	printf("| Nome do aluno: %s\n", tmpAluno.nome);
	i = 0;
	while(i < 4) {
	  printf("| Nota %d: %.2f\n", i+1, tmpAluno.notas[i]);
	  i++;
	}
	i = 0;
	while(i != 1 && i != 2){
	  printf("| 1) Salvar  2) Descartar:\n");
	  printf("| > ");
	  scanf("%d", &i);
	  fflush(stdin);
	}
	if (i != 1) {
	  return;
	}	
	
	i = 0;
	while(i < 4) {
	  (alunos+(*id))->notas[i] = tmpAluno.notas[i];
	  i++;
	}
	(*id)++;
	
	
	
	db = openDBMode("a");
	fprintf(db, "%d,%s,%f,%f,%f,%f,%d;\n", 
	  counter,
    tmpAluno.nome, 
    tmpAluno.notas[0], 
    tmpAluno.notas[1],
    tmpAluno.notas[2],
    tmpAluno.notas[3],
    1);
  
	closeDB(db);

	printf("\nSalvo!\n[ENTER]");
	getch();
	fflush(stdin);
	return;
};

void exibirAlunos(int lastId, Aluno alunos[]) {
  int i = 0;
  int c = 0;
  Aluno tmp;
  system("cls");
	printf("_______________________________________________________________________________\n");
	printf("| ALUNOS CADASTRADOS                                                          |\n");
	FILE * db = openDBMode("r");
	char buffer[1000];
	while(fgets(buffer, 1000, db)) {
	  tmp = line2struct(buffer);
	  if (tmp.ativo == 1) {
	    printf("| %d - %s\n", tmp.matricula, tmp.nome);
	    c++;
	  }
	}
	if (c ==0) {
	  printf("| Nenhum aluno cadastrado!                                                    |\n");
	}
	close(db);
  printf("\n[ENTER]");
  getch();
  return;
}

void mediaDoAluno(int lastId, Aluno alunos[]) {
  Aluno aluno;
  int i = 0;
  int has = 0;
  float media = 0;
  int id;
  system("cls");
	printf("_______________________________________________________________________________\n");
	printf("| MEDIA DO ALUNO                                                             |\n");
	printf("| Matricula: ");
	scanf("%d", &id);
	fflush(stdin);
	
  while (i < lastId) {
    if ( alunos[i].matricula == id && alunos[i].ativo == 1) {
      has = 1;
    }
    i++;
  }
  if (has == 1) {
    printf("| Aluno: %s\n", alunos[id].nome);
    i = 0;
    while(i < 4) {
      media += alunos[id].notas[i];
      printf("| Nota %d:    %.2f\n", i+1, alunos[id].notas[i] );
      i++;
    }
    printf("| --------------------\n");
    printf("| Media :    %.2f\n\n[ENTER]", media/4);
  } else {
    printf("Aluno nao encontrado!\n[ENTER]");
  }
  
  getch();
  return;
}

void mediaDaTurma(int lastId, Aluno alunos[]) {
  int i = 0;
  int j = 0;
  int c = 0;
  float medias[5];
  while(i < 5) {
    medias[i] = 0;
    i++;
  }
  system("cls");
	printf("_______________________________________________________________________________\n");
	printf("| MEDIAS DA TURMA                                                            |\n");
	printf("| Medias por nota                                                            |\n");
	i = 0;
	while(i < lastId) {
	  if (alunos[i].ativo == 1) {
  	  while(j < 4) {
  	    medias[j] += (alunos+i)->notas[j]; //media da nota
  	    medias[4] += (alunos+i)->notas[j]; //media total
  	    j++;
  	  }
  	  c++;
  	}
	  j = 0;
	  i++;
	}
	
	if (lastId > 0) {
    i = 0;
    while(i < 4) {
      printf("| Nota %d:    %.2f\n", i+1, medias[i]/c);
      i++;
    }
    printf("| --------------------------\n");
    printf("| Media :    %.2f (todas as notas)\n\n[ENTER]", medias[4]/(c*4));
  } else {
    printf("| Nenhum aluno cadastrado!\n[ENTER]");
  }
  
  getch();
  return;
  
}

void excluirAluno(int lastId, Aluno *alunos) {
  int i = 0;
  int id = 0;
  int found = 0;
  system("cls");
	printf("_______________________________________________________________________________\n");
	printf("| EXCLUIR ALUNO                                                               |\n");
	printf("| Matricula: ");
	scanf("%d", &id);
	fflush(stdin);
	
	while(i < lastId) {
	  if (alunos[i].ativo == 1 && alunos[i].matricula == id) {
	    found = 1;
	  }
	  i++;
	}
	
	if (found == 1) {
	  i = 0;
  	while(i != 1 && i != 2){
  	  printf("| Aluno: %s\n", alunos[id].nome);
  	  printf("| 1) Excluir  2) Nao excluir:\n");
  	  printf("| > ");
  	  scanf("%d", &i);
  	  fflush(stdin);
  	}
  	if (i != 1) {
  	  return;
  	}
  	alunos[id].ativo = 0;
  	printf("\nExcluido!\n\n[ENTER]");
  	getch();
  	fflush(stdin);
  	return;
	} else {
    printf("| Aluno nao encontrado!                                                       |\n");
	}
}

void cadastroPorParametro(char *argv[], int *lastId, Aluno *alunos) {
  int i = 0;
	int j = 0;
	int c = 0;
	char *aluno;
	char *alunosTemp[10];
	char *campos;
	aluno = strtok(argv[1], "@");
//	
	while (aluno) {
	  alunosTemp[*lastId] = malloc(sizeof(aluno));
	  alunosTemp[*lastId] = aluno;
	  (*lastId)++;
	  aluno = strtok(NULL, "@");
	}
	while (i < *lastId) {
	  c = 0;
	  j = 0;
	  campos = NULL;
	  campos = strtok(alunosTemp[i], "#");	  
	  while(campos != NULL) {
	    printf("\n%s", campos);
	    switch (c) {
	      case 0:
	        alunos[i].matricula = i;
	        alunos[i].ativo = 1;
	        break;
	      case 1:
	        //strncpy(alunos[i].nome, campos, sizeof(campos));
	        strcpy(alunos[i].nome, campos);
	        break;
	      default:
	        alunos[i].notas[j] = atof(campos);
	        j++;
	    }
	    c++;
	    campos = strtok(NULL, "#");
	  }
	  i++;
	}
	return;
}

void procurarAlunos(int lastId, Aluno alunos[]) {
  char parametro[255];
  char nomeTemp[255];
  int i = 0;
  int j = 0;
  int c = 0;
  
  system("cls");
	printf("_______________________________________________________________________________\n");
	printf("| ALUNOS POR NOME                                                             |\n");
	printf("| Buscar por: ");
	gets(parametro);
	while(parametro[j] != '\0') {
	  parametro[j] = toupper(parametro[j]);
	  j++;
	}
	printf("|\n");
	while(i < lastId) {
	  if (alunos[i].ativo == 1) {
  	  strcpy(nomeTemp, alunos[i].nome);
  	  j = 0;
  	  while(nomeTemp[j] != '\0') {
  	    nomeTemp[j] = toupper(nomeTemp[j]);
  	    j++;
  	  }
  	  if (strstr(nomeTemp, parametro)) {
  	    printf("| %d - %s\n", alunos[i].matricula, alunos[i].nome);
  	    c++;
  	  }
  	}
  	i++;
	}
	if (c == 0) {
	  printf("| Nenhum aluno encontrado!                                                    |\n");
	}
	printf("\n\n[ENTER]");
	getch();
  return;
}

int main(int argc, char *argv[]) {

	Aluno alunos[10];
  int lastId = countRegisters();
	int opt = 999;
  
  if (argc > 1) {
    cadastroPorParametro(argv, &lastId, alunos);
  }
	
	while (opt != 0) {
		system("cls");
		printf("_______________________________________________________________________________\n");
		printf("| ESCOLHA UMA OPCAO:                                                          |\n");
		printf("| 1 - Cadastrar aluno                                                         |\n");
		printf("| 2 - Exibir alunos cadastrados                                               |\n");
		printf("| 3 - Exibir media do aluno                                                   |\n");
		printf("| 4 - Exibir media da turma                                                   |\n");
		printf("| 5 - Excluir aluno                                                           |\n");
		printf("| 6 - Exibir alunos por nome                                                  |\n");
		printf("| 0 - Sair                                                                    |\n");
		printf("| >> ");
		scanf("%d", &opt);
    fflush(stdin);
    
		switch(opt){
  		case 1:
  			cadastrarAluno(&lastId, alunos);
  			break;
  		case 2:
  			exibirAlunos(lastId, alunos);
  			break;
  	  case 3:
  			mediaDoAluno(lastId, alunos);
  			break;
  		case 4:
  			mediaDaTurma(lastId, alunos);
  			break;
  		case 5:
  			excluirAluno(lastId, alunos);
  			break;
  		case 6:
  			procurarAlunos(lastId, alunos);
  			break;
  	}
	}
	
	return 0;
}

