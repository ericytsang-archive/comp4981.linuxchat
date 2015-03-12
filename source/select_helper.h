#include <stdio.h>
#include <sys/select.h>
#include <QSet>

typedef struct
{
    QSet<int> fdSet;
    fd_set _selectFds;
    fd_set selectFds;
    int maxFd;      // integer corresponding to the biggest file descriptor
} Files;

void files_init(Files* files);
int files_select(Files* files);
void files_add_file(Files* files, int newFd);
void files_rm_file(Files* files, int fd);
