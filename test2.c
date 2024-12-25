#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#define MIN_FLOOR 1  // ����������� ����
#define MAX_FLOOR 10 // ������������ ����
#define INITIAL_COUNT 0 // ��������� �������� ��������
#define INITIAL_COUNT_UP 0 // ��������� �������� �������� �����
#define INITIAL_COUNT_DOWN 0 // ��������� �������� �������� ����
#define INITIAL_COUNT_DATA 0 // C������ ����������� ������
#define MAX_REQUESTS 100 // ������������ ���������� ��������

// ��������� ��� �������� ���������� � ������ �����
typedef struct {
    int floor; // ���� ������
    int direction; // �����������
} ElevatorCall;

// ��������� ��� �������� ��������
typedef struct {
    int request_up[MAX_REQUESTS]; // ������ ��� �������� ������ �������� �����
    int request_down[MAX_REQUESTS]; // ������ ��� �������� ������ �������� ����
    int upCount; // ���������� �������� �� ������
    int downCount; // ���������� �������� �� �����
} RequestData;

/**
 * ������� ��������� ������ �� ������������.
 *
 * @param calls ��������� �� ������, � ������� ����� �������� ��������� ������.
 * @return ���������� ������� ��������� �������.
 */
int readUserInput(ElevatorCall* calls);

/**
 * ������� ���������� ������ � ������� �����.
 *
 * @param calls ��������� �� ������, ���������� ������ � �������.
 * @param count ���������� �������, ������� ���������� ����������.
 */
void displayInput(ElevatorCall* calls, int count);

/**
 * ������� ���������� ������ � ������� ����� � ����.
 *
 * @param calls ��������� �� ������, ���������� ������ � �������.
 * @param count ���������� �������, ������� ���������� �������� � ����.
 */
void writeToFile(ElevatorCall* calls, int count);

/**
 * ������� ���������� ��������� ������ �����.
 *
 * @param calls ��������� �� ������, � ������� ����� �������� ��������������� ������.
 * @param count ���������� ��������� �������, ������� ���������� �������������.
 */
void generateRandomCalls(ElevatorCall* calls, int count);

/**
 * ������� ������ ������ �� �����.
 *
 * @param totalCount ��������� �� ����������, � ������� ����� �������� ���������� ����������� ������.
 * @return ��������� �� ������ ����� �����, ���������� ������ � ������� �����.
 */
int* readFromFile(int* totalCount);

/**
 * ������� ������������ ������� �� �������� ��������.
 *
 * @param total ��������� �� ������ ����� �����, ���������� ������ � ������� �����.
 * @param totalCount ���������� ��������� � ������� total.
 * @param requests ��������� �� ��������� RequestData, � ������� ����� �������� �������������� �������.
 */
void selectRequests(int* total, int totalCount, RequestData* requests);

/**
 * ������� ���������� �������.
 *
 * @param requests ��������� �� ��������� RequestData, ���������� ������� �����.
 */
void displayRequests(const RequestData* requests);

/**
 * ������� ������������ ������� �����.
 *
 * @param request_up ��������� �� ������ ����� ����� � ��������� �� ������.
 * @param upCount ���������� �������� �� ������.
 * @param request_down ��������� �� ������ ����� ����� � ��������� �� �����.
 * @param downCount ���������� �������� �� �����.
 */
void processRequests(int* request_up, int upCount, int* request_down, int downCount);

/**
 * ������� ��������� ������ ��������.
 *
 * @param requests ��������� �� ������ ����� ����� � ��������� ��� ����������.
 * @param count ���������� ��������� � ������� requests.
 * @param compare ��������� �� ������� ��������� ��� ����������� ������� ���������� (����������� ��� ��������).
 */
void sortRequests(int* requests, int count, int (*compare)(int a, int b));

/**
 * ������� ����������� ������ � ������� �����.
 *
 * @param total ��������� �� ������ ����� ����� � ������� � ������� �����.
 * @param totalCount ���������� ��������� � ������� total.
 */
void analyzeData(int* total, int totalCount);

