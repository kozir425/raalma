#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "filesystem.txt"

// Функция, которая открывает (или создаёт и открывает) файл вашей файловой системы.
FILE* open_file(const char* filename) {
    FILE* file = fopen(filename, "a+");
    if (file == NULL) {
        perror("Ошибка при открытии файла");
        exit(EXIT_FAILURE);
    }
    return file;
}

// Функция для просмотра содержимого одного файла в файловой системе.
void view_file(const char* filename) {
    FILE* file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("Ошибка при открытии файла");
        exit(EXIT_FAILURE);
    }

    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strcmp(line, filename) == 0) {
            found = 1; // Файл найден
            printf("Содержимое файла %s:\n", filename);
            // Читаем содержимое файла
            while (fgets(line, sizeof(line), file) && line[0] != '/') {
                printf("%s", line);
            }
            break;
        }
    }

    if (!found) {
        printf("Файл %s не найден.\n", filename);
    }

    fclose(file);
}

// Функция для удаления файла в файловой системе.
void delete_file(const char* filename) {
    FILE* file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("Ошибка при открытии файла");
        exit(EXIT_FAILURE);
    }

    char line[256];
    FILE* temp_file = fopen("temp.txt", "w");
    if (temp_file == NULL) {
        perror("Ошибка при создании временного файла");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strcmp(line, filename) == 0) {
            found = 1; // Файл найден
            // Игнорируем следующую часть файлового содержимого
            while (fgets(line, sizeof(line), file) && line[0] != '/') {
                // Пропускаем, ничего не записываем в временный файл
            }
            continue; // Пропустим добавление строки с именем файла
        }
        fprintf(temp_file, "%s", line);
    }

    if (!found) {
        printf("Файл %s не найден для удаления.\n", filename);
    }

    fclose(file);
    fclose(temp_file);

    // Перезаписываем основной файл с временного
    remove(FILENAME);
    rename("temp.txt", FILENAME);
}

