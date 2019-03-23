#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define     TRUE                1
#define     FALSE               0
#define     START_VAR_AMOUNT    1
#define     START_VAR_LENGHT    10
#define     START_EXPR_LENGHT   20
#define     STACK_SIZE          100
#define     TOKEN_AMOUNT        4

typedef char Bool;

typedef struct varriable {
    char *name;
    Bool value;
    size_t len;
} var;

typedef struct Stack {
    char data[STACK_SIZE];
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

char *increase_string(char* arr, size_t new_len) {
    char *timeless = NULL;
    if ((timeless = (char*)realloc(arr, new_len * sizeof(char))) == NULL) {
        return NULL;
    } else {
        return timeless;
    }
}

// ================================

Bool get_var(char* buf, var* v, char opers[][sizeof("and")]) {
    int i = 0;
    for (i = 0; buf[i] == ' '; i++) {}                  //  удалилить стартовые пробелы

    const char *token_begin = &buf[i];
    char *token_end = NULL;

    for (; buf[i] != '=' && buf[i] != '\n' && buf[i] != '\0'; i++) {     //  Поверка переменной
        if ((buf[i] < 'a') || (buf[i] > 'z')) {
            if (buf[i] == ' ') {
                token_end = &buf[i];
                for (; buf[i] == ' ' ; i++) {}
                if (buf[i] != '=') {
                    return FALSE;
                } else {
                    i--;
                }
            } else {
                return FALSE;
            }
        }
    }
    if (token_end == NULL) {
        token_end = &buf[i];
    }

    if ((buf[i] == '\0') || (buf[i] == '\n')) {
        return FALSE;
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
            for (size_t t = 0; t < TOKEN_AMOUNT; t++) {
                if (!strncmp(token_begin, opers[t], strlen(opers[t]))) {
                    return FALSE;
                }
            }
            size_t diff = (size_t)(token_end - token_begin);
            if (v->len < diff) {
                char *check = NULL;
                if ((check = increase_string(v->name, diff)) == NULL) {
                    return FALSE;
                } else {
                    v->name = check;
                }
            }
            strncpy(v->name, token_begin, diff);
            v->value = val;
            return TRUE;
        } else return FALSE;

    } else return FALSE;
}           //  Done

// ================================

int compare_vars(char* word, var** varriables, size_t var_numb) {
    for (size_t v = 0; v < var_numb; v++) {
        if (!strncmp(word, varriables[v]->name, varriables[v]->len)) {
            return v;
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
    if (!strncmp(word, "True", 4)) {
        return 1;
    } else if (!strncmp(word, "False", 5)) {
        return 0;
    }
    return -1;
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
                    free(varriables[j]->name);
                    free(varriables[j]);
                }
                free(varriables);
                return NULL;
            } else {
                if ((varriables[i]->name = (char*)calloc(START_VAR_LENGHT, sizeof(char))) == NULL) {
                    for (int j = 0; j < i; j++) {
                        free(varriables[j]->name);
                        free(varriables[j]);
                    }
                    free(varriables);
                    return NULL;
                }
                varriables[i]->len = START_VAR_LENGHT;
            }
        }
    }
    return varriables;
}

// ================================

Bool free_var_array(var **v, size_t len) {
    for (int i =0; i < len; i++) {
        free(v[i]->name);
        free(v[i]);
    }
    free(v);
    return FALSE;
}

// ================================

var** add_vars(var** v, size_t current_amount, size_t add_amount) {
    var** timeless = NULL;
    if ((timeless = (var**)realloc(v, sizeof(var*) * (current_amount + add_amount))) == NULL) {
        return NULL;
    } else {
        v = timeless;
        size_t new_amount = current_amount + add_amount;
        for (size_t i = current_amount; i < new_amount; i++) {
            if ((v[i] = (var*)calloc(1, sizeof(var))) == NULL) {
                for (int j = 0; j < i; j++) {
                    free(v[j]->name);
                    free(v[j]);
                }
                return NULL;
            } else {
                if ((v[i]->name = (char*)calloc(START_VAR_LENGHT, sizeof(char))) == NULL) {
                    for (int j = 0; j < i; j++) {
                        free(v[j]->name);
                        free(v[j]);
                    }
                    return NULL;
                }
                v[i]->len = START_VAR_LENGHT;
            }
        }
        return v;
    }
}

