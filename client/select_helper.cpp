/**
 * implementation of select_helper.h
 *
 * @sourceFile set_helper.cpp
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
#include "select_helper.h"

/**
 * initializes a files set. the set will have no files in it.
 *
 * @function   files_init
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
 *
 * @signature  void files_init(Files* files)
 *
 * @param      files files set to initialize.
 */
void files_init(Files* files)
{
    FD_ZERO(&files->_selectFds);
    files->maxFd = 0;
}

/**
 * performs select on the files set, and returns what select returns.
 *
 * @function   files_select
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
 *
 * @signature  int files_select(Files* files)
 *
 * @param      files files set to select
 *
 * @return     whatever select returns
 */
int files_select(Files* files)
{
    files->selectFds = files->_selectFds;
    return select(files->maxFd+1 , &files->selectFds, 0, 0, 0);
}

/**
 * adds a file descriptor to the files set
 *
 * @function   files_add_file
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
 *
 * @signature  void files_add_file(Files* files, int newFd)
 *
 * @param      files files set to perform the operation on.
 * @param      newFd file descriptor to add to the files set.
 */
void files_add_file(Files* files, int newFd)
{
    // add the file to our sets
    FD_SET(newFd, &files->_selectFds);
    files->fdSet.insert(newFd);

    // update maxFd if needed
    if(newFd > files->maxFd)
    {
        files->maxFd = newFd;
    }
}

/**
 * removes a file descriptor from the files set
 *
 * @function   files_rm_file
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
 *
 * @signature  void files_rm_file(Files* files, int fd)
 *
 * @param      files files set to perform the operation on.
 * @param      fd file descriptor to add to the files set.
 */
void files_rm_file(Files* files, int fd)
{
    // remove the file from our sets
    FD_CLR(fd, &files->_selectFds);
    files->fdSet.erase(fd);

    // update maxFd if needed
    if(fd == files->maxFd)
    {
        files->maxFd = 0;
        for(auto fd = files->fdSet.begin(); fd != files->fdSet.end(); ++fd)
        {
            if(*fd > files->maxFd)
            {
                files->maxFd = *fd;
            }
        }
    }
}
