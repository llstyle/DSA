#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "data.h"

#define SAVE_FILE "events.dat"

int main()
{
    setlocale(LC_ALL, "Rus");

    Event *events = NULL;
    int count = 0;
    int capacity = 0;
    int choice;

    printf("=== Программа управления событиями ===\n");

    do
    {
        printf("\n=== МЕНЮ ===\n");
        printf("1. Добавить\n");
        printf("2. Вывести\n");
        printf("3. Редактировать\n");
        printf("4. Поиск\n");
        printf("5. Удалить последний\n");
        printf("6. Сортировать\n");
        printf("7. Вставить на позицию\n");
        printf("8. Удалить по индексу\n");
        printf("9. Очистить список\n");
        printf("10. Сохранить в файл\n");
        printf("11. Загрузить из файла\n");
        printf("0. Выход\n");
        printf("Выбор > ");

        scanf("%d", &choice);
        clear_buffer();

        switch (choice)
        {
        case 1:
            addEvent(&events, &count, &capacity);
            break;
        case 2:
            printEvents(events, count);
            break;
        case 3:
            editEvent(events, count);
            break;
        case 4:
            searchEvent(events, count);
            break;
        case 5:
            removeLastEvent(&events, &count, &capacity);
            break;
        case 6:
            sortEvents(events, count);
            break;
        case 7:
            insertEvent(&events, &count, &capacity);
            break;
        case 8:
            removeEventByIndex(&events, &count, &capacity);
            break;
        case 9:
            clearAll(&events, &count, &capacity);
            break;
        case 10:
            saveEventsToFile(events, count, SAVE_FILE);
            break;
        case 11:
            loadEventsFromFile(&events, &count, &capacity, SAVE_FILE);
            break;
        case 0:
            printf("\n--- Сохранение данных перед выходом ---\n");
            saveEventsToFile(events, count, SAVE_FILE);
            clearAll(&events, &count, &capacity);
            printf("Выход...\n");
            break;
        default:
            printf("Неверный пункт.\n");
        }

    } while (choice != 0);

    return 0;
}