/**
 * ������� ��������� ��� ���������� �� �����������.
 *
 * @param a ������ �������� ��� ���������.
 * @param b ������ �������� ��� ���������.
 * @return ������������� ��������, ���� a < b; 0 ���� a == b; ������������� �������� ���� a > b.
 */
int compareAscending(int a, int b);

/**
 * ������� ��������� ��� ���������� �� ��������.
 *
 * @param a ������ �������� ��� ���������.
 * @param b ������ �������� ��� ���������.
 * @return ������������� ��������, ���� a < b; 0 ���� a == b; ������������� �������� ���� a > b.
 */
int compareDescending(int a, int b);



int main() {
    setlocale(LC_ALL, "RUS");
    system("chcp 1251");
    srand(time(NULL)); // ������������� ���������� ��������� �����

    // ���������� ������������ (����� ���������)
    puts("*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*");
    printf("\n\t  ����� ���������� � ��������� ���������� ������!\n");
    printf("\t����������: ������������ � ��������� �������� �����.\n\n");
    puts("*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*");

    int monitoringMode = 0;
    char choice;

    ElevatorCall calls[MAX_REQUESTS]; // ������ ��� �������� �������

    do {
        // ������� ����
        printf("\n�������� ��������:\n");
        printf("1. �������� ����� �����������\n");
        printf("2. ��������� ����� �����������\n");
        printf("3. ������ ���� ������\n");
        printf("4. �������� ��������������� ���� ������\n");
        printf("5. ������� �������\n");
        printf("6. ���������� ������� �����\n"); // ����� ����� ����
        printf("7. �������� ������ ������\n"); // ����� ����� ����
        printf("8. �����\n");
        printf("��� �����: ");
        scanf(" %c", &choice);

        switch (choice) {
        case '1': // ��������� �����������
            monitoringMode = 1;
            printf("����� ����������� �������.\n");
            break;
        case '2': // ���������� �����������
            monitoringMode = 0;
            printf("����� ����������� ��������.\n");
            break;
        case '3': // ������ ���� ������
            if (monitoringMode) {
                int count = readUserInput(calls); // ���������� ������ �� ������������
                displayInput(calls, count); // ����� ��������� ������
                writeToFile(calls, count); // ������ ������ � ����
            }
            else {
                printf("����� ����������� ��������. �������� ��� ��� ����� ������.\n");
            }
            break;
        case '4': { // ��������� ������
            if (monitoringMode) {
                int count; // ���������� ������ �� ������������
                printf("������� ���������� ��������� ������� ��� ���������: ");
                scanf("%d", &count);

                if (count > MAX_REQUESTS) { // �������� �� ���������� ������������� ���������� ��������
                    printf("������: ������������ ���������� ������� - %d.\n", MAX_REQUESTS);
                    break;
                }

                generateRandomCalls(calls, count); // ��������� ��������� �������
                displayInput(calls, count); // ����� ��������� ������
                writeToFile(calls, count); // ������ ������ � ����
            }
            else {
                printf("����� ����������� ��������. �������� ��� ��� ��������� ������.\n");
            }
            break;
        }
        case '5':
            if (monitoringMode) {
                int totalCount; // ������ ��������
                int* total = readFromFile(&totalCount); // ������ ������ �� �����

                if (total != NULL) {
                    RequestData requests = { .upCount = 0, .downCount = 0 }; // ������������� ��������� ��������

                    selectRequests(total, totalCount, &requests); // ������������� ��������
                    displayRequests(&requests); // ����� ��������

                    free(total); // ������������ ������
                }
            }
            else {
                printf("����� ����������� ��������. �������� ��� ��� ����������� ��������.\n");
            }
            break;
        case '6':
            if (monitoringMode) {
                int totalCount; // ������ ��������
                int* total = readFromFile(&totalCount); // ������ ������ �� �����

                if (total != NULL) {
                    RequestData requests = { .upCount = 0, .downCount = 0 }; // ������������� ��������� ��������

                    selectRequests(total, totalCount, &requests); // ������������� ��������

                    sortRequests(requests.request_up, requests.upCount, compareAscending);   // ��������� �� ����������� ��� �����
                    sortRequests(requests.request_down, requests.downCount, compareDescending); // ��������� �� �������� ��� ����

                    processRequests(requests.request_up, requests.upCount, requests.request_down, requests.downCount); // ��������� ��������

                    free(total); // ������������ ������
                }
            }
            else {
                printf("����� ����������� ��������. �������� ��� ��� ��������� �������� �����.\n");
            }
            break;
        case '7':
            if (monitoringMode) {
                int totalCount; // ������ ��������
                int* total = readFromFile(&totalCount); // ������ ������ �� �����

                if (total != NULL) {
                    analyzeData(total, totalCount); // ������ ������ � ������� �����
                    free(total); // ������������ ������
                }
            }
            else {
                printf("����� ����������� ��������. �������� ��� ��� ������� ������.\n");
            }
            break;
        case '8':
            printf("����� �� ���������.\n");
            break;
        default:
            printf("�������� �����. ����������, ���������� �����.\n");
        }

    } while (choice != '8');

    return 0;
}

