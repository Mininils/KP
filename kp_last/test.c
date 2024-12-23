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

void monitorElevator(ElevatorCall call);
void logElevatorData(ElevatorCall call);
void processRequest(int direction, int request_up[], int* up_count, int request_down[], int* down_count);
void bubbleSort(int arr[], int n, int ascending);
void generateRandomCalls(int count, int request_up[], int* up_count, int request_down[], int* down_count);
void readTestData(int request_up[], int* up_count, int request_down[], int* down_count);
void printRequests(int request_up[], int up_count, int request_down[], int down_count);
void processElevatorRequests(int request_up[], int up_count, int request_down[], int down_count);
void printAnalysis(int request_up[], int up_count, int request_down[], int down_count);



int main() {
    setlocale(LC_ALL, "RUS"); 
    system("chcp 1251");
    srand(time(NULL));          
    
    puts("*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*");
    printf("\nДобро пожаловать в программу управления лифтом!\n");
    printf("Назначение: отслеживание и обработка запросов лифта.\n\n");
    puts("*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*");

    ElevatorCall userCall;   
    char choice;
    int monitoringEnabled = 0;

    int request_up[MAX_CALLS];
    int up_count = 0;
    int request_down[MAX_CALLS];
    int down_count = 0;

    do {
        readTestData(request_up, &up_count, request_down, &down_count);

        printf("\nВыберите действие:\n");
        printf("1. Включить режим мониторинга\n");
        printf("2. Выключить режим мониторинга\n");
        printf("3. Ручной ввод данных\n");
        printf("4. Случайно сгенерированный ввод данных\n");
        printf("5. Вывести запросы\n");
        printf("6. Обработать запросы лифта\n");
        printf("7. Провести анализ данных\n");
        printf("8. Выход\n");
        printf("Ваш выбор: ");
        scanf(" %c", &choice);

        switch (choice) {
        case '1':
            readTestData(request_up, &up_count, request_down, &down_count);
            monitoringEnabled = 1;
            printf("Режим мониторинга включен.\n");
            readTestData(request_up, &up_count, request_down, &down_count);
            break;

        case '2':
            readTestData(request_up, &up_count, request_down, &down_count);
            monitoringEnabled = 0;
            printf("Режим мониторинга выключен.\n");
            readTestData(request_up, &up_count, request_down, &down_count);
            break;

        case '3':
            readTestData(request_up, &up_count, request_down, &down_count);
            if (monitoringEnabled) {
                printf("Введите этаж вызова и направление (например: -2 для вниз): ");
                scanf("%d", &userCall.direction);
                userCall.floor = abs(userCall.direction);
                monitorElevator(userCall);
                logElevatorData(userCall);

                processRequest(userCall.direction, request_up, &up_count, request_down, &down_count);
            }
            else {
                printf("Режим мониторинга выключен. Включите его для ввода данных.\n");
            }
            readTestData(request_up, &up_count, request_down, &down_count);
            break;

        case '4':
            readTestData(request_up, &up_count, request_down, &down_count);
            if (monitoringEnabled) {
                int callCount;
                printf("Введите количество случайных вызовов: ");
                scanf("%d", &callCount);
                if (callCount > 0 && callCount <= MAX_CALLS) {
                    generateRandomCalls(callCount, request_up, &up_count, request_down, &down_count);
                }
                else {
                    printf("Ошибка: Количество вызовов должно быть от 1 до %d.\n", MAX_CALLS);
                }
            }
            else {
                printf("Режим мониторинга выключен. Включите его для генерации данных.\n");
            }
            readTestData(request_up, &up_count, request_down, &down_count);
            break;

        case '5':
            printRequests(request_up, up_count, request_down, down_count);
            readTestData(request_up, &up_count, request_down, &down_count);
            break;

        case '6':
            readTestData(request_up, &up_count, request_down, &down_count);
            processElevatorRequests(request_up, up_count, request_down, down_count);
            readTestData(request_up, &up_count, request_down, &down_count);
            break;

        case '7':
            printAnalysis(request_up, up_count, request_down, down_count);
            readTestData(request_up, &up_count, request_down, &down_count);
            break;

        case '8':
            printf("Выход из программы.\n");
            break;

        default:
            printf("Ошибка: Неверный выбор. Попробуйте снова.\n");
            break;
        }

    } while (choice != '8');

    return 0;
}

/*Отслеживает вызов лифта, проверяя, находится ли этаж в допустимом диапазоне.*/
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

/*Записывает данные о вызове лифта в файл.*/
void logElevatorData(ElevatorCall call) {
    FILE* file = fopen("elevator_log.txt", "a");
    if (file == NULL) {
        printf("Ошибка: Не удалось открыть файл для записи.\n");
        return;
    }

    fprintf(file, "Этаж: %d, Направление: %s\n", abs(call.floor), call.direction > 0 ? "вверх" : "вниз");
    fclose(file);
}

