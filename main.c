#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef union
{
    float fData;
    char  cData;
} Item;

typedef struct node{
    Item data;
    struct node* next;
}node;

typedef struct{
node* top;
}Stack;

node* newNode(Item value)
{
    node* n = malloc(sizeof(node));
    n->data = value;
    n->next = NULL;
    return n;
}

Stack* initialize()
{
    Stack* s = malloc(sizeof(Stack));
    s->top = NULL;
    return s;
}

int isEmpty(Stack *s)
{
    if(s->top == NULL)
        return 1;
    return 0;
}


Item top(Stack *s)
{
    return s->top->data;
}
/*
*
*/
Item pop(Stack *s)
{
    Item temp_data = s->top->data;
    node* temp_node = s->top;
    s->top = s->top->next;
    free(temp_node);
    return temp_data;
}
/*
*
*/
void push(Stack *s, Item val)
{
    node* n = newNode(val);
    if(!n) return;
    n->next = s->top;
    s->top = n;
}

int isOperator(char ch)
{
    if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' || ch == '(' || ch == ')')
        return 1;
    return 0;
}

int Precedence(char ch)
{
    switch (ch)
    {
    case '+':
    case '-':
        return 0;

    case '*':
    case '/':
        return 1;

    case '^':
        return 2;

    case '(':
         return 3;
    }
    return -1;
}

void infixToPostfix(char *infix, char* postfix)
{
    memset(postfix,0,strlen(postfix));
    int j = 0;
    char* token;
    char* tokens_infix[100];
    token = strtok(infix, " ");
    while (token != NULL) {
        tokens_infix[j++] = token;
        token = strtok(NULL, " ");
    }
    int i;
    Item temp;
    char c;
    Stack* s = initialize();
    for(i = 0; i < j; i++)
    {
        if(!isOperator(tokens_infix[i][0]) || tokens_infix[i][1] != '\0')
        {
            strcat(postfix, tokens_infix[i]);
            strcat(postfix, " ");
        }
        else if(tokens_infix[i][0] == ')')
        {
            while(!isEmpty(s) && top(s).cData != '(')
            {
                c = pop(s).cData;
                strncat(postfix,&c, 1);
                strcat(postfix, " ");
            }
            temp = pop(s);
        }
        else if(isEmpty(s))
        {
            temp.cData = tokens_infix[i][0];
            push(s, temp);
        }
        else
        {
            while(!isEmpty(s) && Precedence(top(s).cData) >= Precedence(tokens_infix[i][0]) && top(s).cData != '(')
            {
                c = pop(s).cData;
                strncat(postfix,&c, 1);
                strcat(postfix, " ");
            }
            temp.cData = tokens_infix[i][0];
            push(s, temp);
        }
    }

    while(!isEmpty(s))
    {
        c = pop(s).cData;
        strncat(postfix,&c, 1);
        strcat(postfix, " ");
    }
}
float Operation(float x, float y, char oper)
{
    switch(oper)
    {
    case '+':
        return x+y;
    case '-':
        return y - x;
    case '*':
        return x*y;
    case '/':
        return y / x;
    case '^':
        return pow(y, x);
    }
    return 0;
}

float evaluatePostfix(char* postfix)
{
    Item temp;
    float f1;
    float f2;
    int j = 0;
    char* token;
    char* tokens_postfix[100];
    token = strtok(postfix, " ");
    while (token != NULL) {
        tokens_postfix[j++] = token;
        token = strtok(NULL, " ");
    }
    int i;
    Stack* s = initialize();
    for(i = 0; i < j; i++)
    {
        if(!isOperator(tokens_postfix[i][0]) || tokens_postfix[i][1] != '\0')
        {
            temp.fData = atof(tokens_postfix[i]);
            push(s, temp);
        }
        else if(isOperator(tokens_postfix[i][0]))
        {
            f1 = pop(s).fData;
            f2 = pop(s).fData;
            temp.fData = Operation(f1, f2, tokens_postfix[i][0]);
            push(s, temp);
        }
    }
    return pop(s).fData;
}

void replaceNewLineBySpace(char *s)
{
    char *s1 = s;
    while((s1 = strstr(s1, "\n")) != NULL)
        *s1 = ' ';
}
/*
*
*/
int main()
{
    char infixExpr[256] = "";
    char postfixExpr[256] = "";
    printf("Enter an expression you want to evaluate or Ctrl+Z to exit: ");
    while(fgets(infixExpr, 255, stdin) != NULL)
    {
        replaceNewLineBySpace(infixExpr);
        infixToPostfix(infixExpr, postfixExpr);
        printf("Postfix : %s\n", postfixExpr);
        float result = evaluatePostfix(postfixExpr);
        printf("Result: %f\n\n", result);
        printf("Enter an expression you want to evaluate or Ctrl+Z to exit: ");
    }
    return 0;
}