// ================================

Bool get_expr(char *buf, char **expr, size_t *e_len, var** varriables, size_t var_numb) {
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
    do {
        if ((buf[i] != ' ') && (buf[i] != '(') && (buf[i] != ')') && (buf[i] != '\0') && (buf[i] != '\n')) {
            word[pos] = buf[i];
            word[pos + 1] = '\0';
            pos++;
        } else {
            if (element_position >= *e_len - 3) {
                *e_len *= 2;
                char *check = NULL;
                if ((check = increase_string(expr[0], *e_len)) == NULL) {
                    return FALSE;
                } else {
                    expr[0] = check;
                }
            }

            if (pos > 0) {
                int find = -1;
                if ((find = compare_vars(word, varriables, var_numb)) != -1) {
                    if (varriables[find]->value == 0) {
                        expr[0][element_position++] = '0';
                    } else {
                        expr[0][element_position++] = '1';
                    }
                } else if ((find = compare_operands(word, opers)) != -1) {
                    expr[0][element_position++] = opers[find][0];
                } else if ((find = compare_bool(word)) != -1) {
                    if (find == 0) {
                        expr[0][element_position++] = '0';
                    } else {
                        expr[0][element_position++] = '1';
                    }
                } else {
                    free(word);
                    return FALSE;
                }
                expr[0][element_position] = '\0';
                pos = 0;
            }
        }

        if (buf[i] == '(') {
            expr[0][element_position++] = '(';
            expr[0][element_position] = '\0';
            open++;
        } else if (buf[i] == ')') {
            expr[0][element_position++] = ')';
            expr[0][element_position] = '\0';
            close++;
        }

    } while (buf[i++] != '\0');

    if (open != close) {
        free(word);
        return FALSE;
    }

    free(word);
    return TRUE;
}

// ================================

Bool handle_input(char **expression, size_t *e_len) {
    size_t amount = START_VAR_AMOUNT;
    var** varriables = NULL;
    if ((varriables = get_var_array()) == NULL) {
        return FALSE;
    }

    char *buf = NULL;
    char opers[TOKEN_AMOUNT][sizeof("and")] = {"not", "and", "or", "xor"};

    size_t var_numb = 0;
    size_t mem_len = 0;
    while (getline(&buf, &mem_len, stdin) != -1) {
        if (var_numb == amount) {
            var **timeless = NULL;
            if ((timeless = add_vars(varriables, var_numb, START_VAR_AMOUNT)) == NULL) {
                free (buf);
                free_var_array(varriables, amount);
                return FALSE;
            } else {
                amount += START_VAR_AMOUNT;
                varriables = timeless;
            }
        }
        if (!get_var(buf, varriables[var_numb], opers)) {
            if (!get_expr(buf, expression, e_len, varriables, var_numb)) {
                free (buf);
                free_var_array(varriables, amount);
                return FALSE;
            } else {
                free (buf);
                free_var_array(varriables, amount);
                return TRUE;
            }
        } else {
            var_numb++;
            free(buf);
            buf = NULL;
        }
    }
    free_var_array(varriables, amount);
    return TRUE;
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
        } else if (d < 2) {
            return -1;
        } else if(polish[i] == 'a') {
            d--;
            digits[d - 1] &= digits[d];
        } else if (polish[i] == 'o') {
            d--;
            digits[d - 1] |= digits[d];
        } else if (polish[i] == 'x') {
            d--;
            digits[d - 1] ^= digits[d];
        } else {
            return -1;
        }

        i++;
    }
    return digits[0];
}

// ================================

int main() {
    char *expression = NULL;

    if ((expression = (char*)calloc(START_EXPR_LENGHT, sizeof(char))) == NULL) {
        printf("[error]");
        return 0;
    }


    size_t len = START_EXPR_LENGHT;
    if (!handle_input(&expression, &len)) {
        printf("[error]");
        free(expression);
        return 0;
    }


    char* polish = NULL;
    if ((polish = get_polish(expression)) == NULL) {
        printf("[error]");
        free(expression);
        return 0;
    }
    free(expression);
    expression = NULL;

    int value = calculate(polish);
    if (value == 1) {
        printf("True");
    } else if (value == 0) {
        printf("False");
    } else {
        free(polish);
        printf("[error]");
        return 0;
    }

    free(polish);


    return 0;
}