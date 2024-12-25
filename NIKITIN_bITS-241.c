#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#define MIN_FLOOR 1  // Минимальный этаж
#define MAX_FLOOR 10 // Максимальный этаж
#define INITIAL_COUNT 0 // Начальное значение счетчика
#define INITIAL_COUNT_UP 0 // Начальное значение счетчика вверх
#define INITIAL_COUNT_DOWN 0 // Начальное значение счетчика вниз
#define INITIAL_COUNT_DATA 0 // Cчетчик прочитанных данных
#define MAX_REQUESTS 100 // Максимальное количество запросов

// Структура для хранения информации о вызове лифта
typedef struct {
    int floor; // Этаж вызова
    int direction; // Направление
} ElevatorCall;

// Структура для хранения запросов
typedef struct {
    int request_up[MAX_REQUESTS]; // Массив для хранения этажей запросов вверх
    int request_down[MAX_REQUESTS]; // Массив для хранения этажей запросов вниз
    int upCount; // Количество запросов на подъем
    int downCount; // Количество запросов на спуск
} RequestData;

/**
 * Функция считывает данные от пользователя.
 *
 * @param calls Указатель на массив, в который будут записаны введенные данные.
 * @return Количество успешно введенных вызовов.
 */
int readUserInput(ElevatorCall* calls);

/**
 * Функция отображает данные о вызовах лифта.
 *
 * @param calls Указатель на массив, содержащий данные о вызовах.
 * @param count Количество вызовов, которые необходимо отобразить.
 */
void displayInput(ElevatorCall* calls, int count);

/**
 * Функция записывает данные о вызовах лифта в файл.
 *
 * @param calls Указатель на массив, содержащий данные о вызовах.
 * @param count Количество вызовов, которые необходимо записать в файл.
 */
void writeToFile(ElevatorCall* calls, int count);

/**
 * Функция генерирует случайные вызовы лифта.
 *
 * @param calls Указатель на массив, в который будут записаны сгенерированные данные.
 * @param count Количество случайных вызовов, которые необходимо сгенерировать.
 */
void generateRandomCalls(ElevatorCall* calls, int count);

/**
 * Функция читает данные из файла.
 *
 * @param totalCount Указатель на переменную, в которую будет записано количество прочитанных данных.
 * @return Указатель на массив целых чисел, содержащий данные о вызовах лифта.
 */
int* readFromFile(int* totalCount);

/**
 * Функция распределяет запросы по массивам запросов.
 *
 * @param total Указатель на массив целых чисел, содержащий данные о вызовах лифта.
 * @param totalCount Количество элементов в массиве total.
 * @param requests Указатель на структуру RequestData, в которую будут записаны распределенные запросы.
 */
void selectRequests(int* total, int totalCount, RequestData* requests);

/**
 * Функция отображает запросы.
 *
 * @param requests Указатель на структуру RequestData, содержащую запросы лифта.
 */
void displayRequests(const RequestData* requests);

/**
 * Функция обрабатывает запросы лифта.
 *
 * @param request_up Указатель на массив целых чисел с запросами на подъем.
 * @param upCount Количество запросов на подъем.
 * @param request_down Указатель на массив целых чисел с запросами на спуск.
 * @param downCount Количество запросов на спуск.
 */
void processRequests(int* request_up, int upCount, int* request_down, int downCount);

/**
 * Функция сортирует массив запросов.
 *
 * @param requests Указатель на массив целых чисел с запросами для сортировки.
 * @param count Количество элементов в массиве requests.
 * @param compare Указатель на функцию сравнения для определения порядка сортировки (возрастание или убывание).
 */
void sortRequests(int* requests, int count, int (*compare)(int a, int b));

/**
 * Функция анализирует данные о вызовах лифта.
 *
 * @param total Указатель на массив целых чисел с данными о вызовах лифта.
 * @param totalCount Количество элементов в массиве total.
 */
void analyzeData(int* total, int totalCount);

/**
 * Функция сравнения для сортировки по возрастанию.
 *
 * @param a Первое значение для сравнения.
 * @param b Второе значение для сравнения.
 * @return Отрицательное значение, если a < b; 0 если a == b; положительное значение если a > b.
 */
int compareAscending(int a, int b);

