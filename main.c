
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef struct node
{
    signed int letter;
    signed int number;
    signed int bt_index;
    signed int amount;
    struct node *next;
} NODE;

clock_t begin;
clock_t end;
int times_tried = 0;
int CRAR_checkExpr(char *exprInLetters, NODE *letters);

NODE *LL_init(int value);

NODE *LL_getNode(NODE *head, int letter);

void LL_destroy(NODE *head);

int LL_len(NODE *head);

NODE *LL_addValue(NODE *head, int value);

void CRAR_findPermutations(char *expr, NODE *letters, unsigned int l, unsigned int r);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("[CRAR]: Usage: ./%s SEND+MORE=MONEY", argv[0]);
    }
    srand(time(NULL));
    NODE *letters = NULL;
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (isalpha(argv[1][i]))
        {
            if (!letters)
            {
                letters = LL_init(argv[1][i]);
            }
            else if (LL_getNode(letters, argv[1][i]) != NULL)
            {
                LL_getNode(letters, argv[1][i])->amount++;
            }
            else
            {
                LL_addValue(letters, argv[1][i]);
            }
        }
    }
    if (LL_len(letters) > 10)
    {
        printf("ERROR\n");
        return 1;
    }
    else
    {
        int i = 35;
        while (LL_len(letters) != 10)
        {
            LL_addValue(letters, i);
            i++;
        }
    }
    int i = 0;
    for (NODE *tmp = letters; tmp; tmp = tmp->next)
    {
        tmp->number = i;
        i++;
    }
    begin = clock();
    CRAR_findPermutations(argv[1], letters, 0, LL_len(letters));
    for (NODE *tmp = letters; tmp; tmp = tmp->next)
    {
        if (!isalpha(tmp->letter))
        {
            break;
        }
        printf("%c: %d\n", tmp->letter, tmp->number);
    }

}


NODE *LL_findNodeWithIndex(NODE *head, unsigned int index)
{
    if (head == NULL)
    {
        return NULL;
    }
    NODE *tmp = head;
    while (tmp != NULL)
    {
        if (tmp->bt_index == index)
        {
            break;
        }
        tmp = tmp->next;
    }
    return tmp;
}

void CRAR_findPermutations(char *expr, NODE *letters, unsigned int l, unsigned int r)
{
    if (l == r)
    {
        if (CRAR_checkExpr(expr, letters))
        {
            return;
        }
    }
    else
    {
        times_tried++;
        for (unsigned int i = l; i <= r; i++)
        {
            NODE *nodeL = LL_findNodeWithIndex(letters, l);
            NODE *nodeI = LL_findNodeWithIndex(letters, i);
            if (nodeL == NULL || nodeI == NULL)
            {
                return;
            }
            int tmpNum = nodeL->number;
            nodeL->number = nodeI->number;
            nodeI->number = tmpNum;
            CRAR_findPermutations(expr, letters, l + 1, r);
            nodeL = LL_findNodeWithIndex(letters, l);
            nodeI = LL_findNodeWithIndex(letters, i);
            if (nodeL == NULL || nodeI == NULL)
            {
                return;
            }
        }
    }
}





int CRAR_checkExpr(char *exprInLetters, NODE *letters)
{
    char *exprInNums = NULL;
    exprInNums = malloc((strlen(exprInLetters) + 1) * sizeof(char));
    char *exprInNums2 = NULL;
    exprInNums2 = malloc((strlen(exprInLetters) + 1) * sizeof(char));
    strcpy(exprInNums, exprInLetters);
    for (NODE *tmp = letters; tmp; tmp = tmp->next)
    {
        if (isalpha(tmp->letter))
        {
            for (signed int i = 0; i < strlen(exprInNums); i++)
            {
                if (tmp->letter == exprInNums[i])
                {
                    exprInNums[i] = (char) tmp->number + 48;
                }
            }
        }
        else
        {
            break;
        }
    }
    for (int i = 0; i < strlen(exprInNums); i++)
    {
        if ((i == 0 || !isdigit(exprInNums[i - 1])) && exprInNums[i] == '0')
        {
            return 0;
        }
    }
    strcpy(exprInNums2, exprInNums);
    char *leftPart = strtok(exprInNums, "=");
    char *rightPart = strtok(NULL, "=");
    long int lCheck = 0;
    long int rCheck = strtol(rightPart, NULL, 10);
    char *term = strtok(leftPart, "+");
    while (term != NULL)
    {
        lCheck += strtol(term, NULL, 10);
        term = strtok(NULL, "+");
    }
    if (lCheck == rCheck)
    {
        end = clock();
        printf("[CRAR]: Correct answer for cryptarithmetic %s is %s\n", exprInLetters, exprInNums2);
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("[CRAR]: Time spent: %lf s\n", time_spent);
        printf("[CRAR]: Options enumerated: %d\n", times_tried);
    }
    free(exprInNums);
    exprInNums = NULL;
    free(exprInNums2);
    exprInNums2 = NULL;
    if (lCheck == rCheck)
    {
        LL_destroy(letters);
        exit(0);
    }
    return lCheck == rCheck;
}


NODE *LL_getNode(NODE *head, int letter)
{
    NODE *tmp = head;
    while (tmp != NULL)
    {
        if (tmp->letter == letter)
        {
            break;
        }
        tmp = tmp->next;
    }
    return tmp;
}

NODE *LL_init(int value)
{
    NODE *head = malloc(sizeof(NODE));
    head -> letter = 0;
    head -> number = 0;
    head -> bt_index = 0;
    head -> amount = 0;
    head -> next = 0;
    head->letter = value;
    head->amount = 1;
    return head;
}

void LL_destroy(NODE *head)
{
    NODE *tmpPtr = head;
    while (tmpPtr != NULL)
    {
        NODE *tmpPtr2 = tmpPtr->next;
        free(tmpPtr);
        tmpPtr = NULL;
        tmpPtr = tmpPtr2;
    }
}


NODE *LL_addValue(NODE *head, int value)
{
    NODE *tmp = malloc(sizeof(NODE));
    tmp->letter = value;
    tmp->number = 0;
    tmp->next = NULL;
    NODE *tmp2 = head;
    int i = 0;
    while (tmp2->next != NULL)
    {
        tmp2 = tmp2->next;
        i++;
    }
    tmp->amount = 1;
    tmp->bt_index = i + 1;
    tmp2->next = tmp;
    return tmp;
}

int LL_len(NODE *head)
{
    NODE *tmp = head;
    int len = 0;
    for (; tmp != NULL; len++)
    {
        tmp = tmp->next;
    }
    return len;

}