int readUserInput(ElevatorCall* calls) {
    int count = INITIAL_COUNT; // ������� �������
    int inputFloor; // �������� ����

    printf("������� ���� ������ (��������, -2 ��� ������ ���� �� ������� �����), ��� 0 ��� ���������� �����:\n");

    while (count < MAX_REQUESTS) { // ����������� �� ���������� �������� ��������
        printf("���� ������: ");
        scanf("%d", &inputFloor);

        if (inputFloor == -1) { // �������� �� ����� � ������� ����� 
            printf("� ����� %d ����� ������� ���� ������ �����.\n", MIN_FLOOR);
            continue;
        }
        else if (inputFloor == MAX_FLOOR) { // �������� �� ������ � �������� ����� 
            printf("� ����� %d ����� ������� ���� ������ ����.\n", MAX_FLOOR);
            continue;
        }

        if (inputFloor == 0) { // ���������� �����, ���� ������� 0
            printf("������ ���� ������ ��������.\n");
            break; 
        }

        if (abs(inputFloor) < MIN_FLOOR || abs(inputFloor) > MAX_FLOOR) { // �������� ���������
            printf("������: ���� ������ ���� � ��������� [%d, %d].\n", MIN_FLOOR, MAX_FLOOR);
            continue; 
        }

        calls[count].floor = abs(inputFloor); // ������ ����� ���������� ����
        calls[count].direction = (inputFloor > 0) ? 1 : -1; // ����������� ����������� �� ����� �����
        count++; // ���������� �������� �������
    }

    return count;
}

void displayInput(ElevatorCall* calls, int count) {
    for (int i = 0; i < count; i++) {
        if (calls[i].direction > 0) { 
            printf("����� � ����� %d. �����������: �����.\n", calls[i].floor);
        }
        else {
            printf("����� � ����� %d. �����������: ����.\n", calls[i].floor);
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
        int floor = rand() % (MAX_FLOOR - MIN_FLOOR + 1) + MIN_FLOOR; // ��������� ����� �� MIN_FLOOR �� MAX_FLOOR
        int direction = (rand() % 2) == 0 ? 1 : -1; // ��������� ����������� ��������� �������

        calls[i].floor = floor; // ���������� ���������������� ����� � ������ �������
        calls[i].direction = direction; // ���������� ���������������� ����������� � ������ �������
    }
    return 0;
}

int* readFromFile(int* totalCount) {
    FILE* file = fopen("test_data.txt", "r");
    if (file == NULL) {
        return -1;
    }
    
    int* total = malloc(MAX_REQUESTS * sizeof(int)); // ������������ ��������� ������ ��� ������ total
    if (total == NULL) {
        fclose(file);
        return -1;
    }

    *totalCount = INITIAL_COUNT_DATA; // ������������� �������� ����������� ������

    while (*totalCount < MAX_REQUESTS && fscanf(file, "%d", &total[*totalCount]) == 1) { // ���������� �������� �� ������� ��� �������� ������ ����� �� �����
        (*totalCount)++; 
    }

    fclose(file);

    return total;
}

void selectRequests(int* total, int totalCount, RequestData* requests) {
    for (int i = 0; i < totalCount; i++) {
        if (total[i] > 0) { // ��������, �������� �� ����� �����
            requests->request_up[requests->upCount++] = total[i]; // ��������� ����� ����� � ������ request_up � ����������� �������
        }
        else { // � ��������� ������ ��� ����� ����
            requests->request_down[requests->downCount++] = -total[i]; // ��������� ������ ������ ���� � ������ request_down � ����������� �������
        }
    }
}

void displayRequests(const RequestData* requests) {
    printf("\n������� �����:\n");
    for (int i = 0; i < requests->upCount; i++) {
        printf("%d ", requests->request_up[i]);
    }

    printf("\n������� ����:\n");
    for (int i = 0; i < requests->downCount; i++) {
        printf("%d ", requests->request_down[i]);
    }

    printf("\n");
    return 0;
}

int compareAscending(int a, int b) {
    return a - b; // ���������� ������� ����� a � b
}

int compareDescending(int a, int b) {
    return b - a; // ���������� ������� ����� b � a
}

void sortRequests(int* requests, int count, int (*compare)(int a, int b)) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (compare(requests[i], requests[j]) > 0) { // ������������� ������� ��������� ��� ����������� �������
                int temp = requests[i]; // ���������� �������� �������� �� ��������� ����������
                requests[i] = requests[j]; // ������ ������� ���������, ���� ��� �� � ���������� �������
                requests[j] = temp;
            }
        }
    }
    return 0;
}