/**
 * Функция сравнения для сортировки по убыванию.
 *
 * @param a Первое значение для сравнения.
 * @param b Второе значение для сравнения.
 * @return Отрицательное значение, если a < b; 0 если a == b; положительное значение если a > b.
 */
int compareDescending(int a, int b);



int main() {
    setlocale(LC_ALL, "RUS");
    system("chcp 1251");
    srand(time(NULL)); // Инициализация генератора случайных чисел

    // Приветсвие пользователя (шапка программы)
    puts("*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*");
    printf("\n\t  Добро пожаловать в программу управления лифтом!\n");
    printf("\tНазначение: отслеживание и обработка запросов лифта.\n\n");
    puts("*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*");

    int monitoringMode = 0;
    char choice;

    ElevatorCall calls[MAX_REQUESTS]; // Массив для хранения вызовов

    do {
        // Главное меню
        printf("\nВыберите действие:\n");
        printf("1. Включить режим мониторинга\n");
        printf("2. Выключить режим мониторинга\n");
        printf("3. Ручной ввод данных\n");
        printf("4. Случайно сгенерированный ввод данных\n");
        printf("5. Вывести запросы\n");
        printf("6. Обработать запросы лифта\n"); // Новый пункт меню
        printf("7. Провести анализ данных\n"); // Новый пункт меню
        printf("8. Выход\n");
        printf("Ваш выбор: ");
        scanf(" %c", &choice);

        switch (choice) {
        case '1': // Включение мониторинга
            monitoringMode = 1;
            printf("Режим мониторинга включен.\n");
            break;
        case '2': // Выключение мониторинга
            monitoringMode = 0;
            printf("Режим мониторинга выключен.\n");
            break;
        case '3': // Ручной ввод данных
            if (monitoringMode) {
                int count = readUserInput(calls); // Считывание данных от пользователя
                displayInput(calls, count); // Вывод введенных данных
                writeToFile(calls, count); // Запись данных в файл
            }
            else {
                printf("Режим мониторинга выключен. Включите его для ввода данных.\n");
            }
            break;
        case '4': { // Генерация данных
            if (monitoringMode) {
                int count; // Считывание данных от пользователя
                printf("Введите количество случайных вызовов для генерации: ");
                scanf("%d", &count);

                if (count > MAX_REQUESTS) { // Проверка на превышение максимального количества запросов
                    printf("Ошибка: Максимальное количество вызовов - %d.\n", MAX_REQUESTS);
                    break;
                }

                generateRandomCalls(calls, count); // Генерация случайных вызовов
                displayInput(calls, count); // Вывод введенных данных
                writeToFile(calls, count); // Запись данных в файл
            }
            else {
                printf("Режим мониторинга выключен. Включите его для генерации данных.\n");
            }
            break;
        }
        case '5':
            if (monitoringMode) {
                int totalCount; // Массив запросов
                int* total = readFromFile(&totalCount); // Чтение данных из файла

                if (total != NULL) {
                    RequestData requests = { .upCount = 0, .downCount = 0 }; // Инициализация структуры запросов

                    selectRequests(total, totalCount, &requests); // Распределение запросов
                    displayRequests(&requests); // Вывод запросов

                    free(total); // Освобождение памяти
                }
            }
            else {
                printf("Режим мониторинга выключен. Включите его для отображения запросов.\n");
            }
            break;
        case '6':
            if (monitoringMode) {
                int totalCount; // Массив запросов
                int* total = readFromFile(&totalCount); // Чтение данных из файла

                if (total != NULL) {
                    RequestData requests = { .upCount = 0, .downCount = 0 }; // Инициализация структуры запросов

                    selectRequests(total, totalCount, &requests); // Распределение запросов

                    sortRequests(requests.request_up, requests.upCount, compareAscending);   // Сортируем по возрастанию для вверх
                    sortRequests(requests.request_down, requests.downCount, compareDescending); // Сортируем по убыванию для вниз

                    processRequests(requests.request_up, requests.upCount, requests.request_down, requests.downCount); // Обработка запросов

                    free(total); // Освобождение памяти
                }
            }
            else {
                printf("Режим мониторинга выключен. Включите его для обработки запросов лифта.\n");
            }
            break;
        case '7':
            if (monitoringMode) {
                int totalCount; // Массив запросов
                int* total = readFromFile(&totalCount); // Чтение данных из файла

                if (total != NULL) {
                    analyzeData(total, totalCount); // Анализ данных о вызовах лифта
                    free(total); // Освобождение памяти
                }
            }
            else {
                printf("Режим мониторинга выключен. Включите его для анализа данных.\n");
            }
            break;
        case '8':
            printf("Выход из программы.\n");
            break;
        default:
            printf("Неверный выбор. Пожалуйста, попробуйте снова.\n");
        }

    } while (choice != '8');

    return 0;
}

