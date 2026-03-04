#ifndef DATA_H
#define DATA_H

#define STR_LEN 100
#define DATE_LEN 20

typedef struct
{
  char name[STR_LEN];
  char date[DATE_LEN];
  char location[STR_LEN];
  int participants;
} Event;

void clean_newline(char *str);
void clear_buffer();
int isLeapYear(int year);
int validateDate(const char *dateStr);

void expandArray(Event **events, int *capacity);
void shrinkArray(Event **events, int *capacity, int count);

void addEvent(Event **events, int *count, int *capacity);
void removeLastEvent(Event **events, int *count, int *capacity);
void removeEventByIndex(Event **events, int *count, int *capacity);
void printEvents(const Event *events, int count);
void editEvent(Event *events, int count);
void searchEvent(const Event *events, int count);

int compareDates(const char *date1, const char *date2);
void sortEvents(Event *events, int count);

void insertEvent(Event **events, int *count, int *capacity);
void clearAll(Event **events, int *count, int *capacity);

int saveEventsToFile(const Event *events, int count, const char *filename);
int loadEventsFromFile(Event **events, int *count, int *capacity, const char *filename);

#endif