void processRequests(int* request_up, int upCount, int* request_down, int downCount) {
    int current_floor = MIN_FLOOR;

    // ��������� �������� �����
    for (int i = 0; i < upCount; i++) {
        while (current_floor < request_up[i]) { // ������ �� �������������� �����
            current_floor++; // ���������� �������� �����
            printf("���� ����������� �� ���� %d ...\n", current_floor);
        }
        if (current_floor == request_up[i]) { // �������� � ������������� ������
            printf("���� ����������� �� ����� %d\n", current_floor);
        }
    }

    // �������� ������������� ��������� �� ������ "����"
    int highest_floor; // ���� �� �������� ����� ��������
    if (downCount > 0) { // �������� ������� �������� ����
        highest_floor = request_down[0]; // ������ ����� �������� ����� �� �������� ����
    }
    else {
        highest_floor = request_up[upCount - 1]; // ������ ������ �������� ����� �� �������� ����� 
    }

    while (current_floor < highest_floor) { // ������ �� ����� ���� ��������
        current_floor++;
        printf("���� ����������� �� ���� %d ...\n", current_floor);
    }

    // ��������� �������� ����
    for (int i = 0; i < downCount; i++) {
        while (current_floor > request_down[i]) { // ����� �� �������������� �����
            current_floor--;
            printf("���� ���������� �� ���� %d ...\n", current_floor);
        }
        if (current_floor == request_down[i]) { // �������� � ������������� ������
            printf("���� ����������� �� ����� %d\n", current_floor);
        }
    }
    return 0;
}

void analyzeData(int* total, int totalCount) {
    if (total == NULL || totalCount <= 0) {
        printf("������: ������ ����������� ��� ����� �������");
    }

    int upCalls = INITIAL_COUNT_UP; // ������� ������� �����
    int downCalls = INITIAL_COUNT_DOWN; // ������� ������� ����
    int callCounts[MAX_FLOOR + 1] = { 0 }; // ������ ��� �������� ������� �� ������ �����

    for (int i = 0;i < totalCount;i++) {
        if (total[i] > 0) {
            upCalls++;
        }
        else {
            downCalls++;
        }
        callCounts[abs(total[i])]++; // ���������� �������� ��� ���������������� ����� 
    }

    int mostFrequentFloor = MIN_FLOOR; // ����� ����� ���������� ����
    for (int floor = MIN_FLOOR; floor <= MAX_FLOOR; floor++) {
        if (callCounts[floor] > callCounts[mostFrequentFloor]) { 
            mostFrequentFloor = floor;
        }
    }

    printf("\n������ ������:\n");
    printf("����� �������: %d\n", totalCount);
    printf("������ �����: %d\n", upCalls);
    printf("������ ����: %d\n", downCalls);
    printf("����� ����� ���������� ����: %d\n", mostFrequentFloor);

    return 0;
}
