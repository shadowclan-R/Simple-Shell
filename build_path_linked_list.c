#include "shell.h"

/**
 * build_path_linked_list - Builds a linked list of directories in the PATH.
 * Return: Pointer to the head of the linked list.
 */
void build_path_linked_lists(void)
{
    char *path_variable = "PATH";
    char *path_value = _getenv(path_variable);

    if (path_value != NULL)
    {
        path_list_t *head = NULL;
        char *path_copy = strdup(path_value);  /* Make a copy to avoid modifying the original */
	char *directory;  /* Declare directory at the beginning */
	
        if (path_copy == NULL)
        {
            perror("strdup");
            exit(EXIT_FAILURE);
        }


        directory = strtok(path_copy, ":");  /* Initialize directory here */

        while (directory != NULL)
        {
            add_node_end(&head, directory);
            directory = strtok(NULL, ":");
        }

        print_path_list(head);
        free_path_list(head);

        free(path_copy);  /* Free the allocated copy */
    }
    else
    {
        printf("PATH variable not found.\n");
    }
}

/**
 * add_node_end - Add a new node at the end of the linked list
 * @head: A pointer to the head of the linked list
 * @directory: The directory to be added to the linked list
 * Return: A pointer to the newly added node
 */
path_list_t *add_node_end(path_list_t **head, char *directory)
{
    path_list_t *new_node = malloc(sizeof(path_list_t));
    path_list_t *last = *head;

    if (new_node == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    new_node->directory = strdup(directory);
    new_node->next = NULL;

    if (*head == NULL)
    {
        *head = new_node;
    }
    else
    {
        while (last->next != NULL)
        {
            last = last->next;
        }
        last->next = new_node;
    }

    return new_node;
}

/**
 * print_path_list - Prints each node in the linked list.
 * @head: Pointer to the head of the linked list.
 */
void print_path_list(path_list_t *head)
{
    while (head != NULL)
    {
        printf("%s\n", head->directory);
        head = head->next;
    }
}

/**
 * free_path_list - Frees the memory allocated for the linked list.
 * @head: Pointer to the head of the linked list.
 */
void free_path_list(path_list_t *head)
{
    while (head != NULL)
    {
        path_list_t *temp = head;
        head = head->next;
        free(temp->directory);
        free(temp);
    }
}
