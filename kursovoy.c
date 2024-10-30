#include <stdio.h>
#include <locale.h>
int main() {
    setlocale(LC_ALL, "rus");
    int lift_floor = 0;
    int to_floor = 0;

    while (1) {
        printf("Вы находитесь на этаже %d. Введите номер этажа, на который хотите поехать (0 для выхода): \n", lift_floor);
        scanf("%d", &to_floor);

        if (to_floor == 0) {
            break;
        }

        if (to_floor < 1 || to_floor > 10) {
            printf("Ошибка: некорректный номер этажа.\n");
            continue;
        }

        printf("Лифт отправляется на этаж %d.\n", to_floor);

        if (to_floor > lift_floor) {
            printf("Движение вверх...\n");
            while (lift_floor < to_floor) {
                printf("    лифт проезжает этаж %d...\n", lift_floor++);
            }
        }
        else {
            printf("Движение вниз...\n");
            while (lift_floor > to_floor) {
                printf("    лифт проезжает этаж %d...\n", lift_floor--);
            }
        }

        printf("Вы прибыли на этаж %d.\n", lift_floor);
    }
}