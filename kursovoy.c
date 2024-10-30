#include <stdio.h>
#include <locale.h>
int main() {
    setlocale(LC_ALL, "rus");
    int lift_floor = 0;
    int to_floor = 0;

    while (1) {
        printf("�� ���������� �� ����� %d. ������� ����� �����, �� ������� ������ ������� (0 ��� ������): \n", lift_floor);
        scanf("%d", &to_floor);

        if (to_floor == 0) {
            break;
        }

        if (to_floor < 1 || to_floor > 10) {
            printf("������: ������������ ����� �����.\n");
            continue;
        }

        printf("���� ������������ �� ���� %d.\n", to_floor);

        if (to_floor > lift_floor) {
            printf("�������� �����...\n");
            while (lift_floor < to_floor) {
                printf("    ���� ��������� ���� %d...\n", lift_floor++);
            }
        }
        else {
            printf("�������� ����...\n");
            while (lift_floor > to_floor) {
                printf("    ���� ��������� ���� %d...\n", lift_floor--);
            }
        }

        printf("�� ������� �� ���� %d.\n", lift_floor);
    }
}