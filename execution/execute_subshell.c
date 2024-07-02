#include "../minishell.h"

int execute_subshell(t_tree *root)
{
    pid_t pid;
    int status;

    printf("Executing subshell...\n");
    fflush(stdout);
    pid = fork();
    if (pid < 0)
    {
        pop_error("fork");
        return (1);
    }
    else if (pid == 0)
    {
        g_shell.exit_status = execute_ast(root->subtree);
        exit(g_shell.exit_status);
    }
    else
    {
        waitpid(pid, &status, 0);
        g_shell.exit_status = WEXITSTATUS(status);
    }
    printf("Subshell execution finished with status: %d\n", g_shell.exit_status);
    fflush(stdout);
    return (g_shell.exit_status);
}