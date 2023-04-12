#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

int err = 0;	//에러발생 체크


typedef char element;
typedef struct {
    element* data;
    int capacity;
    int top;
}StackType;

void init_stack(StackType* sp)
{
    sp->top = -1;
    sp->capacity = 1;
    sp->data = (element*)malloc(sp->capacity * sizeof(element));
}

int is_empty(StackType* sp)
{
    return (sp->top == -1);
}

int is_full(StackType* sp)
{
    return (sp->top == (MAX_SIZE - 1));
}

void push(StackType* sp, element item)
{
    if (is_full(sp))
    {
        sp->capacity = sp->capacity * 2;
        sp->data = (element*)realloc(sp->data, sp->capacity * sizeof(element));
    }
    sp->data[++(sp->top)] = item;
}

element pop(StackType* sp)
{
    if (is_empty(sp))
    {
        fprintf(stderr, "스택 공백 에러\n");
        return 1;
    }
    else
    {
        return sp->data[(sp->top)--];
    }
}

element peek(StackType* sp)
{
    if (is_empty(sp))
    {
        fprintf(stderr, "스택 공백 에러\n");
        return 1;
    }
    else return sp->data[sp->top];
}

int prec(char op)
{
    switch (op)
    {
    case '(': case ')':return 0;
    case '+': case '-':return 1;
    case '*': case '/':return 2;
    }
    return -1;
}
//중위->후위 출력
void infix_to_postfix(char exp[])
{
    int i = 0;
    char ch, top_op;
    int len = strlen(exp);
    StackType sp;

    init_stack(&sp);
    for (i = 0; i < len; i++)
    {
        ch = exp[i];
        switch (ch)
        {
        case '(':
            push(&sp, ch);
            break;
        case ')':
            top_op = pop(&sp);
            while (top_op != '(')
            {
                printf("%c", top_op);
                top_op = pop(&sp);
            }
            break;
        case '+': case '-': case '*': case '/':
            while (!is_empty(&sp) && prec(ch) <= prec(peek(&sp)))
            {
                printf("%c", pop(&sp));
            }
            push(&sp, ch);
            break;
        default:
            printf("%c", ch);
            break;
        }
    }
    while (!is_empty(&sp))
    {
        printf("%c", pop(&sp));
    }
}

int evaluate_postfix(char* exp)    //후위 표식 계산 함수
{
    StackType sp;
    int len = strlen(exp);
    char ch;
    int op1, op2, value, i = 0;

    init_stack(&sp);

    for (i = 0; i < len; i++)
    {
        ch = exp[i];
        if (ch != '+' && ch != '-' && ch != '*' && ch != '/')
        {
            value = ch - '0';
            push(&sp, value);
        }
        else
        {
            op2 = pop(&sp);
            op1 = pop(&sp);
            switch (ch)
            {
            case '+': push(&sp, op1 + op2); break;
            case '-': push(&sp, op1 - op2); break;
            case '*': push(&sp, op1 * op2); break;
            case '/': push(&sp, op1 / op2); break;
            }
        }
    }
    return pop(&sp);
}

// 후위표기식에 잘못된 문자가 있는지 확인하는 함수
void check_error(element exp[])
{
    err = -1;
    int len = strlen(exp);
    //예외 문자 오류
    for (int i = 0; i < len; i++)
    {
        if (exp[i] == '(' || exp[i] == ')')
        {
            continue;
        }
        else if (exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/')
        {
            continue;
        }
        else if ('0' <= exp[i] && exp[i] <= '9')
        {
            continue;
        }
        else
        {
            printf("Error: Invalid charactor\n");
            err = 1;
        }
    }

    //괄호 오류
    int count = 0;
    for (int i = 0; i < len; i++)
    {
        if (exp[i] == '(')
        {
            count++;
        }
        else if (exp[i] == ')')
        {
            count++;
        }
    }
    if (count != 2 && count != 0)
    {
        printf("Error: Mismatched parentheses\n");
        err = 2;
    }

    //표현 오류
    for (int i = 0; i < len; i++)
    {
        if (exp[0] < '1' && exp[0]>'9')
        {
            printf("Error: Invalid expression\n");
            err = 3;
        }
    }

}

int main()
{
    char infix[MAX_SIZE];
    printf("Enter an infix expression: ");
    scanf_s("%s", infix, MAX_SIZE);

    check_error(infix); // 중위 표기식에 잘못된 문자, 괄호, 표현 검사

    if (err == -1)
    {
        printf("Postfix expression: ");
        infix_to_postfix(infix); // 중위 표기식을 후위 표기식으로 출력
        printf("\n");

        int result = evaluate_postfix(infix); // 후위 표기식 계산
        printf("Result: %d\n", result);
    }
    return 0;
}