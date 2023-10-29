#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <threads.h>
#include <unistd.h>
#include <utils.h>

FILE *file = NULL;
const char *filename = NULL;
char *cwd = NULL;
char *dir = NULL;

void fetch_dir(char *arg0)
{
    int execlen = strlen(arg0) - 4;
    if ((dir = strndup(arg0, execlen)) == NULL)
        splperror("cannot duplicate path");
    cwd = (char *)malloc(65536);
    if ((cwd = getcwd(cwd, 65536)) == NULL)
        splperror("cannot get current working directory");
    char *newcwd = (char *)realloc(cwd, (strlen(cwd) + 1)* sizeof(char));
    if (newcwd == NULL)
        splperror("failed to reallocate memory for char* newcwd");
    cwd = newcwd;
}

static void process_args(int argc, char *argv[])
{
    if (argc != 2)
    {
        splperror("expected exactly one file as argument");
    }
    if ((file = fopen(argv[1], "r")) != NULL)
    {
        fclose(file);
        filename = strdup(argv[1]);
    }
    else
    {
        fprintf(stderr, "splc: \033[31mfatal error:\033[0m %s: no such file\ncompilation terminated.\n", argv[1]);
        exit(1);
    }
    fetch_dir(argv[0]);
}

void invoke_spl_parser()
{
    int pid, status;
    pid = fork();
    if (pid < 0)
        splperror("failed to call spl_parser");
    else if (pid == 0)
    {
        // concatenate paths to call spl_parser
        char *cwdpdir = concat_with_insertion(cwd, dir, '/');
        char *orig_path = getenv("PATH");
        char *tmp_path = concat_with_insertion(cwdpdir, orig_path, ':');
        char *final_path = concat_with_insertion(cwd, tmp_path, ':');
        free(tmp_path);
        tmp_path = final_path;
        final_path = concat_with_insertion(dir, tmp_path, ':');
        free(tmp_path);
        setenv("PATH", final_path, 1);

        if (execlp("spl_parser", "spl_parser", strdup(filename), (char *)NULL) == -1)
        {
            printf("spl_parser: ");
            perror(NULL);
            splperror("failed to call spl_parser");
        }
        exit(1);
    }
    else
    {
        int rpid = wait(&status);
        if (rpid == -1 || !WIFEXITED(status))
            splperror("failed to wait for response from spl_parser");
    }
}

int main(int argc, char *argv[])
{
    // printf("This program only supports preprocessing and parsing right now.\n");
    process_args(argc, argv);
    // ================================
    // Get path to other executables

    // ================================
    // Call spl_preprocessor, spl_parser
    invoke_spl_parser();

    return 0;
}