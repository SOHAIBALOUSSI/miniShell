// #include "../minishell.h"

// char *expand_arg(char *arg)
// {
// 	int idx;
// 	char *expanded;

// 	idx = 0;
// 	if (arg[idx] == '$')
// 	{

// 		expanded = get_value()
// 	}
	
	
// }

// /*
// 	TODO:
// 	expand_cmd()
// 		iterate argv and check args for expand
// 			if argv[i] has single quote , dont expand anything 
// 			if argv[i] has double quote , search for $KEY and expend it and mark it to be splited .
// 			if argv[i] has a $, Expand it w sff
// */
// void    expand_cmd(t_tree *root)
// {
// 	int     i;
// 	char    **expanded_argv;
// 	int     expanded_argc;
// 	char    *expanded;
// 	i = 0;
// 	expanded_argv = NULL;
// 	expanded_argc = 0;
// 	while (i < root->argc) 
// 	{
// 		expanded = expand_arg(root->argv[i]);
// 		if (expanded)
// 		{
// 			expanded_argv = m_realloc(expanded_argv, sizeof(char *) * expanded_argc, sizeof(char *) * (expanded_argc + 1));
// 			expanded_argv[expanded_argc] = expanded;
// 			expanded_argc++;
// 		}
// 		i++;
// 	}
// 	root->argc = expanded_argc;
// 	root->argv = expanded_argv;
// }

// void	expander(t_tree *root)
// {
// 	if (root->type == _CMD)
// 		expand_cmd(root);
// 	// expand_redir(root->redir_list);
// 	// expand_wildcards(root->argv);
// }