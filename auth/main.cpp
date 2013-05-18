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

static void interrupt_handler(int signo) {
    if (lockf(lockfd, F_ULOCK, 0) < 0) exit(EXIT_FAILURE);
    remove("qh3client.lock");

    exit(EXIT_FAILURE);
}

void daemonize() {
    if (getpid() == 1) return;
    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    else if (pid > 0) exit(EXIT_SUCCESS);

    setsid();
    for (int i = getdtablesize(); i >= 0; -- i)
        close(i);
    int nullfd = open("/dev/null", O_RDWR);
    dup(nullfd);
    dup(nullfd);

    umask(027);
    chdir("/tmp");

    lockfd = open("qh3client.lock", O_RDWR | O_CREAT, 0640);
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
}

int stop() {
    FILE *fp = fopen("/tmp/qh3client.lock", "r");
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

    QCoreApplication a(argc, argv);

    EAPDaemon daemon;
    new EAPDaemonAdapter(&daemon);

    return a.exec();
}
