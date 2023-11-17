#include "shell.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


/**
 * main - Entry point for the simple_shell program.
 * Return: Always 0.
 */
int main(void)
{
    char *input;

    while (1)
    {
        display_prompt();

        if ((input = get_input()) == NULL)
        {
            printf("\n");
            break; /* EOF (Ctrl+D) detected */
        }

        if (execute_command(input) == -1)
            fprintf(stderr, "Error executing command: %s\n", input);

        free(input); /* Free the memory allocated by getline */
    }

    return EXIT_SUCCESS;
}

/**
 * display_prompt - Displays the shell prompt.
 */
void display_prompt(void)
{
    printf("RK-->");
}

/**
 * get_input - Get input from the user.
 * Return: Pointer to the input buffer.
 */
/**
 * get_input - Get input from the user
 * @input: Buffer to store the input
 * Return: Pointer to the input buffer
 */
void *get_input(void)
{
    char *input = NULL;
    size_t len;
    size_t input_len;
    
    printf("$ ");
    


    if (getline(&input, &len, stdin) == -1)
    {
        if (feof(stdin))
            exit(EXIT_SUCCESS);
        perror("getline");
        exit(EXIT_FAILURE);
    }

    /* Now you can use input_len */
    input_len = strlen(input);
    if (input_len > 0)
    {
       printf(" Input length: %lu\n", (unsigned long)input_len);
    }
		       
      
    
    return input;
}



/**
 * execute_command - Executes a command entered by the user.
 * @command: The command to execute.
 * Return: 0 on success, -1 on failure.
 */
int execute_command(const char *command)
{
    pid_t child_pid = fork();

    if (child_pid == -1)
    {
        perror("fork");
        return -1;
    }

    if (child_pid == 0) /* Child process */
    {
        char **args = malloc(2 * sizeof(char *));
        if (args == NULL)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        args[0] = (char *)command;
        args[1] = NULL;

        if (execvp(command, args) == -1)
        {
            perror(command);
            free(args);
            exit(EXIT_FAILURE);
        }

        free(args);
    }
    else /* Parent process */
    {
        int status;
        if (waitpid(child_pid, &status, 0) == -1)
        {
            perror("waitpid");
            return -1;
        }

        if (WIFEXITED(status) && WEXITSTATUS(status) == 127)
            fprintf(stderr, "%s: command not found\n", command);
    }

    return 0;
}
