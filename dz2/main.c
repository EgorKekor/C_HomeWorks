#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define     TRUE                1
#define     FALSE               0
#define     START_VAR_AMOUNT    10
#define     START_LENGHT        100
#define     TOKEN_AMOUNT        4

typedef char Bool;

typedef struct varriable {
    char name[100];                             //  TO_DO: переделать на realloc
    Bool value;
} var;

typedef struct Stack {
    char data[100];
    size_t size;
}Stack;

// ================================

void push(Stack *stack, const char value) {
    stack->data[stack->size] = value;
    stack->size++;
}

// ================================

char pop(Stack *stack) {
    if (stack->size == 0) {
        return -1;
    }
    stack->size--;
    return stack->data[stack->size];
}

// ================================

char peek(Stack *stack) {
    if (stack->size <= 0) {
        return -1;
    }
    return stack->data[stack->size - 1];
}

// ================================

Bool pop_brackets(Stack *stack, char *polish, size_t *ind) {
    char last = pop(stack);
    while (last != '(') {
        polish[*ind] = last;
        *ind = *ind + 1;
        last = pop(stack);
        if (last == -1) {
            return FALSE;
        }
    }
    return TRUE;
}

// ================================

size_t pop_all(Stack *stack, char *polish, size_t *ind) {
    size_t start = *ind;
    char last = -1;
    while ((last = pop(stack)) != -1) {
        polish[*ind] = last;
        *ind = *ind + 1;
    }
    polish[*ind] = '\0';
    return (*ind - start);
}

// ================================================================

Bool get_var(char* buf, var* v) {
    int i = 0;
    for (i = 0; buf[i] == ' '; i++) {}                  //  удалилить стартовые пробелы

    const char *token_begin = &buf[i];
    char *token_end = NULL;

    for (; buf[i] != '=' && buf[i] != '\n'; i++) {     //  Поверка переменной
        if ((buf[i] < 'a') || (buf[i] > 'z')) {
            if (buf[i] == ' ') {
                token_end = &buf[i];
                for (; buf[i] == ' ' ; i++) {}
                if (buf[i] != '=') {
                    return FALSE;
                } else {
                    i--;
                }
            }
        }
    }
    if (token_end == NULL) {
        token_end = &buf[i];
    }

    i++;
    for (; buf[i] == ' ' ; i++) {}

    Bool val = 0;
    if ((buf[i] == 'T') || (buf[i] == 'F')) {
        if ((char*)memmem("False", strlen("False"), &buf[i], strlen("False")) == NULL) {
            if ((char*)memmem("True", strlen("True"), &buf[i], strlen("True")) == NULL) {
                return FALSE;
            } else {
                val = TRUE;
                i += strlen("True");
            }
        } else {
            val = FALSE;
            i += strlen("False");
        }

        for (; buf[i] == ' '; i++) {}
        if (buf[i] == ';') {
            strncpy(v->name, token_begin, (size_t)(token_end - token_begin));
            v->value = val;
            return TRUE;
        } else return FALSE;

    } else return FALSE;
}           //  Done

// ================================

int compare_vars(char* word, var** varriables, size_t var_numb) {
    for (size_t v = 0; v < var_numb; v++) {
        if (!strcmp(word, varriables[v]->name)) {
            return v;                               //  Вернем номер переменной
        }
    }
    return -1;
}

// ================================

int compare_operands(char* word, char opers[][sizeof("and")]) {
    for (int t = 0; t < TOKEN_AMOUNT; t++) {
        if (!strcmp(word, opers[t])) {
            return t;
        }
    }
    return -1;
}

// ================================

int compare_bool(char* word) {
    if (!strcmp(word, "True")) {
        return 1;
    } else if (!strcmp(word, "False")) {
        return 0;
    }
    return -1;
}

// ================================

Bool get_expr(char *buf, char *expr, var** varriables, size_t var_numb) {
    char* word = NULL;
    if ((word = (char*)calloc(strlen(buf), sizeof(char))) == NULL) {
        return FALSE;
    }

    char opers[TOKEN_AMOUNT][sizeof("and")] = {"not", "and", "or", "xor"};

    size_t open = 0;
    size_t close = 0;
    size_t element_position = 0;

    size_t i = 0;
    size_t pos = 0;
    while (buf[i] != '\0') {
        if ((buf[i] != ' ') && (buf[i] != '(') && (buf[i] != ')') && (buf[i] != '\n')) {
            word[pos] = buf[i];
            word[pos + 1] = '\0';
            pos++;
        } else {
            if (pos > 0) {
                int find = -1;
                if ((find = compare_vars(word, varriables, var_numb)) != -1) {
                    if (varriables[find]->value == 0) {
                        expr[element_position++] = '0';
                    } else {
                        expr[element_position++] = '1';
                    }
                } else if ((find = compare_operands(word, opers)) != -1) {
                    expr[element_position++] = opers[find][0];
                } else if ((find = compare_bool(word)) != -1) {
                    if (find == 0) {
                        expr[element_position++] = '0';
                    } else {
                        expr[element_position++] = '1';
                    }
                } else {
                    free(word);
                    return FALSE;
                }
                expr[element_position] = '\0';
                pos = 0;
            }
        }

        if (buf[i] == '(') {
            expr[element_position++] = '(';
            expr[element_position] = '\0';
            open++;
        }
        if (buf[i] == ')') {
            expr[element_position++] = ')';
            expr[element_position] = '\0';
            close++;
        }
        i++;
    }

    if (open != close) {
        free(word);
        return FALSE;
    }

    free(word);
    return TRUE;
}   //  Получает логическое выражение

