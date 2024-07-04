#include "../minishell.h"

int execute_subshell(t_tree *subshell)
{
    pid_t pid;
    int status;

    // printf("Executing subshell...\n");
    // fflush(stdout);
    pid = fork();
    if (pid < 0)
    {
        pop_error("fork");
        return (1);
    }
    else if (pid == 0)
    {
        mshell()->exit_status = execute_ast(subshell);
        exit(mshell()->exit_status);
    }
    else
    {
        waitpid(pid, &status, 0);
        mshell()->exit_status = WEXITSTATUS(status);
    }
    // printf("Subshell execution finished with status: %d\n", mshell()->exit_status);
    // fflush(stdout);
    return (mshell()->exit_status);
}