/*Отслеживает вызов лифта, проверяя, находится ли этаж в допустимом диапазоне.*/
void processRequest(int direction, int request_up[], int* up_count, int request_down[], int* down_count) {
    if (direction > 0) {
        request_up[(*up_count)++] = direction;
    }
    else {
        request_down[(*down_count)++] = abs(direction);
    }
}

/*Отслеживает вызов лифта, проверяя, находится ли этаж в допустимом диапазоне.*/
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

/*Отслеживает вызов лифта, проверяя, находится ли этаж в допустимом диапазоне.*/
void generateRandomCalls(int count, int request_up[], int* up_count, int request_down[], int* down_count) {
    FILE* testDataFile = fopen("test_data.txt", "a");
    if (testDataFile == NULL) {
        printf("Ошибка: Не удалось открыть файл test_data.txt для записи.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        ElevatorCall call;

        // генерация направления
        int direction = (rand() % 2 == 0) ? 1 : -1; // 1 вверх, -1 вниз

        // генерация этажа в зависимости от направления (нет -1 и 10)
        if (direction == 1) { 
            call.floor = rand() % (MAX_FLOOR - MIN_FLOOR) + MIN_FLOOR;
            if (call.floor == MAX_FLOOR) { 
                call.floor = MAX_FLOOR - 1;
            }
        }
        else { // вниз
            call.floor = rand() % (MAX_FLOOR - MIN_FLOOR + 1) + MIN_FLOOR + 1;
            if (call.floor == MIN_FLOOR) {
                call.floor = MIN_FLOOR + 1;
            }
        }

        call.direction = direction * call.floor;

        fprintf(testDataFile, "%d\n", call.direction);

        monitorElevator(call);
        logElevatorData(call);

        processRequest(call.direction, request_up, up_count, request_down, down_count);
    }

    fclose(testDataFile);
}

/*Отслеживает вызов лифта, проверяя, находится ли этаж в допустимом диапазоне.*/
void readTestData(int request_up[], int* up_count, int request_down[], int* down_count) {
    FILE* file = fopen("test_data.txt", "r");
    if (file == NULL) {
        printf("Ошибка: Не удалось открыть файл test_data.txt для чтения.\n");
        return;
    }
   
    *up_count = *down_count = 0;

    int value;
    while (fscanf(file, "%d", &value) == 1) {
        processRequest(value, request_up, up_count, request_down, down_count);
    }

    fclose(file);
}

/*Отслеживает вызов лифта, проверяя, находится ли этаж в допустимом диапазоне.*/
void printRequests(int request_up[], int up_count, int request_down[], int down_count) {
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

void printAnalysis(int request_up[], int up_count, int request_down[], int down_count) {
    int total_calls = up_count + down_count;
    int call_count[MAX_FLOOR + 1] = {0};

    // подсчёт вызовов
    for (int i = 0; i < up_count; i++) {
        call_count[request_up[i]]++;
    }
    for (int i = 0; i < down_count; i++) {
        call_count[request_down[i]]++;
    }

    // самый частый
    int most_called_floor = MIN_FLOOR;
    int max_calls = 0;
    for (int i = MIN_FLOOR; i <= MAX_FLOOR; i++) {
        if (call_count[i] > max_calls) {
            max_calls = call_count[i];
            most_called_floor = i;
        }
    }

    printf("\nАнализ данных:\n");
    printf("Всего вызовов: %d\n", total_calls);
    printf("Вверх: %d, Вниз: %d\n", up_count, down_count);
    printf("Часто вызываемый этаж: %d (%d раз)\n", most_called_floor, max_calls);
}

/*Отслеживает вызов лифта, проверяя, находится ли этаж в допустимом диапазоне.*/
void processElevatorRequests(int request_up[], int up_count, int request_down[], int down_count) {
    int current_floor = MIN_FLOOR;

    bubbleSort(request_up, up_count, 1);
    bubbleSort(request_down, down_count, 0);

    // вверх
    for (int i = 0; i < up_count; i++) {
        while (current_floor < request_up[i]) {
            current_floor++;
            printf("Лифт поднимается на этаж %d ...\n", current_floor);
        }
        if (current_floor == request_up[i]) {
            printf("Лифт остановился на этаже %d\n", current_floor);
        }
    }

    // проверка необходимости подняться до вызова "вниз"
    int highest_floor = (down_count > 0) ? request_down[0] : request_up[up_count - 1];
    while (current_floor < highest_floor) {
        current_floor++;
        printf("Лифт поднимается на этаж %d ...\n", current_floor);
    }

    // вниз
    for (int i = 0; i < down_count; i++) {
        while (current_floor > request_down[i]) {
            current_floor--;
            printf("Лифт спускается на этаж %d ...\n", current_floor);
        }
        if (current_floor == request_down[i]) {
            printf("Лифт остановился на этаже %d\n", current_floor);
        }
    }
}
