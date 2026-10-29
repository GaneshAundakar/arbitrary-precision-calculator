/* This file includes all 4 arithmetic operation functions */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"


/* addition: Performs column-wise addition starting from least significant digit (tails).
 * Propagates carry leftward and prepends digits to the result list */

int addition(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **res_head, Dlist **res_tail)
{
    Remove_zero(head1, tail1, head2, tail2);

    Dlist *temp1 = *tail1;
    Dlist *temp2 = *tail2;
    int carry = 0;

    // Continue as long as digits remain in either number or a carry is pending
    while (temp1 != NULL || temp2 != NULL || carry != 0)
    {
        int sum = carry;

        if (temp1 != NULL)
        {
            sum += temp1 -> data;
            temp1 = temp1 -> prev; // Move towards higher place value
        }
        if (temp2 != NULL)
        {
            sum += temp2 -> data;
            temp2 = temp2 -> prev; // Move towards higher place value
        }
        carry = sum / 10;

        // Insert remainder at head because we process right-to-left
        dl_insert_first(res_head, res_tail, sum % 10);
    }

    return SUCCESS;
}

/* subtraction: Computes |head1| - |head2|. Caller must guarantee |head1| >= |head2| 
 * Propagates borrow leftward when a minuend digit is smaller than a subtrahend digit. */

int subtraction(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **res_head, Dlist **res_tail)
{
    Remove_zero(head1, tail1, head2, tail2);

    Dlist *temp1 = *tail1;
    Dlist *temp2 = *tail2;
    int borrow = 0;

    while (temp1 != NULL)
    {
        int d1 = temp1 -> data - borrow;
        int d2 = (temp2 != NULL) ? temp2 -> data : 0;

        if (d1 < d2)
        {
            d1 += 10;   // Borrow base 10 from preceding place
            borrow = 1;
        }
        else
        {
            borrow = 0;
        }

        dl_insert_first(res_head, res_tail, d1 - d2);  // Prepend result digit to maintain correct order

        temp1 = temp1 -> prev;

        if (temp2 != NULL)
        {
            temp2 = temp2 -> prev;
        }
    }

    // Normalize output: remove leading zeros produced by subtraction (e.g., 105 - 100 = 005 -> 5)
    
    while ((*res_head) -> next != NULL && (*res_head) -> data == 0)
    {
        dl_delete_first(res_head, res_tail);
    }

    return SUCCESS;
}

/* Multiplication: Multiplies head1 by head2 using long-multiplication with shifting and accumulator addition */

int Multiplication(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **res2_head, Dlist **res2_tail)
{
    Remove_zero(head1, tail1, head2, tail2);

    // Short-circuit: anything multiplied by 0 is 0

    if (((*head1) -> data == 0 && (*head1) -> next == NULL) || ((*head2) -> data == 0 && (*head2) -> next == NULL))
    {
        dl_insert_last(res2_head, res2_tail, 0);
        return SUCCESS;
    }

    // Initialize accumulator to 0
    dl_insert_last(res2_head, res2_tail, 0);

    Dlist *temp2 = *tail2;
    int shift_count = 0; // Tracks positional zero padding per multiplier row

    while (temp2 != NULL)
    {
        Dlist *res1_head = NULL;
        Dlist *res1_tail = NULL;
        Dlist *temp1 = *tail1;
        int carry = 0;

        // Multiply temp2 digit with all digits in head1
        while (temp1 != NULL || carry != 0)
        {
            int prod = carry;
            if (temp1 != NULL)
            {
                prod += (temp1 -> data) * (temp2 -> data);
                temp1 = temp1 -> prev;
            }

            carry = prod / 10;
            dl_insert_first(&res1_head, &res1_tail, prod % 10);
        }

        // Apply positional shift (append trailing zeros)
        for (int i = 0; i < shift_count; i++)
        {
            dl_insert_last(&res1_head, &res1_tail, 0);
        }

        // Accumulate row result into running total
        Dlist *sum_head = NULL;
        Dlist *sum_tail = NULL;
        addition(&res1_head, &res1_tail, res2_head, res2_tail, &sum_head, &sum_tail);

        // Safely free old accumulator to prevent memory leaks and pointer aliasing
        dl_delete_list(res2_head, res2_tail);
        *res2_head = sum_head;
        *res2_tail = sum_tail;

        // Free current intermediate row list
        dl_delete_list(&res1_head, &res1_tail);

        temp2 = temp2 -> prev;
        shift_count++;
    }
    return SUCCESS;
}


/* division: Computes integer quotient using magnitude comparison and repeated subtraction. */

int division(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **res_head, Dlist **res_tail)
{
    Remove_zero(head1, tail1, head2, tail2);

    int cmp = compare(head1, tail1, head2, tail2);

    // Case 1: |dividend| < |divisor| -> Integer quotient is 0
    if (cmp == 2)
    {
        dl_insert_last(res_head, res_tail, 0);
        return SUCCESS;
    }

    // Case 2: |dividend| == |divisor| -> Integer quotient is 1
    if (cmp == 0)
    {
        dl_insert_last(res_head, res_tail, 1);
        return SUCCESS;
    }

    // Initialize quotient counter to 0
    Dlist *count_head = NULL, *count_tail = NULL;

    dl_insert_last(&count_head, &count_tail, 0);

    // Constant increment of 1
    Dlist *one_head = NULL, *one_tail = NULL;

    dl_insert_last(&one_head, &one_tail, 1);

    // Create a working duplicate of dividend so the input list remains untouched
    Dlist *curr_head = NULL, *curr_tail = NULL;
    Dlist *ptr = *head1;

    while (ptr)
    {
        dl_insert_last(&curr_head, &curr_tail, ptr->data);
        ptr = ptr->next;
    }

    // Loop while current working dividend >= divisor
    while (compare(&curr_head, &curr_tail, head2, tail2) != 2)
    {
        Dlist *sub_head = NULL, *sub_tail = NULL;

        subtraction(&curr_head, &curr_tail, head2, tail2, &sub_head, &sub_tail);

        dl_delete_list(&curr_head, &curr_tail); // Free previous intermediate dividend

        curr_head = sub_head;
        curr_tail = sub_tail;

        // Increment quotient: count = count + 1
        Dlist *next_count_head = NULL, *next_count_tail = NULL;

        addition(&count_head, &count_tail, &one_head, &one_tail, &next_count_head, &next_count_tail);

        dl_delete_list(&count_head, &count_tail); // Free previous count

        count_head = next_count_head;
        count_tail = next_count_tail;
    }

    // Clean up working lists
    dl_delete_list(&curr_head, &curr_tail);
    dl_delete_list(&one_head, &one_tail);

    *res_head = count_head;
    *res_tail = count_tail;

    return SUCCESS;
}


/* END of functions */