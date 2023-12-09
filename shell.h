#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for command chaining */
#define C_NORMAL 0
#define C_OR 1
#define C_AND 2
#define C_CHAIN 3

/* for read/write buffers */
#define READ_BUFFER_SIZE 1024
#define WRITE_BUFFER_SIZE 1024
#define BUFFER_FLUSH -1

/* for convert_numbers() */
#define CONVERT_TO_LOWERCASE 1
#define CONVERT_TO_UNSIGNED 2

#define HIST_FILE ".simple_shell_history"
#define HIST_MAX 4096

/* 1 if using system getline() */
#define USING_GETLINE 0
#define USING_STRTOK 0

extern char **environ;

/**
 * struct strlist - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct strlist
{
	int num;
	char *str;
	struct strlist *next;
} list_t;

/**
 *struct infopass - contains pseudo-arguements to pass into a function,
 *					allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@linecount: the error count
 *@errnum: the error code for exit()s
 *@line_count_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buffer: address of pointer to cmd_buffer, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@read_fd: the fd from which to read line input
 *@history_count: the history line number count
 */
typedef struct infopass
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int linecount;
	int errnum;
	int line_count_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buffer; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type;  /* CMD_type ||, &&, ; */
	int read_fd;
	int history_count;
} info_t;

#define INFO_INIT                                             \
	{                                                         \
		NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, \
			NULL, 0, 0, NULL, 0, 0, 0                         \
	}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;

/* hshell.c */
void forking_cmd(info_t *);
int hshell(info_t *, char **);
void finding_cmd(info_t *);
int finding_builtin(info_t *);

/* error_string_functions.c */
int _puts_fd(char *str, int fd);
void _eputts(char *);
int _put_fd(char c, int fd);
int _eputcharacter(char);

/* paths.c */
char *finding_path(info_t *, char *, char *);
char *dupchars(char *, int, int);
int iscmd(info_t *, char *);

/* string_functions_1.c */
int _strcmp(char *, char *);
int _strlen(char *);
char *_strcat(char *, char *);
char *start_with(const char *, const char *);

/* string_functions_2.c */
int _putchar(char);
void _puts(char *);
char *_strdup(const char *);
char *_strcpy(char *, char *);

/* string_functions_3.c */
char *_strn_cpy(char *, char *, int);
char *_str_chr(char *, char);
char *_strn_cat(char *, char *, int);

/* string_functions_4.c */
char **str_tow(char *, char *);
char **str_tow_2(char *, char);

/* memory_functions_1 */
void *_re_alloc(void *, unsigned int, unsigned int);
void f_free(char **);
char *_mem_set(char *, char, unsigned int);

/* memory_functions_2.c */
int b_free(void **);

/* builtin_emulators_1.c */
int _my_exit(info_t *);
int _my_cd(info_t *);
int _my_help(info_t *);

/* builtin_emulators_2.c */
int _my_history(info_t *);
int _my_alias(info_t *);

/* more_functions_1.c */
int _is_alpha(int);
int is_delimiter(char, char *);
int _atoi(char *);
int inter(info_t *);

/* more_functions_2.c */
int printd(int, int);
void print_errors(info_t *, char *);
void remove_comment(char *);
char *convert_numbers(long int, int, int);
int _error_atoi(char *);

/* get_line.c module */
int _get_line(info_t *, char **, size_t *);
ssize_t get_input(info_t *);
void sigint_handler(int);

/* env_1.c module */
int _my_setenv(info_t *);
int _my_env(info_t *);
int _populate_env_list(info_t *);
int _my_unsetenv(info_t *);
char *_get_env(info_t *, const char *);

/* env_2.c module */
int _set_env(info_t *, char *, char *);
char **getEnviron(info_t *);
int _unset_env(info_t *, char *);

/* fileio_functions.c */
int history_write(info_t *info);
int history_renumber(info_t *info);
int history_read(info_t *info);
int buildhistory_list(info_t *info, char *buf, int linecount);
char *gethistory_file(info_t *info);

/* info.c module */
void info_free(info_t *, int);
void info_clear(info_t *);
void info_set(info_t *, char **);

/* strlist_1.c module */
list_t *node_add_end(list_t **, const char *, int);
list_t *node_add(list_t **, const char *, int);
int deletenode_at_index(list_t **, unsigned int);
size_t printlist_str(const list_t *);
void list_free(list_t **);

/* strlist_2.c module */
size_t lenlist(const list_t *);
size_t list_print(const list_t *);
char **list_2_strings(list_t *);
ssize_t getnode_index(list_t *, list_t *);
list_t *node_start_with(list_t *, char *, char);

/* chain.c */
int alias_replace(info_t *);
void chain_check(info_t *, char *, size_t *, size_t, size_t);
int ischain(info_t *, char *, size_t *);
int string_replace(char **, char *);
int vars_replace(info_t *);

#endif