int readUserInput(ElevatorCall* calls) {
    int count = INITIAL_COUNT; // Счетчик вызовов
    int inputFloor; // Введённый этаж

    printf("Введите этаж вызова (например, -2 для вызова вниз со второго этажа), или 0 для завершения ввода:\n");

    while (count < MAX_REQUESTS) { // Ограничение на количество вводимых запросов
        printf("Этаж вызова: ");
        scanf("%d", &inputFloor);

        if (inputFloor == -1) { // Проверка на спуск с первого этажа 
            printf("С этажа %d можно вызвать лифт только вверх.\n", MIN_FLOOR);
            continue;
        }
        else if (inputFloor == MAX_FLOOR) { // Проверка на подъём с десятого этажа 
            printf("С этажа %d можно вызвать лифт только вниз.\n", MAX_FLOOR);
            continue;
        }

        if (inputFloor == 0) { // Завершение ввода, если введено 0
            printf("Ручной ввод данных завершён.\n");
            break; 
        }

        if (abs(inputFloor) < MIN_FLOOR || abs(inputFloor) > MAX_FLOOR) { // Проверка диапазона
            printf("Ошибка: Этаж должен быть в диапазоне [%d, %d].\n", MIN_FLOOR, MAX_FLOOR);
            continue; 
        }

        calls[count].floor = abs(inputFloor); // Модуль числа определяет этаж
        calls[count].direction = (inputFloor > 0) ? 1 : -1; // Определение направления по знаку числа
        count++; // Увеличение счетчика вызовов
    }

    return count;
}

void displayInput(ElevatorCall* calls, int count) {
    for (int i = 0; i < count; i++) {
        if (calls[i].direction > 0) { 
            printf("Вызов с этажа %d. Направление: Вверх.\n", calls[i].floor);
        }
        else {
            printf("Вызов с этажа %d. Направление: Вниз.\n", calls[i].floor);
        }
    }
    return 0;
}

void writeToFile(ElevatorCall* calls, int count) {
    FILE* file = fopen("test_data.txt", "a");
    if (file == NULL) {
        return -1;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d\n", (calls[i].direction > 0 ? calls[i].floor : -calls[i].floor));
    }

    fclose(file);
    return 0;
}

void generateRandomCalls(ElevatorCall* calls, int count) {
    for (int i = 0; i < count; i++) {
        int floor = rand() % (MAX_FLOOR - MIN_FLOOR + 1) + MIN_FLOOR; // Генерация этажа от MIN_FLOOR до MAX_FLOOR
        int direction = (rand() % 2) == 0 ? 1 : -1; // Генерация направления случайным образом

        calls[i].floor = floor; // Сохранение сгенерированныго этажа в массив вызовов
        calls[i].direction = direction; // Сохранение сгенерированного направления в массив вызовов
    }
    return 0;
}

int* readFromFile(int* totalCount) {
    FILE* file = fopen("test_data.txt", "r");
    if (file == NULL) {
        return -1;
    }
    
    int* total = malloc(MAX_REQUESTS * sizeof(int)); // Динамическое выделение памяти под массив total
    if (total == NULL) {
        fclose(file);
        return -1;
    }

    *totalCount = INITIAL_COUNT_DATA; // Инициализация счетчика прочитанных данных

    while (*totalCount < MAX_REQUESTS && fscanf(file, "%d", &total[*totalCount]) == 1) { // Увеличение счетчика на единицу при успешном чтении числа из файла
        (*totalCount)++; 
    }

    fclose(file);

    return total;
}

