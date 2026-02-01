#include <stdio.h>
#include <string.h>
#include <locale.h>

#define MAX_EVENTS 10
#define STR_LEN 100

typedef struct
{
    char name[STR_LEN];
    char date[20];
    char location[STR_LEN];
    int participants;
} Event;

void clean_newline(char *str)
{
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
    {
        str[len - 1] = '\0';
    }
}

void clear_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

void addEvent(Event *events, int *count)
{
    if (*count >= MAX_EVENTS)
    {
        printf("\n[!] Массив заполнен (макс. %d элементов).\n", MAX_EVENTS);
        return;
    }

    printf("\n--- Добавление события (%d/%d) ---\n", *count + 1, MAX_EVENTS);

    printf("Введите название: ");
    fgets(events[*count].name, STR_LEN, stdin);
    clean_newline(events[*count].name);

    printf("Введите дату (напр. 12.05.2024): ");
    fgets(events[*count].date, 20, stdin);
    clean_newline(events[*count].date);

    printf("Введите место проведения: ");
    fgets(events[*count].location, STR_LEN, stdin);
    clean_newline(events[*count].location);

    printf("Введите количество участников: ");
    scanf("%d", &events[*count].participants);
    clear_buffer();

    (*count)++;
    printf("[OK] Событие добавлено.\n");
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
        printf("\n[!] Нет записей для редактирования.\n");
        return;
    }

    int id;
    printEvents(events, count);
    printf("\nВведите ID события для редактирования (0 - %d): ", count - 1);
    scanf("%d", &id);
    clear_buffer();

    if (id < 0 || id >= count)
    {
        printf("[!] Неверный ID.\n");
        return;
    }

    printf("\nРедактирование события: %s\n", events[id].name);
    printf("1. Название\n");
    printf("2. Дата\n");
    printf("3. Место\n");
    printf("4. Количество участников\n");
    printf("5. Отмена\n");
    printf("Что вы хотите изменить? > ");

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
        fgets(events[id].date, 20, stdin);
        clean_newline(events[id].date);
        break;
    case 3:
        printf("Новое место: ");
        fgets(events[id].location, STR_LEN, stdin);
        clean_newline(events[id].location);
        break;
    case 4:
        printf("Новое кол-во участников: ");
        scanf("%d", &events[id].participants);
        clear_buffer();
        break;
    case 5:
        printf("Редактирование отменено.\n");
        return;
    default:
        printf("Неверный выбор.\n");
        return;
    }
    printf("[OK] Запись обновлена.\n");
}

void searchEvent(const Event *events, int count)
{
    if (count == 0)
    {
        printf("\n[!] Список пуст.\n");
        return;
    }

    char query[STR_LEN];
    printf("\nВведите название (или часть) для поиска: ");
    fgets(query, STR_LEN, stdin);
    clean_newline(query);

    printf("\nРезультаты поиска:\n");
    int found = 0;

    for (int i = 0; i < count; i++)
    {
        if (strstr(events[i].name, query) != NULL)
        {
            printf("#%d: %s (%s) - %s\n", i, events[i].name, events[i].date, events[i].location);
            found++;
        }
    }

    if (found == 0)
    {
        printf("Ничего не найдено.\n");
    }
}

int main()
{
    setlocale(LC_ALL, "Rus");

    Event events[MAX_EVENTS];
    int currentCount = 0;
    int choice;

    do
    {
        printf("\n=== МЕНЮ УПРАВЛЕНИЯ СОБЫТИЯМИ ===\n");
        printf("1. Добавить событие\n");
        printf("2. Вывести список\n");
        printf("3. Редактировать событие\n");
        printf("4. Поиск события\n");
        printf("0. Выход\n");
        printf("Ваш выбор > ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Ошибка ввода!\n");
            clear_buffer();
            continue;
        }
        clear_buffer();

        switch (choice)
        {
        case 1:
            addEvent(events, &currentCount);
            break;
        case 2:
            printEvents(events, currentCount);
            break;
        case 3:
            editEvent(events, currentCount);
            break;
        case 4:
            searchEvent(events, currentCount);
            break;
        case 0:
            printf("Выход...\n");
            break;
        default:
            printf("Неверный пункт меню.\n");
        }
    } while (choice != 0);

    return 0;
}