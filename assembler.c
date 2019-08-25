#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static void trim(char** str)
{
    int i = strlen(*str) - 1;
    while (i >= 0 && isspace((*str)[i]))
    {
	(*str)[i] = '\0';
	i--;
    }
    while (isspace(**str))
	(*str)++;
}

int tokenize(char *str, char delim, char*** tokens)
{
    int count = 1;
    int i = 0;
    char *pointer = str;
    char *token = NULL;
    /* Count the tokens. */
    while (*pointer)
    {
	if (*pointer == delim)
	    count++;
	pointer++;
    }
    
    if (tokens && *tokens)
    {
	*tokens = (char**)malloc(count*sizeof(char*));
    }
    pointer = str;
    token = str;
    while (count != 1 && *pointer)
    {
	if (*pointer == delim)
	{
	    *pointer = '\0';
	    trim(&token);
	    (*tokens)[i] = token;
	    token = pointer + 1;
	    i++;
	}
	pointer++;
    }
    trim(&token);
    (*tokens)[i] = token;
    
    return count;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
	printf("USAGE: ./assembler operation\n");
	return 1;
    }
    char ** tokens;
    int count = tokenize(argv[1], ',', &tokens);
    for (int i = 0; i < count; ++i)
    {
	printf("token: %s\n", tokens[i]);
    }
    if (tokens)
	free(tokens);
    return 0;
}
