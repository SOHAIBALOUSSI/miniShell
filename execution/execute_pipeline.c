#include "../minishell.h"

int    actual_pipeline(t_tree **pipeline, int n_cmd)
{
    pid_t   pid[n_cmd];
    int     status;
    int     fd[n_cmd - 1][2];
    int     i;

    // printf("Executing pipeline with %d commands...\n", n_cmd);
    // fflush(stdout);
    i = 0;
    while (i < n_cmd - 1)
    {
        if (pipe(fd[i]) < 0)
        {
            pop_error("Pipe creation failed\n");
            return (-1);
        }
        i++;
    }
    i = 0;
    while (i < n_cmd) 
    {
        pid[i] = fork();
        if (pid[i] < 0) 
        {
            pop_error("Fork failed\n");
            return -1;
        } 
        else if (pid[i] == 0)
        {
            if (i > 0) 
                dup2(fd[i - 1][0], STDIN_FILENO);
            if (i < n_cmd - 1)
                dup2(fd[i][1], STDOUT_FILENO);
            int j = 0;  
            while (j < n_cmd - 1) 
            {
                close(fd[j][0]);
                close(fd[j][1]);
                j++;
            }
            if (pipeline[i]->redir_list)
                handle_redirections(pipeline[i]->redir_list);
            if (pipeline[i]->type == _SUBSHELL)
                execute_subshell(pipeline[i]);
            else 
                status = execute_cmd(pipeline[i]);
            exit(status);
        }
        i++;
    }
    i = 0;
    while (i < n_cmd - 1) 
    {
        close(fd[i][0]);
        close(fd[i][1]);
        i++;
    }
    i = 0;
    while (i < n_cmd) 
    {
        waitpid(pid[i], &status, 0);
        mshell()->exit_status = WEXITSTATUS(status);
        i++;
    }
    // printf("Pipeline execution finished with status: %d\n", mshell()->exit_status);
    // fflush(stdout);
    mshell()->pipe_count = 0;
    return (mshell()->exit_status);
}
