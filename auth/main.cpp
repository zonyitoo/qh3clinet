#include <QCoreApplication>
#include <QDebug>
#include "eapdaemon.h"
#include "eapdaemonadapter.h"
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <cstdio>
#include <signal.h>
#include <sys/stat.h>

int lockfd = -1;
const static char *lockfile = "/tmp/qh3client.lock";

static void interrupt_handler(int signo) {
    if (signo != SIGINT && signo != SIGTERM) return;

    remove(lockfile);
    exit(EXIT_FAILURE);
}

void daemonize() {
    if (getpid() == 1) return;
    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    setsid();

    close(fileno(stdin));
    close(fileno(stdout));
    close(fileno(stderr));

    int nullfd = open("/dev/null", O_RDWR);
    dup(nullfd);
    dup(nullfd);

    umask(027);
    chdir("/");

    lockfd = open(lockfile, O_RDWR | O_CREAT, 0640);
    if (lockfd < 0) exit(EXIT_FAILURE);
    if (lockf(lockfd, F_TLOCK, 0) < 0) exit(EXIT_SUCCESS);

    char pidstr[128] = {0};
    sprintf(pidstr, "%d\n", getpid());
    write(lockfd, pidstr, strlen(pidstr));

    signal(SIGCHLD, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGINT, interrupt_handler);
    signal(SIGTERM, interrupt_handler);
}

int stop() {
    FILE *fp = fopen(lockfile, "r");
    if (fp == nullptr) return 0;
    pid_t pid;
    fscanf(fp, "%d", &pid);
    int ret = kill(pid, SIGINT);
    fclose(fp);
    return ret;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        qWarning() << "Usage: qh3client-daemon [start|stop|restart]";
        exit(EXIT_FAILURE);
    }
    QString arg(argv[1]);
    if (arg == "start") {
        qWarning() << "qh3client-daemon Start";
        stop();
        daemonize();
    }
    else if (arg == "stop") {
        qWarning() << "qh3client-daemon Stop";
        return stop();
    }
    else if (arg == "restart") {
        qWarning() << "qh3client-daemon Restart";
        stop();
        daemonize();
    }
    else {
        fprintf(stderr, "argument error\n");
        exit(EXIT_FAILURE);
    }

    QCoreApplication a(argc, argv);

    EAPDaemon daemon;
    new EAPDaemonAdapter(&daemon);

    return a.exec();
}
