#include "../minishell.h"

int    actual_pipeline(t_tree **pipeline, int pc)
{
    pid_t   pid[pc];
    int     status;
    int     fd[pc - 1][2];
    int     i;

    i = 0;
    while (i < pc - 1)
    {
        if (pipe(fd[i]) < 0)
        {
            pop_error("Pipe creation failed\n");
            return (-1);
        }
        i++;
    }
    i = 0;
    while (i < pc) 
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
            if (i < pc - 1)
                dup2(fd[i][1], STDOUT_FILENO);
            int j = 0;  
            while (j < pc - 1) 
            {
                close(fd[j][0]);
                close(fd[j][1]);
                j++;
            }
            if (pipeline[i]->redir_list)
                handle_redirections(pipeline[i]->redir_list);
            status = execute_cmd(pipeline[i]);
            exit(status);
        }
        i++;
    }
    i = 0;
    while (i < pc - 1) 
    {
        close(fd[i][0]);
        close(fd[i][1]);
        i++;
    }
    i = 0;
    while (i < pc) 
    {
        waitpid(pid[i], &status, 0);
        g_shell.exit_status = WEXITSTATUS(status);
        i++;
    }

    g_shell.pipe_count = 0;
    return g_shell.exit_status;
}
