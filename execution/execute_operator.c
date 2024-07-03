#include "../minishell.h"

int execute_operator(t_tree *operator)
{
    int status;

    status = execute_ast(operator->left);
    if ((operator->type == _AND && status == 0)
        || (operator->type == _OR && status != 0))
        return (execute_ast(operator->right));
    return (status);
}