#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
Безруков Егор АПО-12 runid - 1420
Составить программу построчной фильтрации текста.
Суть фильтра — отбор строк, начинающихся с одного из следующих выражений: «Date:», «From:»,«To:», «Subject:».
Текстовые строки подаются на стандартный ввод программы, результат программы должен подаваться на стандартный вывод.

Процедура фильтрации должна быть оформлена в виде отдельной функции, которой подается
на вход массив строк, выделенных в динамической памяти и его длина.
На выходе функция возвращает указатель на NULL-терминированный массив
с найденными строками (последним элементом массива добавлен NULL для обозначения, что данных больше нет).

Программа должна уметь обрабатывать возникающие ошибки (например, ошибки выделения памяти).
В случае возникновения ошибки нужно вывести в поток стандартного вывода сообщение "[error]" и завершить выполнение программы.
*/

#define START_AMOUNT            2
#define START_LENGHT            10
#define SLESH_ZERO              1
#define TRUE                    1
#define FALSE                   0
#define TOKENS_AMOUNT           4
#define LENGHT_FACTOR           1.5
#define AMOUNT_FACTOR           1.5

char** sort(char **buf, size_t amount) {
    char tokens[TOKENS_AMOUNT][sizeof("Subject:")] = {"To:", "Date:", "From:", "Subject:"};
    char **sorted = NULL;
    if ((sorted = (char**)calloc(amount + 1, sizeof(char*))) == NULL) {
        return NULL;
    }

    size_t string_number = 0;
    for (size_t i = 0; i < amount; i++) {
        int find = FALSE;
        for (size_t num = 0; num < TOKENS_AMOUNT && !find; num++) {
            if (strlen(tokens[num]) <= strlen(buf[i])) {
                if ((char*)memmem((void*)tokens[num], strlen(tokens[num]), (void*)buf[i], strlen(tokens[num])) != NULL) {
                    find = TRUE;
                    sorted[string_number++] = buf[i];       //  перекидываем указатель из старого в новый
                    buf[i] = NULL;
                }
            }
        }
        if (!find) {                                        //  Если не нашли освобождаем строку в старом
            free(buf[i]);
            buf[i] = NULL;
        }
    }

    free(buf);
    sorted[string_number] = NULL;
    return sorted;
}

//  ======================================
char** create_string_array(size_t strings, size_t lenght) {
    char **buf = NULL;
    if ((buf = (char**)calloc(strings, sizeof(char*))) == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < strings; i++) {
        if ((buf[i] = (char*)calloc(lenght + SLESH_ZERO, sizeof(char))) == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(buf[j]);
            }
            free(buf);
            return NULL;
        }
    }

    return buf;
}

//  ======================================

void free_string_array(char **array, size_t strings) {
    for (size_t i = 0; i < strings; i++) {
        free(array[i]);
    }
    free(array);
}
//  ======================================
//  Увеличивает количество строк
char** elongation_array(size_t string_number, size_t *amount, char** buf) {
    if (string_number + 1 == *amount) {
        size_t old_amount = *amount;
        *amount = (size_t)((double)(*amount) * AMOUNT_FACTOR);
        char **temporary = NULL;
        if ((temporary = (char**)realloc(buf, *amount * (sizeof(char*)))) == NULL) {
            free_string_array(buf, old_amount);
            printf("[error]");
            return NULL;
        } else {
            buf = temporary;
            for (size_t i = old_amount; i < *amount; i++) {
                if ((buf[i] = (char*)calloc(START_LENGHT + SLESH_ZERO, sizeof(char))) == NULL) {
                    free_string_array(buf, old_amount + i);
                    printf("[error]");
                    return NULL;
                }
            }
        }
    }
    return buf;
}

//  ======================================
//  Корректирует длину
size_t reset_lenghts(size_t *lenght, size_t *substring_lenght) {
    size_t old_lenght = *lenght;
    *lenght = (size_t)((double)(*lenght) * LENGHT_FACTOR);
    *substring_lenght = *lenght - old_lenght;
    return old_lenght;
}

//  ======================================
//  Увеличивает длину строки
char* elongation_string(size_t lenght, size_t old_lenght, size_t amount, size_t string_number, char** buf) {
    char *temporary = NULL;
    if ((temporary = (char*)realloc(buf[string_number], (lenght + SLESH_ZERO) * (sizeof(char)) )) == NULL) {
        free_string_array(buf, amount);
        printf("[error]");
        return NULL;
    } else {
        buf[string_number] = temporary;
        buf[string_number][lenght - SLESH_ZERO] = 0;
        return (&buf[string_number][old_lenght]);
    }
}

//  ======================================
//  Обработчик ввода
char** handle_input(size_t *amount, char **buf) {
    size_t string_number = 0;
    size_t lenght = START_LENGHT;                   //  Длинна строки
    size_t substring_lenght = lenght;               //  Длина реаллоченной части
    char* current_position = buf[string_number];

    while (fgets(current_position, (int)(substring_lenght + SLESH_ZERO), stdin) != NULL) {
        buf[string_number][lenght] = '\0';
        if ((buf[string_number][lenght - 1] == '\n') || (strlen(buf[string_number]) < lenght)) {
            if ((buf = elongation_array(string_number, amount, buf)) == NULL) {
                printf("[error]");
                return NULL;
            } else {
                current_position = buf[++string_number];
                substring_lenght = lenght = START_LENGHT;
            }
        } else {
            size_t old_lenght = reset_lenghts(&lenght, &substring_lenght);
            if ((current_position = elongation_string(lenght, old_lenght, *amount, string_number, buf)) == NULL) {
                printf("[error]");
                return NULL;
            }
        }
    }
    return buf;
}

//  ======================================

int main() {
    char **buf = create_string_array(START_AMOUNT, START_LENGHT);
    if (buf == NULL) {
        printf("[error]");
        return 0;
    }

    size_t amount = START_AMOUNT;
    if ((buf = handle_input(&amount, buf)) == NULL) {
        printf("[error]");
        return 0;
    }

    char **sorted = sort(buf, amount);
    if (sorted == NULL) {
        printf("[error]");
        return 0;
    }

    for (size_t i = 0; sorted[i] != NULL; i++) {
        printf("%s", sorted[i]);
        free(sorted[i]);
    }
    free(sorted);

    return 0;
}