#define _XOPEN_SOURCE 700
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static pid_t pid;

static void usage() {
    printf("usage: container-init <command> [...]\n");
}

static void forward(int signo) {
    printf("Forwarding signal %d\n", signo);
    if (kill(pid, signo) < 0) perror("signal forwarding error");
}

int main(int argc, char **argv)
{
    //sigset_t set;

    // if (getpid() != 1) {
    //     printf("not running as pid 1\n");
    //     usage();
    //     return 1;
    // }

    if (argc < 1) {
        printf("missing command\n");
        usage();
        return 1;
    }

    // sigfillset(&set);
    // sigprocmask(SIG_BLOCK, &set, 0);

    pid = fork();

    if (pid < 0) {
        perror("fork error");
        return 1;
    }

    if (pid == 0) {
        int err;

        // sigprocmask(SIG_UNBLOCK, &set, 0);
        // setsid();
        // setpgid(0, 0);

        argv++;
        err = execv(argv[0], argv);
        perror("exec error");
        return 1;
    }

    // else pid == 1

    int status;
    pid_t current;

    // TODO: Add all signals
    signal(SIGHUP, forward);
    signal(SIGINT, forward);
    signal(SIGKILL, forward);
    signal(SIGQUIT, forward);
    signal(SIGTERM, forward);
    signal(SIGUSR1, forward);
    signal(SIGUSR2, forward);

    for (;;) {
        current = wait(&status);
        printf("current %d\n", current);
        if (current == pid) {
            return status;
        }
        if (current < 0) {
            perror("wait error");
        }
    }
}

