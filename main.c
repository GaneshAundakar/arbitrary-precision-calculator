/*
Name : Ganesh Aundakar
Date : 25/08/2026 - 07/09/2026 
Description : Arbitrary Precision Calculator - DS Project
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

int main(int argc, char *argv[])
{
 //   printf(CLEAR_SCREEN);

    Dlist *head1 = NULL, *tail1 = NULL;
    Dlist *head2 = NULL, *tail2 = NULL;
    Dlist *res_head = NULL, *res_tail = NULL;

    /* Header */
    printf("\n" CYAN "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓" RESET "\n");
    printf(CYAN "┃" WHITE BOLD "              ARBITRARY PRECISION CALCULATOR               " RESET CYAN "┃" RESET "\n");
    printf(CYAN "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛" RESET "\n\n");

    /* Argument Validation - Expect exactly 3 arguments (program name + 3 tokens) */
    if (argc != 4) 
    {
        printf("\t\t\t"BG_RED " ERROR " RESET "\n\n");
        printf(CYAN " ┌──────────────────────────────────────────────────────────┐" RESET "\n\n");     
        printf(RED BOLD " [✖] Argument Error: " RESET "Invalid number of arguments.\n");
        printf(YELLOW " [i] Usage: " RESET "./calc.exe <number1> <operator> <number2>\n");
        printf(DIM    "     Operators: +  -  x  /\n\n" RESET);
        printf(CYAN " └──────────────────────────────────────────────────────────┘" RESET "\n\n");
//        printf(YELLOW "---------------------------------------------------" RESET "\n\n");

        return FAILURE;
    }

    if (validate_number_arg(argv[1]) == FAILURE || validate_number_arg(argv[3]) == FAILURE) // check valid integer input 
    {
        printf(CYAN " ┌──────────────────────────────────────────────────────────┐" RESET "\n\n");     
        printf("\t\t\t"BG_RED " INVALID INPUT " RESET "\n\n");
        printf(RED BOLD " [✖] Operand Error: " RESET "Operands must be valid integers.\n\n");
//        printf("---------------------------------------------------\n\n");
        printf(CYAN " └──────────────────────────────────────────────────────────┘" RESET "\n\n");
        return FAILURE;
    }

    if (strcmp(argv[2], "+") != 0 && strcmp(argv[2], "-") != 0 && strcmp(argv[2], "x") != 0  && strcmp(argv[2], "/") != 0) // check valid operator input
    {
        printf(CYAN " ┌──────────────────────────────────────────────────────────┐" RESET "\n\n");     
        printf(RED BOLD " [✖] Operator Error: " RESET "Unsupported operator '" YELLOW "%s" RESET "'.\n", argv[2]);
        printf(DIM    "     Supported: +  -  x  /\n\n" RESET);
//        printf("---------------------------------------------------\n\n");
        printf(CYAN " └──────────────────────────────────────────────────────────┘" RESET "\n\n");
        return FAILURE;
    }

    // Convert string inputs to linked lists
    convert(argv[1], &head1, &tail1);
    convert(argv[3], &head2, &tail2);

    int sign1 = (argv[1][0] == '-') ? -1 : 1; //sign of first number    
    int sign2 = (argv[3][0] == '-') ? -1 : 1;  // sign of second number
    int final_sign = 1;

    /*   ----------------------- Addition: A + B ------------------------------ */

    if (strcmp(argv[2], "+") == 0)
    {
        printf("\t\t");
        printf(" " GREEN BOLD "● OPERATION:" RESET " Addition (+)\n\n");  // set color and print operation 

        if (sign1 == sign2)
        {
            addition(&head1, &tail1, &head2, &tail2, &res_head, &res_tail); // perform addition if both numbers have same sign
            final_sign = sign1;
        }
        else
        {
            int cmp = compare(&head1, &tail1, &head2, &tail2); // compare the two numbers to determine which is larger in magnitude
            if (cmp == 0)
            {
                dl_insert_last(&res_head, &res_tail, 0);
                final_sign = 1;
            }
            else if (cmp == 1)
            {
                subtraction(&head1, &tail1, &head2, &tail2, &res_head, &res_tail); // perform subtraction if first number is larger in magnitude
                final_sign = sign1;
            }
            else
            {
                subtraction(&head2, &tail2, &head1, &tail1, &res_head, &res_tail); // perform subtraction if second number is larger in magnitude
                final_sign = sign2;
            }
        }
        
        printf("\t\t\t");
        printf(BG_CYAN ""BOLD" RESULTS " RESET " " "\n\n");
        printf(CYAN " ┌──────────────────────────────────────────────────────────┐" RESET "\n");
        printf(CYAN " │ " RESET " Summation : " GREEN BOLD);

    }


    /* ------------------------------ Subtraction: A - B ------------------------------- */

    else if (strcmp(argv[2], "-") == 0)
    {
        printf("\t\t");
        printf(" " BLUE BOLD "● OPERATION:" RESET " Subtraction (-)\n\n");  // set color and print operation

        if (sign1 != sign2)
        {
            addition(&head1, &tail1, &head2, &tail2, &res_head, &res_tail); // perform addition if the numbers have different signs
            final_sign = sign1;
        }
        else
        {
            int cmp = compare(&head1, &tail1, &head2, &tail2); // compare the two numbers to determine which is larger in magnitude
            if (cmp == 0)
            {
                dl_insert_last(&res_head, &res_tail, 0);
                final_sign = 1;
            }
            else if (cmp == 1)
            {
                subtraction(&head1, &tail1, &head2, &tail2, &res_head, &res_tail);
                final_sign = sign1;
            }
            else
            {
                subtraction(&head2, &tail2, &head1, &tail1, &res_head, &res_tail); // perform subtraction if second number is larger in magnitude
                final_sign = -sign1;
            }
        }

        printf("\t\t\t");
        printf(BG_CYAN ""BOLD" RESULTS " RESET " " "\n\n");
        printf(CYAN " ┌──────────────────────────────────────────────────────────┐" RESET "\n");
        printf(CYAN " │ " RESET " Difference : " GREEN BOLD);

    }

    /* ----------------------- Multiplication: A * B ----------------------- */
    
    else if (strcmp(argv[2], "x") == 0)
    {
        printf("\t\t");
        printf(" " MAGENTA BOLD "● OPERATION:" RESET " Multiplication (×)\n\n"); // set color and print operation

        Multiplication(&head1, &tail1, &head2, &tail2, &res_head, &res_tail); // perform multiplication 
        
        final_sign = sign1 * sign2; //  determine the sign of the result based on the signs of the operands

        printf("\t\t\t");
        printf(BG_CYAN ""BOLD" RESULTS " RESET " " "\n\n");
        printf(CYAN " ┌──────────────────────────────────────────────────────────┐" RESET "\n");
        printf(CYAN " │ " RESET " Product : " GREEN BOLD);
    }

    /* --------------------------- Division: A / B --------------------------- */
    
    else if (strcmp(argv[2], "/") == 0)
    {
        printf("\t\t");
        printf(" " YELLOW BOLD "● OPERATION:" RESET " Division (÷)\n\n"); // set color and print operation

        Remove_zero(&head1, &tail1, &head2, &tail2); // remove leading zeros from both numbers to ensure accurate comparison and division

        if (head2->data == 0 && head2->next == NULL) // check for division by zero and print error message
        {
            printf("\t\t\t"BG_RED " MATH ERROR " RESET " \n\n");
            printf(CYAN " ┌──────────────────────────────────────────────────────────┐" RESET "\n\n");
            printf(RED BOLD " [✖] Math Error: " RESET "Division by zero is undefined.\n\n");
            printf(CYAN " └──────────────────────────────────────────────────────────┘" RESET "\n\n"); 
            dl_delete_list(&head1, &tail1);
            dl_delete_list(&head2, &tail2);
            return FAILURE;
        }

        division(&head1, &tail1, &head2, &tail2, &res_head, &res_tail);
        final_sign = sign1 * sign2;

            printf("\t\t\t");
            printf(BG_CYAN ""BOLD" RESULTS " RESET " " "\n\n");
            printf(CYAN " ┌──────────────────────────────────────────────────────────┐" RESET "\n");
            printf(CYAN " │ " RESET " Quotient : " GREEN BOLD);


    }

    /* Formatted Result Display */
/* 
    printf("\t\t\t");
    printf(BG_CYAN ""BOLD" RESULTS " RESET " " "\n\n");
    printf(CYAN " ┌──────────────────────────────────────────────────────────┐" RESET "\n");
    printf(CYAN " │ " RESET " Output : " GREEN BOLD);   */

    if (final_sign < 0 && !is_list_zero(res_head)) // print negative sign if the result is negative and not zero
    {
        printf("-");
    }

    print_list(res_head);

     // Explicitly reset colors BEFORE the newline and bottom frame
    printf(RESET "\n");
    printf(CYAN " └──────────────────────────────────────────────────────────┘" RESET "\n\n");

    // Free all dynamically allocated memory

    dl_delete_list(&head1, &tail1); 
    dl_delete_list(&head2, &tail2);
    dl_delete_list(&res_head, &res_tail);

    return SUCCESS;
}

/*  END  */
