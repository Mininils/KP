#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MIN_FLOOR -2 
#define MAX_FLOOR 10 
#define MAX_INPUT_SIZE 100
#define LOG_FILE "system_log.txt"

typedef enum {UP, DOWN, INVALID} Napravl;

void monitorlift(int floor, Napravl nap);

Napravl napravlenie(const char* napStr);

void printMenu();

void logData(int floor, Napravl nap);

int main() {
    setlocale(LC_ALL, "RUS");
    char input[MAX_INPUT_SIZE];
    int floor;
    Napravl direction;
    int monitoring = 0;

    FILE* logFile = fopen(LOG_FILE, "w");
    if (logFile == NULL) {
        printf("Ошибка: не удалось открыть файл для логов.\n");
        return 1;
    }

    while (1) {
        printMenu();
        printf("Выберите вариант: ");
        int choice;
        scanf("%d", &choice);
        getchar();

        switch (choice) {
        case 1:
            monitoring = 1;
            printf("Мониторинг включен.\n");
            break;

        case 2:
            monitoring = 0;
            printf("Мониторинг выключен.\n");
            break;

        case 3:
            if (!monitoring) {
                printf("Мониторинг выключен, включите для получения данных.\n");
                break;
            }
            printf("Введите этаж и направление (up/down): ");
            fgets(input, sizeof(input), stdin);
            sscanf(input, "%d %s", &floor, input); 
            direction = napravlenie(input);

            if (floor < MIN_FLOOR || floor > MAX_FLOOR) {
                printf("Ошибка: этаж должен быть от %d до %d.\n", MIN_FLOOR, MAX_FLOOR);
            }
            else if (direction == INVALID) {
                printf("Ошибка: неверное направление. Используйте 'вверх' или 'вниз'.\n");
            }
            else {
                monitorlift(floor, direction);
                logData(floor, direction); 
            }
            break;

        case 4:
            printf("Выход из программы.\n");
            fclose(logFile);
            return 0;

        default:
            printf("Некорректный выбор. Попробуйте снова.\n");
            break;
        }
    }
    return 0;
}

void monitorlift(int floor, Napravl nap) {
    const char* directionStr = (nap == UP) ? "вверх" : "вниз";
    printf("Мониторинг: Этаж %d, Направление: %s\n", floor, directionStr);
}

Napravl napravlenie(const char* napStr) {
    if (strcmp(napStr, "up") == 0) {
        return UP;
    }
    else if (strcmp(napStr, "down") == 0) {
        return DOWN;
    }
    else {
        return INVALID;
    }
}

void printMenu() {
    printf("\nМеню:\n");
    printf("1. Включить мониторинг\n");
    printf("2. Выключить мониторинг\n");
    printf("3. Ввести этаж и направление\n");
    printf("4. Выход\n");
}

void logData(int floor, Napravl nap) {
    FILE* logFile = fopen(LOG_FILE, "a");
    if (logFile != NULL) {
        fprintf(logFile, "Этаж: %d, Направление: %s\n", floor, (nap == UP) ? "вверх" : "вниз");
        fclose(logFile); 
    }
    else {
        printf("Ошибка: не удалось записать в файл логов.\n");
    }
}



















