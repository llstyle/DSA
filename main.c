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

int isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int validateDate(const char *dateStr)
{
    int day, month, year;
    char separator;
    int parsed = 0;

    if (sscanf(dateStr, "%d.%d.%d", &day, &month, &year) == 3)
    {
        separator = '.';
        parsed = 1;
    }
    else if (sscanf(dateStr, "%d/%d/%d", &day, &month, &year) == 3)
    {
        separator = '/';
        parsed = 1;
    }
    else if (sscanf(dateStr, "%d-%d-%d", &day, &month, &year) == 3)
    {
        separator = '-';
        parsed = 1;
    }

    if (!parsed)
    {
        printf("[!] Неверный формат даты. Используйте DD.MM.YYYY, DD/MM/YYYY или DD-MM-YYYY\n");
        return 0;
    }

    if (year < 1900 || year > 2100)
    {
        printf("[!] Год должен быть от 1900 до 2100\n");
        return 0;
    }

    if (month < 1 || month > 12)
    {
        printf("[!] Месяц должен быть от 1 до 12\n");
        return 0;
    }

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (month == 2 && isLeapYear(year))
        daysInMonth[1] = 29;

    if (day < 1 || day > daysInMonth[month - 1])
    {
        printf("[!] Неверный день для месяца %d (допустимо 1-%d)\n", month, daysInMonth[month - 1]);
        return 0;
    }

    return 1;
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

    int validDate = 0;
    while (!validDate)
    {
        printf("Дата (DD.MM.YYYY): ");
        fgets((*events)[*count].date, DATE_LEN, stdin);
        clean_newline((*events)[*count].date);

        if (validateDate((*events)[*count].date))
            validDate = 1;
    }

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
    {
        int validDate = 0;
        while (!validDate)
        {
            printf("Новая дата (DD.MM.YYYY): ");
            fgets(events[id].date, DATE_LEN, stdin);
            clean_newline(events[id].date);

            if (validateDate(events[id].date))
                validDate = 1;
        }
    }
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

int compareDates(const char *date1, const char *date2)
{
    int day1, month1, year1;
    int day2, month2, year2;

    if (sscanf(date1, "%d.%d.%d", &day1, &month1, &year1) != 3 &&
        sscanf(date1, "%d/%d/%d", &day1, &month1, &year1) != 3 &&
        sscanf(date1, "%d-%d-%d", &day1, &month1, &year1) != 3)
    {
        if (sscanf(date1, "%d-%d-%d", &year1, &month1, &day1) == 3 && year1 > 31)
        {
        }
        else
        {
            return strcmp(date1, date2);
        }
    }

    if (sscanf(date2, "%d.%d.%d", &day2, &month2, &year2) != 3 &&
        sscanf(date2, "%d/%d/%d", &day2, &month2, &year2) != 3 &&
        sscanf(date2, "%d-%d-%d", &day2, &month2, &year2) != 3)
    {
        if (sscanf(date2, "%d-%d-%d", &year2, &month2, &day2) == 3 && year2 > 31)
        {
        }
        else
        {
            return strcmp(date1, date2);
        }
    }

    if (year1 != year2)
        return year1 - year2;
    if (month1 != month2)
        return month1 - month2;
    return day1 - day2;
}

void sortEvents(Event *events, int count)
{
    if (count == 0)
    {
        printf("[!] Список пуст.\n");
        return;
    }

    printf("\n--- Сортировка ---\n");
    printf("1. По названию\n");
    printf("2. По дате\n");
    printf("3. По месту\n");
    printf("4. По количеству участников\n");
    printf("Выбор > ");

    int choice;
    scanf("%d", &choice);
    clear_buffer();

    // Сортировка пузырьком
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            int needSwap = 0;

            switch (choice)
            {
            case 1:
                needSwap = strcmp(events[j].name, events[j + 1].name) > 0;
                break;
            case 2:
                needSwap = compareDates(events[j].date, events[j + 1].date) > 0;
                break;
            case 3:
                needSwap = strcmp(events[j].location, events[j + 1].location) > 0;
                break;
            case 4:
                needSwap = events[j].participants > events[j + 1].participants;
                break;
            default:
                printf("[!] Неверный выбор.\n");
                return;
            }

            if (needSwap)
            {
                Event temp = events[j];
                events[j] = events[j + 1];
                events[j + 1] = temp;
            }
        }
    }

    printf("[OK] Список отсортирован.\n");
}

void insertEvent(Event **events, int *count, int *capacity)
{
    if (*count == 0)
    {
        printf("[!] Список пуст. Используйте добавление.\n");
        return;
    }

    printEvents(*events, *count);

    int position;
    printf("\nВведите позицию для вставки (0-%d): ", *count);
    scanf("%d", &position);
    clear_buffer();

    if (position < 0 || position > *count)
    {
        printf("[!] Неверная позиция.\n");
        return;
    }

    if (*count >= *capacity)
        expandArray(events, capacity);

    for (int i = *count; i > position; i--)
    {
        (*events)[i] = (*events)[i - 1];
    }

    printf("\n--- Добавление события на позицию %d ---\n", position);

    printf("Название: ");
    fgets((*events)[position].name, STR_LEN, stdin);
    clean_newline((*events)[position].name);

    int validDate = 0;
    while (!validDate)
    {
        printf("Дата (DD.MM.YYYY): ");
        fgets((*events)[position].date, DATE_LEN, stdin);
        clean_newline((*events)[position].date);

        if (validateDate((*events)[position].date))
            validDate = 1;
    }

    printf("Место: ");
    fgets((*events)[position].location, STR_LEN, stdin);
    clean_newline((*events)[position].location);

    printf("Количество участников: ");
    scanf("%d", &(*events)[position].participants);
    clear_buffer();

    (*count)++;
    printf("[OK] Событие вставлено на позицию %d.\n", position);
}

void removeEventByIndex(Event **events, int *count, int *capacity)
{
    if (*count == 0)
    {
        printf("[!] Список пуст.\n");
        return;
    }

    printEvents(*events, *count);

    int index;
    printf("\nВведите индекс для удаления (0-%d): ", *count - 1);
    scanf("%d", &index);
    clear_buffer();

    if (index < 0 || index >= *count)
    {
        printf("[!] Неверный индекс.\n");
        return;
    }

    printf("[INFO] Удаляется: %s\n", (*events)[index].name);

    for (int i = index; i < *count - 1; i++)
    {
        (*events)[i] = (*events)[i + 1];
    }

    (*count)--;
    printf("[OK] Элемент удалён.\n");

    shrinkArray(events, capacity, *count);
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
        printf("6. Сортировать\n");
        printf("7. Вставить на позицию\n");
        printf("8. Удалить по индексу\n");
        printf("9. Очистить список\n");
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
