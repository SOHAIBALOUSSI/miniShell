#include "../minishell.h"

int execute_subshell(t_tree *root)
{
    pid_t pid;
    int status;

    // printf("Executing root...\n");
    // fflush(stdout);
    pid = fork();
    if (pid < 0)
    {
        pop_error("fork");
        return (1);
    }
    else if (pid == 0)
    {
        if (root->redir_list)
            handle_redirections(root->redir_list);
        mshell()->exit_status = execute_ast(root->subtree);
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