
/*

  Trabalho 5 do G2 - Laboratório de Programação
  Christian Guilherme Fritsch
  Fábio Bica

*/

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
  FILE * db = fopen(DB, "r");
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
  
  return tmp;
}

int getMatriculaByBuffer(char buffer[]) {
  char * token = strtok(buffer, ",");
  return atoi(token);
};

cadastrarAluno(){
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
	printf("| ID: %d                                                                      |\n", counter);
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

	db = openDBMode("a");
	fprintf(db, "%d,%s,%f,%f,%f,%f;\n", 
	  counter,
    tmpAluno.nome, 
    tmpAluno.notas[0], 
    tmpAluno.notas[1],
    tmpAluno.notas[2],
    tmpAluno.notas[3]);
  
	closeDB(db);

	printf("\nSalvo!\n[ENTER]");
	getch();
	fflush(stdin);
	return;
};

void exibirAlunos() {
  int i = 0;
  int c = 0;
  Aluno tmp;
  system("cls");
	printf("_______________________________________________________________________________\n");
	printf("| ALUNOS CADASTRADOS                                                          |\n");
	FILE * db = openDBMode("r");
	if (db == NULL) {
	  return;
  }
	char buffer[1000];
	while(fgets(buffer, 1000, db)) {
	  tmp = line2struct(buffer);
    printf("| %d - %s\n", tmp.matricula, tmp.nome);
    c++;
	}
	if (c ==0) {
	  printf("| Nenhum aluno cadastrado!                                                    |\n");
	}
	close(db);
  printf("\n[ENTER]");
  getch();
  return;
}

void mediaDoAluno() {
  Aluno aluno;
  int i = 0;
  int has = 0;
  float media = 0;
  char buffer[1000];
  int id;
  int lastId = countRegisters();
  FILE * db = openDBMode("r");
  system("cls");
	printf("_______________________________________________________________________________\n");
	printf("| MEDIA DO ALUNO                                                             |\n");
	printf("| Matricula: ");
	scanf("%d", &id);
	fflush(stdin);
	
	while(fgets(buffer, 1000, db) && has == 0) {
	  aluno = line2struct(buffer);
	  if (aluno.matricula == id) {
	    has = 1;
	  }
	}
	
  if (has == 1) {
    printf("| Aluno: %s\n", aluno.nome);
    i = 0;
    while(i < 4) {
      media += aluno.notas[i];
      printf("| Nota %d:    %.2f\n", i+1, aluno.notas[i] );
      i++;
    }
    printf("| --------------------\n");
    printf("| Media :    %.2f\n\n[ENTER]", media/4);
  } else {
    printf("Aluno nao encontrado!\n[ENTER]");
  }
  closeDB(db);
  getch();
  return;
}

void mediaDaTurma() {
  int i = 0;
  int j = 0;
  int c = 0;
  int lastId = countRegisters();
  FILE * db = openDBMode("r");
  char buffer[1000];
  float medias[5];
  Aluno aluno;
  while(i < 5) {
    medias[i] = 0;
    i++;
  }
  system("cls");
	printf("_______________________________________________________________________________\n");
	printf("| MEDIAS DA TURMA                                                            |\n");
	printf("| Medias por nota                                                            |\n");
	i = 0;
	while (fgets(buffer, 1000, db)) {
	  aluno = line2struct(buffer);
	  while(i < 4) {
	    medias[i] += aluno.notas[i];
	    medias[4] += aluno.notas[i]; //media total
	    i++;
    }
  	c++;
  	i = 0;
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
  closeDB(db);
  
  getch();
  return;
  
}

void excluirAluno() {
  int i = 0;
  int id = 0;
  int found = 0;
  Aluno aluno;
  system("cls");
	printf("_______________________________________________________________________________\n");
	printf("| EXCLUIR ALUNO                                                               |\n");
	printf("| Matricula: ");
	scanf("%d", &id);
	fflush(stdin);
	char buffer[1000];
	char bufferAux[1000];
  	
  // inicia um arquivo temporário novo
	FILE * tmpDB = fopen("database.tmp", "w");
	if (tmpDB == NULL) {
	  printf("\n Falha ao criar o arquivo temporário!\n");
	  return;
	}
	closeDB(tmpDB);
	// abre o arquivo temporário em modo "append"
	tmpDB = fopen("database.tmp", "a");
	if (tmpDB == NULL) {
	  printf("\n Falha ao abrir o arquivo temporário!\n");
	  return;
	}
	FILE * db = openDBMode("r+");	

	while(fgets(buffer, 1000, db)) {
	  strcpy(bufferAux, buffer);
	  if (getMatriculaByBuffer(buffer) != id) {
	    fputs(bufferAux, tmpDB);
	  } else {
	    strcpy(buffer, bufferAux);
	    aluno = line2struct(buffer);
	    i = 0;
    	while(i != 1 && i != 2){
    	  printf("| Aluno: %s\n", aluno.nome);
    	  printf("| 1) Excluir  2) Nao excluir:\n");
    	  printf("| > ");
    	  scanf("%d", &i);
    	  fflush(stdin);
    	}
    	if (i != 1) {
    	  fputs(bufferAux, tmpDB);
    	  return;
    	}
    	found = 1;
	  }
	}
	if (found == 1) {
	  closeDB(tmpDB);
	  closeDB(db);
	  tmpDB = fopen("database.tmp", "r");
	  db = openDBMode("w");
	  while(fgets(buffer, 1000, tmpDB)) {
	    fputs(buffer, db);
	  }
	}
	closeDB(db);
	closeDB(tmpDB);
	remove("database.tmp");
	if (found == 1) {
	  printf("\nExcluido!\n\n[ENTER]");
  	getch();
  	fflush(stdin);
  	return;
	}
	printf("\nNao encontrado!\n\n[ENTER]");
	getch();
	fflush(stdin);
	return;
}

void procurarAlunos() {
  char parametro[255];
  char nomeTemp[255];
  Aluno aluno;
  FILE * db = openDBMode("r");
  char buffer[1000];
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
	while(fgets(buffer, 1000, db)) {
	  aluno = line2struct(buffer);
	  strcpy(nomeTemp, aluno.nome);
	  j = 0;
	  while(nomeTemp[j] != '\0') {
	    nomeTemp[j] = toupper(nomeTemp[j]);
	    j++;
	  }
	  if (strstr(nomeTemp, parametro)) {
	    printf("| %d - %s\n", aluno.matricula, aluno.nome);
	    c++;
	  }
  	i++;
	}
	if (c == 0) {
	  printf("| Nenhum aluno encontrado!                                                    |\n");
	}
	closeDB(db);
	printf("\n\n[ENTER]");
	getch();
  return;
}

int main() {
  
  // selftest
  FILE * db = openDB();
  closeDB(db);
  //
  int lastId = countRegisters();
	int opt = 999;
	
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
  			cadastrarAluno();
  			break;
  		case 2:
  			exibirAlunos();
  			break;
  	  case 3:
  			mediaDoAluno();
  			break;
  		case 4:
  			mediaDaTurma();
  			break;
  		case 5:
  			excluirAluno();
  			break;
  		case 6:
  			procurarAlunos();
  			break;
  	}
	}
	
	return 0;
}

