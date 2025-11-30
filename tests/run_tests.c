#include "token_fixtures.h"
#include "shell.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

char *get_abs_path(const char *relative_path)
{
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd() error");
		return NULL;
	}
	char *abs_path = malloc(strlen(cwd) + strlen(relative_path) + 2);
	sprintf(abs_path, "%s/%s", cwd, relative_path);
	return abs_path;
}

char *read_file_content(const char *path)
{
	int fd = open(path, O_RDONLY);
	if (fd < 0)
		return (NULL);
	char *content = malloc(4096);
	if (!content) return NULL;
	int bytes_read = read(fd, content, 4095);
	if (bytes_read >= 0)
		content[bytes_read] = '\0';
	else
		content[0] = '\0';
	close(fd);
	return (content);
}

int	run_and_check(t_token *tokens, char **envp, char *test_name, int expected_status, const char* expected_output_file)
{
	t_shell	*shell;
	int		exit_status;
	int		stdout_fd;
	char	tmp_out_path[] = "tests/out/test_output.tmp";
	int		tmp_fd;
	char	*buffer;
	char	*expected_output;

	printf("\n--- Test: %s ---\n", test_name);

	stdout_fd = dup(STDOUT_FILENO);
	tmp_fd = open(tmp_out_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(tmp_fd, STDOUT_FILENO);
	close(tmp_fd);

	shell = create_shell(envp);
	shell->build(shell, tokens);
	shell->execute(shell);
	exit_status = shell->ctx->last_exit_status;
	shell->free(shell);

	dup2(stdout_fd, STDOUT_FILENO);
	close(stdout_fd);

	buffer = read_file_content(tmp_out_path);
	expected_output = expected_output_file ? read_file_content(expected_output_file) : NULL;

	printf("Expected status: %d, Got: %d\n", expected_status, exit_status);
	if (expected_output && buffer)
	{
		if (strcmp(buffer, expected_output) != 0)
			printf("Expected output:\n---\n%s\n---\nGot:\n---\n%s\n---\n", expected_output, buffer);
	}

	if (exit_status == expected_status && (!expected_output || (buffer && strcmp(buffer, expected_output) == 0)))
	{
		printf("✅ PASSED\n");
		free(buffer);
		free(expected_output);
		return (0);
	}
	else
	{
		printf("❌ FAILED\n");
		free(buffer);
		free(expected_output);
		return (1);
	}
}

void absolute_path_tests(char **envp, int *failures)
{
    printf("\n--- Running Absolute Path Tests ---\n");
    *failures += run_and_check(tokens_abs_ls(), envp, "/bin/ls", 0, NULL);
    *failures += run_and_check(tokens_abs_pwd(), envp, "/bin/pwd", 0, NULL);
    *failures += run_and_check(tokens_abs_echo(), envp, "/bin/echo hello", 0, "hello\n");
}

void relative_path_tests(char **envp, int *failures)
{
    printf("\n--- Running Relative Path Tests ---\n");
    *failures += run_and_check(tokens_rel_ls(), envp, "ls", 0, NULL);
    *failures += run_and_check(tokens_rel_pwd(), envp, "pwd", 0, NULL);
    *failures += run_and_check(tokens_rel_echo(), envp, "echo hello relative", 0, "hello relative\n");
}

void redirection_tests(char **envp, int *failures)
{
    printf("\n--- Running Redirection Tests ---\n");
    *failures += run_and_check(tokens_redir_in(), envp, "cat < tests/in/simple.txt", 0, "This is a simple file.\nIt has a few lines.\nThe third line is here.\nEnd of file.\n");
    *failures += run_and_check(tokens_redir_out(), envp, "echo 'redirect out' > tests/out/redir_out.txt", 0, "");
    *failures += run_and_check(tokens_redir_append(), envp, "echo 'redirect append' >> tests/out/redir_append.txt", 0, "");
    *failures += run_and_check(tokens_redir_in_out(), envp, "cat < tests/in/simple.txt > tests/out/redir_in_out.txt", 0, "");
    *failures += run_and_check(tokens_redir_heredoc(), envp, "cat << EOF", 0, "heredoc line\n");
}

void pipe_tests(char **envp, int *failures)
{
    printf("\n--- Running Pipe Tests ---\n");
    *failures += run_and_check(tokens_pipe_simple(), envp, "ls -l | wc -l", 0, NULL);
    *failures += run_and_check(tokens_pipe_double(), envp, "cat tests/in/lines.txt | sort | uniq", 0, "alpha\nbravo\ncharlie\ndelta\ngamma\n");
    *failures += run_and_check(tokens_pipe_grep(), envp, "cat tests/in/lines.txt | grep bravo", 0, "bravo\nbravo\n");
    *failures += run_and_check(tokens_pipe_long(), envp, "cat tests/in/lines.txt | grep a | sort | uniq | wc -l", 0, "      4       4      28\n");
}

void mixed_command_tests(char **envp, int *failures)
{
    printf("\n--- Running Mixed Command Tests ---\n");
    *failures += run_and_check(tokens_mixed_redir_pipe(), envp, "cat < tests/in/lines.txt | wc -l", 0, "      7       7      49\n");
    *failures += run_and_check(tokens_mixed_pipe_redir(), envp, "cat tests/in/lines.txt | sort > tests/out/sorted_lines.txt", 0, "");
    *failures += run_and_check(tokens_mixed_heredoc_grep(), envp, "grep 'world' << EOF\nhello world\nEOF", 0, "hello world\n");
    *failures += run_and_check(tokens_mixed_and(), envp, "mkdir tests/out/tmp_dir && rmdir tests/out/tmp_dir", 0, "");
    *failures += run_and_check(tokens_mixed_or(), envp, "cat non_existent_file || echo 'fallback'", 0, "fallback\n");
}


int main(int argc, char **argv, char **envp)
{
	int failures = 0;

	(void)argc;
	(void)argv;

	printf("Running minishell tests...\n");

    absolute_path_tests(envp, &failures);
    relative_path_tests(envp, &failures);
    redirection_tests(envp, &failures);
    pipe_tests(envp, &failures);
    mixed_command_tests(envp, &failures);

	printf("\n----------------------------------------\n");
	if (failures == 0)
		printf("All tests passed!\n");
	else
		printf("%d test(s) failed.\n", failures);
	printf("----------------------------------------\n");

	return (failures > 0);
}
