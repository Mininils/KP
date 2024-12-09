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
        printf("������� �����������.\n");
    }
}

void processElevator(int* currentFloor, int* direction) {
    while (queueSize > 0) {
        for (int i = 0; i < queueSize; i++) {
            if (queue[i].floor == *currentFloor) {
                printf("���� �� ����� %d, �����������: %s.\n", queue[i].floor, (*direction == 1) ? "�����" : "����");

                if (logCount < MAX_LOG_ENTRIES) {
                    logEntries[logCount].floor = queue[i].floor;
                    strcpy(logEntries[logCount].direction, (*direction == 1) ? "�����" : "����");
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

        // ��������� ������� ���� � ����������� �� �����������
        *currentFloor += *direction;

        // ��������� ������� �����
        if (*currentFloor > MAX_FLOOR) {
            *currentFloor = MAX_FLOOR;
            *direction = -1; // ������ ����������� �� ����
        }
        else if (*currentFloor < MIN_FLOOR) {
            *currentFloor = MIN_FLOOR;
            *direction = 1; // ������ ����������� �� �����
        }

        // �������� �������� ��� �������/������ �����
        printf("���� ������������ �� ���� %d...\n", *currentFloor);
    }
}

// ������� ��� ��������� �������� ������ ��� �����������
void generateTestData() {
    FILE* file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("������ ��� �������� ����� ��� ������.\n");
        return;
    }

    srand(time(NULL)); // ������������� ���������� ��������� �����

    for (int i = 0; i < NUM_TEST_CASES; i++) {
        int floor = rand() % (MAX_FLOOR - MIN_FLOOR + 1) + MIN_FLOOR; // ��������� ����

        fprintf(file, "%d\n", floor); // ������ ������ ����� � ����
    }

    fclose(file);
    printf("�������� ������ ������������� � ��������� � ���� '%s'.\n", FILENAME);
}

// ������� ��� �������� ������ �� ����� � �������
void loadQueueFromFile() {
    FILE* file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("������ ��� �������� ����� ��� ������.\n");
        return;
    }

    int floor;

    while (fscanf(file, "%d", &floor) != EOF) { // ������ ������ ����� ��� �����������
        addToQueue(floor); // ��������� ������ � �������
    }

    fclose(file);
}

// ������� ��� ������ ���� � ����
void writeLogToFile() {
    FILE* file = fopen(LOGFILE, "w");
    if (file == NULL) {
        printf("������ ��� �������� ����� ��� ������ ����.\n");
        return;
    }

    for (int i = 0; i < logCount; i++) {
        fprintf(file, "%d %s\n", logEntries[i].floor, logEntries[i].direction); // ������ ����� � �����������
    }

    fclose(file);
    printf("������ � ������ ������� �������� � ���� '%s'.\n", LOGFILE);
}

int main() {
    setlocale(LC_ALL, "RUS");
    int choice;

    while (1) {
        printf("\n�������� ����� ������:\n");
        printf("1. ������������� �������� ������\n");
        printf("2. ��������� ������ �� ����� � ��������� ����\n");
        printf("3. �������� ������ � ������ ������� � ����\n");
        printf("4. �����\n");

        scanf("%d", &choice);

        switch (choice) {
        case 1:
            generateTestData(); // ��������� �������� ������ ��� �����������
            break;
        case 2: {
            int currentFloor = rand() % (MAX_FLOOR - MIN_FLOOR + 1) + MIN_FLOOR; // �������� � ���������� �����
            int direction = 1; // ��������� ����������� �����

            loadQueueFromFile(); // ��������� ������ � �������

            printf("���� �������� � ����� %d.\n", currentFloor);
            processElevator(&currentFloor, &direction); // ��������� ������� �����

            break;
        }
        case 3:
            writeLogToFile();
            break;
        case 4:
            printf("����� �� ���������.\n");
            return 0;
        default:
            printf("�������� �����. ����������, ���������� �����.\n");
            break;
        }
    }

    return 0;
}