void selectRequests(int* total, int totalCount, RequestData* requests) {
    for (int i = 0; i < totalCount; i++) {
        if (total[i] > 0) { // Проверка, является ли вызов вверх
            requests->request_up[requests->upCount++] = total[i]; // Добавляем вызов вверх в массив request_up и увеличиваем счетчик
        }
        else { // В противном случае это вызов вниз
            requests->request_down[requests->downCount++] = -total[i]; // Добавляем модуль вызова вниз в массив request_down и увеличиваем счетчик
        }
    }
}

void displayRequests(const RequestData* requests) {
    printf("\nЗапросы вверх:\n");
    for (int i = 0; i < requests->upCount; i++) {
        printf("%d ", requests->request_up[i]);
    }

    printf("\nЗапросы вниз:\n");
    for (int i = 0; i < requests->downCount; i++) {
        printf("%d ", requests->request_down[i]);
    }

    printf("\n");
    return 0;
}

int compareAscending(int a, int b) {
    return a - b; // Возвращает разницу между a и b
}

int compareDescending(int a, int b) {
    return b - a; // Возвращает разницу между b и a
}

void sortRequests(int* requests, int count, int (*compare)(int a, int b)) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (compare(requests[i], requests[j]) > 0) { // Использование функции сравнения для определения порядка
                int temp = requests[i]; // Сохранение текущего элемента во временной переменной
                requests[i] = requests[j]; // Замена местами элементов, если они не в правильном порядке
                requests[j] = temp;
            }
        }
    }
    return 0;
}

void processRequests(int* request_up, int upCount, int* request_down, int downCount) {
    int current_floor = MIN_FLOOR;

    // Обработка запросов вверх
    for (int i = 0; i < upCount; i++) {
        while (current_floor < request_up[i]) { // Подъём до запрашиваемого этажа
            current_floor++; // Увеличение текущего этажа
            printf("Лифт поднимается на этаж %d ...\n", current_floor);
        }
        if (current_floor == request_up[i]) { // Проверка с запрашиваемым этажом
            printf("Лифт остановился на этаже %d\n", current_floor);
        }
    }

    // Проверка необходимости подняться до вызова "вниз"
    int highest_floor; // Этаж до которого нужно подяться
    if (downCount > 0) { // Проверка наличия запросов вниз
        highest_floor = request_down[0]; // Взятие самый верхнего этажа из запросов вниз
    }
    else {
        highest_floor = request_up[upCount - 1]; // Взятие самого высокого этажа из запросов вверх 
    }

    while (current_floor < highest_floor) { // Подъём до этажа выше текущего
        current_floor++;
        printf("Лифт поднимается на этаж %d ...\n", current_floor);
    }

    // Обработка запросов вниз
    for (int i = 0; i < downCount; i++) {
        while (current_floor > request_down[i]) { // Спукс до запрашиваемого этажа
            current_floor--;
            printf("Лифт спускается на этаж %d ...\n", current_floor);
        }
        if (current_floor == request_down[i]) { // Проверка с запрашиваемым этажом
            printf("Лифт остановился на этаже %d\n", current_floor);
        }
    }
    return 0;
}

void analyzeData(int* total, int totalCount) {
    if (total == NULL || totalCount <= 0) {
        printf("Ошибка: Данные отсутствуют или введы неверно");
    }

    int upCalls = INITIAL_COUNT_UP; // Счетчик вызовов вверх
    int downCalls = INITIAL_COUNT_DOWN; // Счетчик вызовов вниз
    int callCounts[MAX_FLOOR + 1] = { 0 }; // Массив для подсчета вызовов на каждом этаже

    for (int i = 0;i < totalCount;i++) {
        if (total[i] > 0) {
            upCalls++;
        }
        else {
            downCalls++;
        }
        callCounts[abs(total[i])]++; // Увеличение счетчика для соответствующего этажа 
    }

    int mostFrequentFloor = MIN_FLOOR; // Самый часто вызываемый этаж
    for (int floor = MIN_FLOOR; floor <= MAX_FLOOR; floor++) {
        if (callCounts[floor] > callCounts[mostFrequentFloor]) { 
            mostFrequentFloor = floor;
        }
    }

    printf("\nАнализ данных:\n");
    printf("Всего вызовов: %d\n", totalCount);
    printf("Вызовы вверх: %d\n", upCalls);
    printf("Вызовы вниз: %d\n", downCalls);
    printf("Самый часто вызываемый этаж: %d\n", mostFrequentFloor);

    return 0;
}
