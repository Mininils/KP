#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#define MIN_FLOOR 1
#define MAX_FLOOR 10
#define MAX_CALLS 100

typedef struct {
    int floor;      
    int direction;  
} ElevatorCall;

int request_up[MAX_CALLS];
int request_down[MAX_CALLS];
int up_count = 0;
int down_count = 0;

void monitorElevator(ElevatorCall call) {
    if (call.floor < MIN_FLOOR || call.floor > MAX_FLOOR) {
        printf("Ошибка: Этаж %d вне допустимого диапазона (%d-%d).\n", call.floor, MIN_FLOOR, MAX_FLOOR);
        return;
    }
    if (call.direction > 0) {
        printf("Вызов с этажа %d вверх.\n", call.floor);
    }
    else {
        printf("Вызов с этажа %d вниз.\n", call.floor);
    }
}

void logElevatorData(ElevatorCall call) {
    FILE* file = fopen("elevator_log.txt", "a");
    if (file == NULL) {
        printf("Ошибка: Не удалось открыть файл для записи.\n");
        return;
    }

    fprintf(file, "Этаж: %d, Направление: %s\n", abs(call.floor), call.direction > 0 ? "вверх" : "вниз");
    fclose(file);
}

void processRequest(int direction) {
    if (direction > 0) {
        request_up[up_count++] = direction;
    }
    else {
        request_down[down_count++] = abs(direction);
    }
}

void bubbleSort(int arr[], int n, int ascending) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if ((ascending && arr[j] > arr[j + 1]) || (!ascending && arr[j] < arr[j + 1])) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void generateRandomCalls(int count) {
    FILE* testDataFile = fopen("test_data.txt", "a");
    if (testDataFile == NULL) {
        printf("Ошибка: Не удалось открыть файл test_data.txt для записи.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        ElevatorCall call;

        call.floor = rand() % (MAX_FLOOR - MIN_FLOOR + 1) + MIN_FLOOR;
        call.direction = (rand() % 2 == 0) ? -call.floor : call.floor;

        fprintf(testDataFile, "%d\n", call.direction);

        monitorElevator(call);
        logElevatorData(call);

        processRequest(call.direction);
    }

    fclose(testDataFile);
}

void printRequests() {
    printf("\nЗапросы вверх:\n");
    for (int i = 0; i < up_count; i++) {
        printf("%d ", request_up[i]);
    }

    printf("\nЗапросы вниз:\n");
    for (int i = 0; i < down_count; i++) {
        printf("%d ", request_down[i]);
    }
    printf("\n");
}

void processElevatorRequests() {
    int current_floor = MIN_FLOOR;

    bubbleSort(request_up, up_count, 1);   
    bubbleSort(request_down, down_count, 0);

    // Запросы вверх
    for (int i = 0; i < up_count; i++) {
        while (current_floor < request_up[i]) {
            current_floor++;
            printf("Лифт поднимается на этаж %d ...\n", current_floor);
        }
        if (current_floor == request_up[i]) {
            printf("Лифт остановился на этаже %d\n", current_floor);
        }
    }

    // Проверка необходимости подняться до самого верхнего этажа перед спуском
    if (down_count > 0 && request_down[0] > request_up[up_count - 1]) {
        int highest_up = request_up[up_count - 1];
        while (current_floor < request_down[0]) { // Поднимаемся до самого высокого этажа
            current_floor++;
            printf("Лифт поднимается на этаж %d ...\n", current_floor);
        }
        printf("Лифт остановился на этаже %d\n", current_floor);
    }

    // Запросы вниз
    for (int i = down_count - 1; i >= 0; i--) {
        while (current_floor > request_down[i]) {
            current_floor--;
            printf("Лифт спускается на этаж %d\n", current_floor);
        }
        if (current_floor == request_down[i]) {
            printf("Лифт остановился на этаже %d\n", current_floor);
        }
    }
}

int main() {
    setlocale(LC_ALL, "RUS");
    srand(time(NULL));

    ElevatorCall userCall;
    char choice;
    int monitoringEnabled = 0;

    do {
        printf("\nВыберите действие:\n");
        printf("1. Ручной ввод данных\n");
        printf("2. Случайно сгенерированный ввод данных\n");
        printf("3. Включить режим мониторинга\n");
        printf("4. Выключить режим мониторинга\n");
        printf("5. Вывести запросы\n");
        printf("6. Обработать запросы лифта\n");
        printf("7. Выход\n");
        printf("Ваш выбор: ");
        scanf(" %c", &choice);

        switch (choice) {
        case '1':
            if (monitoringEnabled) {
                printf("Введите этаж вызова и направление (например: -2 для вниз): ");
                scanf("%d", &userCall.direction);
                userCall.floor = abs(userCall.direction);
                monitorElevator(userCall);
                logElevatorData(userCall);

                processRequest(userCall.direction);
            }
            else {
                printf("Режим мониторинга выключен. Включите его для ввода данных.\n");
            }
            break;

        case '2':
            if (monitoringEnabled) {
                int callCount;
                printf("Введите количество случайных вызовов: ");
                scanf("%d", &callCount);
                if (callCount > 0 && callCount <= MAX_CALLS) {
                    generateRandomCalls(callCount);
                }
                else {
                    printf("Ошибка: Количество вызовов должно быть от 1 до %d.\n", MAX_CALLS);
                }
            }
            else {
                printf("Режим мониторинга выключен. Включите его для генерации данных.\n");
            }
            break;

        case '3':
            monitoringEnabled = 1;
            printf("Режим мониторинга включен.\n");
            break;

        case '4':
            monitoringEnabled = 0;
            printf("Режим мониторинга выключен.\n");
            break;

        case '5':
            printRequests();
            break;

        case '6':
            processElevatorRequests();
            break;

        case '7':
            printf("Выход из программы.\n");
            break;

        default:
            printf("Ошибка: Неверный выбор. Попробуйте снова.\n");
            break;
        }
    } while (choice != '7');

    return 0;
}
