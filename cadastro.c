#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_EMAIL_LENGTH 50

// Estrutura para armazenar os dados do usuario
typedef struct {
    char name[MAX_NAME_LENGTH];
    char email[MAX_EMAIL_LENGTH];
} User;

// Função para adicionar um novo usuario ao cadastro
void addEntry(FILE* file) {
    User user;
    printf("Digite o nome do usuario: ");
    fgets(user.name, MAX_NAME_LENGTH, stdin);
    printf("Digite o email do usuario: ");
    fgets(user.email, MAX_EMAIL_LENGTH, stdin);
    fprintf(file, "%s;%s\n", user.name, user.email);
    printf("Usuario adicionado com sucesso!\n");
}

// Função para excluir um usuario do cadastro
void deleteEntry(FILE* file) {
    char name[MAX_NAME_LENGTH];
    printf("Digite o nome do usuario que deseja excluir: ");
    fgets(name, MAX_NAME_LENGTH, stdin);

    FILE* tempFile = fopen("temp.txt", "w");
    char line[MAX_NAME_LENGTH + MAX_EMAIL_LENGTH + 2];
    int found = 0;
    int skipLine = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (skipLine) {
            skipLine = 0;
            continue;
        }

        if (strstr(line, name) == NULL) {
            fputs(line, tempFile);
        } else {
            found = 1;
            skipLine = 1;
        }
    }
    
    fclose(tempFile);
    fclose(file);

    if (found) {
        remove("cadastro.txt");
        rename("temp.txt", "cadastro.txt");
        printf("Usuario removido com sucesso!\n");
    } else {
        remove("temp.txt");
        printf("Usuario nao encontrado.\n");
    }

    file = fopen("cadastro.txt", "a+");
}

// Função para editar os dados de um usuario do cadastro
void editEntry(FILE* file) {
    char name[MAX_NAME_LENGTH];
    printf("Digite o nome do usuario que deseja editar: ");
    fgets(name, MAX_NAME_LENGTH, stdin);

    FILE* tempFile = fopen("temp.txt", "w");
    char line[MAX_NAME_LENGTH + MAX_EMAIL_LENGTH + 2];
    int found = 0;
    int skipLine = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (skipLine) {
            skipLine = 0;
            continue;
        }
        
        if (strstr(line, name) == NULL) {
            fputs(line, tempFile);
        } else {
            User user;
            printf("Digite o novo nome do usuario: ");
            fgets(user.name, MAX_NAME_LENGTH, stdin);
            printf("Digite o novo email do usuario: ");
            fgets(user.email, MAX_EMAIL_LENGTH, stdin);
            fprintf(tempFile, "%s;%s\n", user.name, user.email);
            found = 1;
            skipLine = 1;
        }
    }

    fclose(tempFile);
    fclose(file);

    if (found) {
        remove("cadastro.txt");
        rename("temp.txt", "cadastro.txt");
        printf("Dados do usuario atualizados com sucesso!\n");
    } else {
        remove("temp.txt");
        printf("Usuario nao encontrado.\n");
    }

    file = fopen("cadastro.txt", "a+");
}

// Função para exibir todos os usuarios cadastrados
void viewEntries(FILE* file) {
    char line[MAX_NAME_LENGTH + MAX_EMAIL_LENGTH + 2];
    printf("\n=== Cadastro de Usuarios ===\n");
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }
    printf("===========================\n\n");
}

int main() {
    FILE* file = fopen("cadastro.txt", "a+");

    int choice;
    do {
        printf("=== MENU ===\n");
        printf("1. Adicionar usuario\n");
        printf("2. Excluir usuario\n");
        printf("3. Editar usuario\n");
        printf("4. Visualizar usuarios cadastrados\n");
        printf("5. Fechar o programa\n");
        printf("Escolha uma opçao: ");
        scanf("%d", &choice);
        getchar();  // Limpar o caractere de nova linha do buffer

        switch (choice) {
            case 1:
                addEntry(file);
                fclose(file);
                file = fopen("cadastro.txt", "a+");
                break;
            case 2:
                deleteEntry(file);
                break;
            case 3:
                editEntry(file);
                break;
            case 4:
                fclose(file);
                file = fopen("cadastro.txt", "a+");
                viewEntries(file);
                fclose(file);
                file = fopen("cadastro.txt", "a+");
                break;
            case 5:
                printf("Programa encerrado.\n");
                break;
            default:
                printf("Opçao inválida. Por favor, tente novamente.\n");
        }
    } while (choice != 5);

    fclose(file);
    return 0;
}