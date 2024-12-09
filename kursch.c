#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#define _CRT_SECURE_NO_DEPRECATE
#define MIN_FLOOR 1
#define MAX_FLOOR 10
#define NUM_TEST_CASES 5
#define FILENAME "test_data.txt"
#define LOGFILE "system_log.txt"
#define MAX_LOG_ENTRIES 100
#define MAX_QUEUE_SIZE 20

typedef struct {
    int floor;
    char direction[10];
} ElevatorCall;

ElevatorCall logEntries[MAX_LOG_ENTRIES]; 
int logCount = 0;

ElevatorCall queue[MAX_QUEUE_SIZE]; 
int queueSize = 0; 

void addToQueue(int floor) {
    if (queueSize < MAX_QUEUE_SIZE) {
        queue[queueSize].floor = floor;
        queueSize++;
    }
    else {
        printf("Очередь переполнена.\n");
    }
}

void processElevator(int* currentFloor, int* direction) {
    while (queueSize > 0) {
        for (int i = 0; i < queueSize; i++) {
            if (queue[i].floor == *currentFloor) {
                printf("Лифт на этаже %d, направление: %s.\n", queue[i].floor, (*direction == 1) ? "вверх" : "вниз");

                if (logCount < MAX_LOG_ENTRIES) {
                    logEntries[logCount].floor = queue[i].floor;
                    strcpy(logEntries[logCount].direction, (*direction == 1) ? "вверх" : "вниз");
                    logCount++;
                }

                for (int j = i; j < queueSize - 1; j++) {
                    queue[j] = queue[j + 1];
                }
                queueSize--;
                i--;
            }
        }

        int hasCallsAbove = 0;
        int hasCallsBelow = 0;

        for (int i = 0; i < queueSize; i++) {
            if (queue[i].floor > *currentFloor) {
                hasCallsAbove = 1;
            }
            if (queue[i].floor < *currentFloor) {
                hasCallsBelow = 1;
            }
        }
        if (!hasCallsAbove && *direction == 1) {
            *direction = -1; 
        }
        else if (!hasCallsBelow && *direction == -1) {
            *direction = 1; 
        }

        // Обновляем текущий этаж в зависимости от направления
        *currentFloor += *direction;

        // Проверяем границы этажа
        if (*currentFloor > MAX_FLOOR) {
            *currentFloor = MAX_FLOOR;
            *direction = -1; // Меняем направление на вниз
        }
        else if (*currentFloor < MIN_FLOOR) {
            *currentFloor = MIN_FLOOR;
            *direction = 1; // Меняем направление на вверх
        }

        // Имитация задержки для подъема/спуска лифта
        printf("Лифт перемещается на этаж %d...\n", *currentFloor);
    }
}

// Функция для генерации тестовых данных без направления
void generateTestData() {
    FILE* file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Ошибка при открытии файла для записи.\n");
        return;
    }

    srand(time(NULL)); // Инициализация генератора случайных чисел

    for (int i = 0; i < NUM_TEST_CASES; i++) {
        int floor = rand() % (MAX_FLOOR - MIN_FLOOR + 1) + MIN_FLOOR; // Случайный этаж

        fprintf(file, "%d\n", floor); // Запись только этажа в файл
    }

    fclose(file);
    printf("Тестовые данные сгенерированы и сохранены в файл '%s'.\n", FILENAME);
}

// Функция для загрузки данных из файла в очередь
void loadQueueFromFile() {
    FILE* file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("Ошибка при открытии файла для чтения.\n");
        return;
    }

    int floor;

    while (fscanf(file, "%d", &floor) != EOF) { // Чтение только этажа без направления
        addToQueue(floor); // Добавляем вызовы в очередь
    }

    fclose(file);
}

// Функция для записи лога в файл
void writeLogToFile() {
    FILE* file = fopen(LOGFILE, "w");
    if (file == NULL) {
        printf("Ошибка при открытии файла для записи лога.\n");
        return;
    }

    for (int i = 0; i < logCount; i++) {
        fprintf(file, "%d %s\n", logEntries[i].floor, logEntries[i].direction); // Запись этажа и направления
    }

    fclose(file);
    printf("Данные о работе системы записаны в файл '%s'.\n", LOGFILE);
}

int main() {
    setlocale(LC_ALL, "RUS");
    int choice;

    while (1) {
        printf("\nВыберите режим работы:\n");
        printf("1. Сгенерировать тестовые данные\n");
        printf("2. Загрузить данные из файла и запустить лифт\n");
        printf("3. Записать данные о работе системы в файл\n");
        printf("4. Выйти\n");

        scanf("%d", &choice);

        switch (choice) {
        case 1:
            generateTestData(); // Генерация тестовых данных без направления
            break;
        case 2: {
            int currentFloor = rand() % (MAX_FLOOR - MIN_FLOOR + 1) + MIN_FLOOR; // Начинаем с случайного этажа
            int direction = 1; // Начальное направление вверх

            loadQueueFromFile(); // Загружаем данные в очередь

            printf("Лифт начинает с этажа %d.\n", currentFloor);
            processElevator(&currentFloor, &direction); // Запускаем процесс лифта

            break;
        }
        case 3:
            writeLogToFile();
            break;
        case 4:
            printf("Выход из программы.\n");
            return 0;
        default:
            printf("Неверный выбор. Пожалуйста, попробуйте снова.\n");
            break;
        }
    }

    return 0;
}