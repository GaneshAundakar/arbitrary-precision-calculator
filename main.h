#ifndef MAIN_H      
#define MAIN_H

/* ANSI Color & Style Codes */
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"

/* Colors */
#define RED         "\033[1;31m"
#define GREEN       "\033[1;32m"
#define YELLOW      "\033[1;33m"
#define BLUE        "\033[1;34m"
#define MAGENTA     "\033[1;35m"
#define CYAN        "\033[1;36m"
#define WHITE       "\033[1;37m"

/* UI Badges */
#define BG_CYAN     "\033[46;30m"
#define BG_RED      "\033[41;37m"

#define CLEAR_SCREEN "\033[H\033[J"

#define SUCCESS 0
#define FAILURE -1

typedef struct node
{
	int data;
	struct node *prev;
	struct node *next;
}Dlist;

/* Math functions*/

int addition(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **res_head, Dlist **res_tail);

int subtraction(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **res_head, Dlist **res_tail);

int Multiplication (Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **res2_head, Dlist **res2_tail);

int division (Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **res_head, Dlist **res_tail);

/* DLL functions */

int dl_insert_first(Dlist **head1, Dlist **tail1, int data);

int dl_insert_last(Dlist **head1, Dlist **tail1, int data);

int dl_delete_first(Dlist **head1, Dlist **tail1);

int dl_delete_list(Dlist **head1, Dlist **tail1);

void print_list(Dlist *head1);

/* Other functions */

int validate_number_arg(const char *str);

int is_list_zero(Dlist *head); 

void Remove_zero(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2);

int compare(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2);

int convert(char str[], Dlist **head1, Dlist **tail1);

#endif