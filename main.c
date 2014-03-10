#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdbool.h>

#define BUFFER_LENGTH           (256)
#define MILI_TO_MICRO(mili)		(mili)*1000
#define STDIN					fileno(stdin)

static volatile bool g_running = true;

void signal_handler(int signum) {
	g_running = false;
}

void remove_special_characters(char *line, int length) {
	int i;
	for (i = 0; i < length; i++) {
		if (line[i] == '\n' || line[i] == '\f' || line[i] == '\r') {
			line[i] = 0;
		}
	}
}

int main(int argc, char **argv) {
	char line[BUFFER_LENGTH];
	fd_set read_set;
	struct timeval timeout;

	signal(SIGUSR1, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGINT, signal_handler);

	while (g_running) {

		timeout.tv_sec  = 1;
		timeout.tv_usec = 0;

		FD_ZERO(&read_set);
		FD_SET(STDIN, &read_set);

		select(STDIN+1, &read_set, NULL, NULL, &timeout);
		if(FD_ISSET(STDIN, &read_set) && g_running) {
			if (fgets(line, BUFFER_LENGTH, stdin) != NULL) {
				remove_special_characters(&line[0], BUFFER_LENGTH);
				printf("Line received: %s!\n", &line[0]);
			}
		}

		if(g_running) {
			usleep(timeout.tv_usec);
			printf("Output line, time = %ld\n", (long int)time(NULL));
		}
		fflush(stdout);
	}

	return 0;
}

