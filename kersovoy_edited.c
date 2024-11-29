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
        printf("������: �� ������� ������� ���� ��� �����.\n");
        return 1;
    }

    while (1) {
        printMenu();
        printf("�������� �������: ");
        int choice;
        scanf("%d", &choice);
        getchar();

        switch (choice) {
        case 1:
            monitoring = 1;
            printf("���������� �������.\n");
            break;

        case 2:
            monitoring = 0;
            printf("���������� ��������.\n");
            break;

        case 3:
            if (!monitoring) {
                printf("���������� ��������, �������� ��� ��������� ������.\n");
                break;
            }
            printf("������� ���� � ����������� (up/down): ");
            fgets(input, sizeof(input), stdin);
            sscanf(input, "%d %s", &floor, input); 
            direction = napravlenie(input);

            if (floor < MIN_FLOOR || floor > MAX_FLOOR) {
                printf("������: ���� ������ ���� �� %d �� %d.\n", MIN_FLOOR, MAX_FLOOR);
            }
            else if (direction == INVALID) {
                printf("������: �������� �����������. ����������� '�����' ��� '����'.\n");
            }
            else {
                monitorlift(floor, direction);
                logData(floor, direction); 
            }
            break;

        case 4:
            printf("����� �� ���������.\n");
            fclose(logFile);
            return 0;

        default:
            printf("������������ �����. ���������� �����.\n");
            break;
        }
    }
    return 0;
}

void monitorlift(int floor, Napravl nap) {
    const char* directionStr = (nap == UP) ? "�����" : "����";
    printf("����������: ���� %d, �����������: %s\n", floor, directionStr);
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
    printf("\n����:\n");
    printf("1. �������� ����������\n");
    printf("2. ��������� ����������\n");
    printf("3. ������ ���� � �����������\n");
    printf("4. �����\n");
}

void logData(int floor, Napravl nap) {
    FILE* logFile = fopen(LOG_FILE, "a");
    if (logFile != NULL) {
        fprintf(logFile, "����: %d, �����������: %s\n", floor, (nap == UP) ? "�����" : "����");
        fclose(logFile); 
    }
    else {
        printf("������: �� ������� �������� � ���� �����.\n");
    }
}



















