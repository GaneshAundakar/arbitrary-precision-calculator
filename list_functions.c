/* This file includes all supporting functions */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "main.h"


/* validate_number_arg: Verifies that the string contains only an optional sign followed by pure digits. */

int validate_number_arg(const char *str)
{
    int i = 0;
    if (str[0] == '-' || str[0] == '+') // Skip optional leading sign
        i = 1;

    // Discard strings containing only a sign with no digits (e.g. "+" or "-")
    if (str[i] == '\0')
        return FAILURE;

    while (str[i])
    {
        if (!isdigit((unsigned char)str[i])) // Reject non-digit characters
            return FAILURE;
        i++;
    }
    return SUCCESS;
}

/* is_list_zero: Returns 1 if all digits evaluate to zero, preventing negative zero output ("-0") */

int is_list_zero(Dlist *head)
{
    while (head != NULL)
    {
        if (head -> data != 0) 
            return 0;
        head = head -> next;
    }
    return 1;
}


/* convert: Parses an ASCII numeric string into a doubly linked list of single digits.
 * Skips an optional leading '+' or '-' sign. Returns SUCCESS on completion, or FAILURE if input is malformed */

 int convert(char str[], Dlist **head1, Dlist **tail1)
{
    // Validate output pointers and input string exist
    if (str == NULL || head1 == NULL || tail1 == NULL) // Check for null pointers
    {
        return FAILURE;
    }

    int i = 0;
    
    // Advance index to skip optional sign character
    if (str[0] == '-' || str[0] == '+')
    {
        i = 1;
    }

    // Edge Case: Bare sign with no digits following (e.g., "+" or "-")
    if (str[i] == '\0')
    {
        return FAILURE;
    }

    // Traverse character by character until null-terminator
    while (str[i] != '\0')
    {
        // Reject non-numeric characters early
        if (!isdigit((unsigned char)str[i]))
        {
            return FAILURE;
        }

        // Convert ASCII character to its integer scalar ('5' - '0' = 5)
        int num = str[i] - '0';
        
        // Append digit to list tail (maintains original left-to-right order)
        if (dl_insert_last(head1, tail1, num) == FAILURE)
        {
            return FAILURE;
        }
        i++;
    }
    return SUCCESS;
}



/* Remove_zero: Strips redundant leading zeros (e.g., 007 -> 7)
 * Retains a single '0' node if the entire numeric value is zero (e.g., 0000 -> 0) */

void Remove_zero(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2)
{
    // Strip leading zeros for operand 1 while leaving at least one node
    if (head1 && *head1)
    {
        while ((*head1) -> next != NULL && (*head1) -> data == 0)
        {
            dl_delete_first(head1, tail1);
        }
    }

    // Strip leading zeros for operand 2 targeting head2 and tail2
    if (head2 && *head2)
    {
        while ((*head2) -> next != NULL && (*head2) -> data == 0)
        {
            dl_delete_first(head2, tail2);
        }
    }
}

/* compare: Compares absolute magnitudes of two lists.
 * Returns: 1 if |head1| > |head2| ,  2 if |head2| > |head1| , 0 if |head1| == |head2| */

int compare(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2)
{
    // Eliminate leading zeros to guarantee accurate node count checks
    Remove_zero(head1, tail1, head2, tail2);

    int count1 = 0, count2 = 0;
    Dlist *temp1 = *head1;
    Dlist *temp2 = *head2;

    // Phase 1: Measure length of both lists
    while (temp1 != NULL)
    {
        count1++;
        temp1 = temp1 -> next;
    }

    while (temp2 != NULL)
    {
        count2++;
        temp2 = temp2 -> next;
    }

    // The number with more digits has higher magnitude
    if (count1 > count2) return 1;
    if (count1 < count2) return 2;

    // Phase 2: Lengths match - compare most significant digits left-to-right
    temp1 = *head1;
    temp2 = *head2;

    while (temp1 != NULL && temp2 != NULL)
    {
        if(temp1 -> data > temp2 -> data) 
            return 1;  //first number is larger
        
        if(temp1 -> data < temp2 -> data) 
            return 2;  //second number is larger

        temp1 = temp1 -> next;
        temp2 = temp2 -> next;
    }
    return 0; // All digits are identical
}


/* DLL functions*/

/* dl_insert_last: Appends a new node to the end (tail) of the doubly linked list */

int dl_insert_last(Dlist **head1, Dlist **tail1, int data)
{
    if (head1 == NULL || tail1 == NULL)
    {
        return FAILURE;
    }

    Dlist *new = malloc(sizeof(Dlist));
    if (new == NULL)
    {
        return FAILURE; // Allocation failure
    }

    new -> data = data;
    new -> next = NULL;
    new -> prev = *tail1; // Link backwards to current tail

    if (*head1 == NULL) // Case 1: First insertion into an empty list
    {
        *head1 = new;
        *tail1 = new;
    }
    else                // Case 2: List contains existing nodes
    {
        (*tail1) -> next = new; // Link current tail forward to new node
        *tail1 = new;         // Update tail pointer
    }
    return SUCCESS;
}

/* dl_insert_first: Prepends a new node to the start (head) of the doubly linked list.
 * Critical for arithmetic operations where results are built right-to-left. */

int dl_insert_first(Dlist **head1, Dlist **tail1, int data)
{
    if (head1 == NULL || tail1 == NULL)
    {
        return FAILURE;
    }

    Dlist *new = malloc(sizeof(Dlist));
    if (new == NULL)
    {
        return FAILURE;
    }

    new -> data = data;
    new -> prev = NULL;
    new -> next = *head1; // Link forward to current head

    if (*head1 == NULL) // Case 1: First insertion into an empty list
    {
        *head1 = new;
        *tail1 = new;
    }
    else                // Case 2: List contains existing nodes
    {
        (*head1)->prev = new; // Link current head backward to new node
        *head1 = new;         // Update head pointer
    }

    return SUCCESS;
}

/* dl_delete_first: Removes and frees the node currently at the head of the list. */

int dl_delete_first(Dlist **head1, Dlist **tail1)
{
    // Cannot delete from a non-existent or empty list
    if (head1 == NULL || *head1 == NULL || tail1 == NULL || *tail1 == NULL)
    {
        return FAILURE;
    }

    Dlist *temp = *head1;

    if (*head1 == *tail1) // Case 1: Exactly one node remains
    {
        *head1 = NULL;
        *tail1 = NULL;
    }
    else                  // Case 2: Multiple nodes exist
    {
        *head1 = (*head1) -> next; // Advance head pointer
        (*head1) -> prev = NULL;   // Sever backward link to old head
    }

    free(temp); // Reclaim memory
    return SUCCESS;
}


/* dl_delete_list: Deallocates every node from head to tail and clears pointers to NULL */

 int dl_delete_list(Dlist **head1, Dlist **tail1)
{
    if (!head1 || !*head1) return FAILURE;

    Dlist *temp = *head1;
    while (temp != NULL)
    {
        Dlist *next = temp -> next;
        free(temp);
        temp = next;
    }

    *head1 = NULL;

    if(tail1) 
        *tail1 = NULL;
    return SUCCESS;
}

/* print_list: Traverses forward from head to tail, outputting each digit sequentially. */

void print_list(Dlist *head1)
{
    if (head1 == NULL)
    {
        printf("0");
        return;
    }

    while (head1 != NULL)
    {
        printf("%d", head1 -> data);
        head1 = head1 -> next;
    }
}


/* END */