// ================================

Bool handle_input(char *expression, var** varriables, size_t *var_amount) {
    char *buf = NULL;

    size_t var_numb = 0;
    size_t mem_len = 0;
    while (getline(&buf, &mem_len, stdin) != -1) {
        if (!get_var(buf, varriables[var_numb])) {
            if (!get_expr(buf, expression, varriables, var_numb)) {
                free (buf);
                return FALSE;
            } else {
                free (buf);
                return TRUE;
            }
        } else {
            var_numb++;
            *var_amount = var_numb;
            free(buf);
            buf = NULL;
        }
    }
    return TRUE;
}

// ================================

var** get_var_array() {
    var **varriables = NULL;
    if ((varriables = (var**)calloc(START_VAR_AMOUNT, sizeof(var*))) == NULL) {      //  TO_DO: переделать на realloc
        return NULL;
    } else {
        for (int i = 0; i < START_VAR_AMOUNT; i++) {
            if ((varriables[i] = (var*)calloc(1, sizeof(var))) == NULL) {
                for (int j = 0; j < i; j++) {
                    free(varriables[j]);
                }
                free(varriables);
                return NULL;
            }
        }
    }
    return varriables;
}

// ================================

var** delete_var_array(var **v, size_t len) {
    for (int i = 0; i < len; i++) {
        free(v[i]);
    }
    free(v);
    return NULL;
}

// ================================

Bool is_operand(char ch) {
    if ((ch == '0') || (ch == '1'))
        return TRUE;
    else
        return FALSE;
};

// ================================

size_t get_priorety(char ch) {
    switch (ch) {
        case '(': return 0;
        case ')': return 0;
        case 'o': return 1;     //  or
        case 'x': return 1;     //  xor
        case 'a': return 2;     //  and
        case 'n': return 3;     //  not
        default: return 0;
    }
}

// ================================

char* get_polish(char* expr) {
    char* polish = NULL;
    if ((polish = (char*)calloc(51, sizeof(char))) == NULL) {
        return NULL;
    }

    Stack operators;
    operators.size = 0;

    size_t p = 0;
    size_t i = 0;
    while (expr[i] != '\0') {
        if (is_operand(expr[i])) {
            polish[p++] = expr[i];
        } else if (expr[i] == '(') {
            push(&operators, expr[i]);
        } else if (expr[i] == ')') {
            pop_brackets(&operators, polish, &p);
        } else {
            size_t prior = get_priorety(expr[i]);
            char last;
            if ((last = peek(&operators)) == -1) {
                push(&operators, expr[i]);
            } else {
                if (prior > get_priorety(last)) {
                    push(&operators, expr[i]);
                } else {
                    last = pop(&operators);
                    push(&operators, expr[i]);
                    polish[p++] = last;
                }
            }
        }
        i++;
        polish[p] = '\0';
    }
    pop_all(&operators, polish, &p);
    return polish;
}

// ================================

int calculate(char* polish) {
    int digits[20];
    size_t d = 0;
    size_t i = 0;
    while (polish[i] != '\0') {
        while (is_operand(polish[i])) {
            if (polish[i] == '1') {
                digits[d] = 1;
            } else {
                digits[d] = 0;
            }

            d++;
            i++;
        }

        if (polish[i] == 'n') {
            digits[d - 1] = !digits[d - 1];
        } else if (polish[i] == 'a') {
            d--;
            digits[d - 1] &= digits[d];
        } else if (polish[i] == 'o') {
            d--;
            digits[d - 1] |= digits[d];
        }

        i++;
    }
    return digits[0];
}

// ================================

int main() {
    char *expression = NULL;
    var **varriables = NULL;

    if ((expression = (char*)calloc(START_LENGHT, sizeof(char))) == NULL) {
        printf("[error]");
        return 0;
    }

    if ((varriables = get_var_array()) == NULL) {
        free(expression);
        printf("[error]");
        return 0;
    }


    size_t amount = 0;
    if (!handle_input(expression, varriables, &amount)) {
        printf("[error]");                                  //  TO_DO: добавить free
        return 0;
    }

    char* polish = NULL;
    if ((polish = get_polish(expression)) == NULL) {
        printf("[error]");
        return 0;
    }

    int value = calculate(polish);
    if (value == 1) {
        printf("True");
    } else if (value == 0) {
        printf("False");
    } else {
        printf("[error]");
        return 0;
    }

    //printf("%s\n", expression);
    //printf("%s\n", polish);

    free(expression);
    free(polish);
    delete_var_array(varriables, START_VAR_AMOUNT);


    return 0;
}