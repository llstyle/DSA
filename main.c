#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

#define STR_LEN 100
#define DATE_LEN 20

typedef struct
{
    char name[STR_LEN];
    char date[DATE_LEN];
    char location[STR_LEN];
    int participants;
} Event;

void clean_newline(char *str)
{
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

void clear_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void expandArray(Event **events, int *capacity)
{
    int newCapacity = (*capacity == 0) ? 1 : (*capacity * 2);

    Event *newArray = (Event *)malloc(newCapacity * sizeof(Event));
    if (newArray == NULL)
    {
        printf("Ошибка выделения памяти!\n");
        return;
    }

    for (int i = 0; i < *capacity; i++)
        newArray[i] = (*events)[i];

    free(*events);
    *events = newArray;
    *capacity = newCapacity;

    printf("[INFO] Массив расширен до %d элементов\n", *capacity);
}

void shrinkArray(Event **events, int *capacity, int count)
{
    if (count == 0)
    {
        free(*events);
        *events = NULL;
        *capacity = 0;
        return;
    }

    int newCapacity = count;

    Event *newArray = (Event *)malloc(newCapacity * sizeof(Event));
    for (int i = 0; i < count; i++)
        newArray[i] = (*events)[i];

    free(*events);
    *events = newArray;
    *capacity = newCapacity;

    printf("[INFO] Массив уменьшен до %d элементов\n", *capacity);
}

void addEvent(Event **events, int *count, int *capacity)
{
    if (*count >= *capacity)
        expandArray(events, capacity);

    printf("\n--- Добавление события ---\n");

    printf("Название: ");
    fgets((*events)[*count].name, STR_LEN, stdin);
    clean_newline((*events)[*count].name);

    printf("Дата: ");
    fgets((*events)[*count].date, DATE_LEN, stdin);
    clean_newline((*events)[*count].date);

    printf("Место: ");
    fgets((*events)[*count].location, STR_LEN, stdin);
    clean_newline((*events)[*count].location);

    printf("Количество участников: ");
    scanf("%d", &(*events)[*count].participants);
    clear_buffer();

    (*count)++;
    printf("[OK] Событие добавлено.\n");
}

void removeLastEvent(Event **events, int *count, int *capacity)
{
    if (*count == 0)
    {
        printf("[!] Список пуст.\n");
        return;
    }

    (*count)--;
    printf("[INFO] Последний элемент удалён.\n");

    shrinkArray(events, capacity, *count);
}

void printEvents(const Event *events, int count)
{
    if (count == 0)
    {
        printf("\n[!] Список пуст.\n");
        return;
    }

    printf("\n========================================\n");
    for (int i = 0; i < count; i++)
    {
        printf("ID: %d\n", i);
        printf("Название: %s\n", events[i].name);
        printf("Дата: %s\n", events[i].date);
        printf("Место: %s\n", events[i].location);
        printf("Участники: %d\n", events[i].participants);
        printf("----------------------------------------\n");
    }
}

void editEvent(Event *events, int count)
{
    if (count == 0)
    {
        printf("[!] Нет записей.\n");
        return;
    }

    int id;
    printEvents(events, count);

    printf("Введите ID для редактирования: ");
    scanf("%d", &id);
    clear_buffer();

    if (id < 0 || id >= count)
    {
        printf("[!] Неверный ID.\n");
        return;
    }

    printf("1. Название\n2. Дата\n3. Место\n4. Участники\nВыбор > ");
    int choice;
    scanf("%d", &choice);
    clear_buffer();

    switch (choice)
    {
    case 1:
        printf("Новое название: ");
        fgets(events[id].name, STR_LEN, stdin);
        clean_newline(events[id].name);
        break;
    case 2:
        printf("Новая дата: ");
        fgets(events[id].date, DATE_LEN, stdin);
        clean_newline(events[id].date);
        break;
    case 3:
        printf("Новое место: ");
        fgets(events[id].location, STR_LEN, stdin);
        clean_newline(events[id].location);
        break;
    case 4:
        printf("Новое количество участников: ");
        scanf("%d", &events[id].participants);
        clear_buffer();
        break;
    default:
        printf("Неверный выбор.\n");
        return;
    }

    printf("[OK] Обновлено.\n");
}

void searchEvent(const Event *events, int count)
{
    if (count == 0)
    {
        printf("[!] Список пуст.\n");
        return;
    }

    char query[STR_LEN];
    printf("Введите часть названия: ");
    fgets(query, STR_LEN, stdin);
    clean_newline(query);

    int found = 0;

    for (int i = 0; i < count; i++)
    {
        if (strstr(events[i].name, query) != NULL)
        {
            printf("#%d: %s (%s) - %s\n",
                   i,
                   events[i].name,
                   events[i].date,
                   events[i].location);
            found++;
        }
    }

    if (!found)
        printf("Ничего не найдено.\n");
}

void clearAll(Event **events, int *count, int *capacity)
{
    free(*events);
    *events = NULL;
    *count = 0;
    *capacity = 0;

    printf("[INFO] Вся память освобождена.\n");
}

int main()
{
    setlocale(LC_ALL, "Rus");

    Event *events = NULL;
    int count = 0;
    int capacity = 0;
    int choice;

    do
    {
        printf("\n=== МЕНЮ ===\n");
        printf("1. Добавить\n");
        printf("2. Вывести\n");
        printf("3. Редактировать\n");
        printf("4. Поиск\n");
        printf("5. Удалить последний\n");
        printf("6. Очистить список\n");
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
            clearAll(&events, &count, &capacity);
            break;
        case 0:
            clearAll(&events, &count, &capacity);
            printf("Выход...\n");
            break;
        default:
            printf("Неверный пункт.\n");
        }

    } while (choice != 0);

    return 0;
}
