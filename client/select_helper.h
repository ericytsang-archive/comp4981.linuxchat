/**
 * header file that exposes interface used to provide wrapper functions used to
 *   add file descriptors to a set, and lets you select them.
 *
 * @sourceFile select_helper.h
 *
 * @program    N/A
 *
 * @function   void files_init(Files* files)
 * @function   int files_select(Files* files)
 * @function   void files_add_file(Files* files, int newFd)
 * @function   void files_rm_file(Files* files, int fd)
 *
 * @date       2015-03-23
 *
 * @revision   none
 *
 * @designer   Eric Tsang
 *
 * @programmer Eric Tsang
 *
 * @note       none
 */
#ifndef _SELECT_HELPER_H_
#define _SELECT_HELPER_H_

#include <stdio.h>
#include <sys/select.h>
#include <set>

/**
 * files structure that's used by the functions in this file. it provides
 *   wrapper functions that let you do what select does.
 */
typedef struct
{
    std::set<int> fdSet;    // set of all file descriptors
    fd_set _selectFds;      // set of all file descriptors for internal uses
    fd_set selectFds;       // set of selected file descriptors
    int maxFd;              // integer corresponding to biggest file descriptor
} Files;

void files_init(Files* files);
int files_select(Files* files);
void files_add_file(Files* files, int newFd);
void files_rm_file(Files* files, int fd);

#endif
