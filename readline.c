#include "minishell.h"

int main()
{
  /* compile with -lreadline */
  char *line = readline("minishell :");
  while (strcmp(line, "exit") != 0)
  {
    printf("%s\n", line);
    if (line)
    {
      add_history(line);
      free(line);
    }
    line = readline("minishell  :");
  }
  /* history logs */
  for (int i = history_base; i < history_length; i++) {
    printf("%d: %s\n", i, history_get(i)->line);
    }
  if (line)
    free(line);
    return (0);
}
