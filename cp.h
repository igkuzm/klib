/**
 * File              : cp.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 04.09.2021
 * Last Modified Date: 26.05.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#ifndef k_lib_cp_h__
#define k_lib_cp_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

//copy file FROM path to destination TO path
int k_lib_cp(const char *from, const char *to);

#ifndef cp
#define cp(from, to)	\
({	\
	int ___c = k_lib_cp(from, to);\
	___c;	\
})
#endif

int k_lib_cp(const char *from, const char *to)
{
    int fd_to, fd_from;
    char buf[4096];
    ssize_t nread;
    int saved_errno;

    fd_from = open(from, O_RDONLY);
    if (fd_from < 0)
        return -1;

	fd_to = open(to, O_WRONLY | O_CREAT | O_EXCL, 0666);
    
	if (fd_to < 0)
        goto out_error;

    while (nread = read(fd_from, buf, sizeof buf), nread > 0)
    {
        char *out_ptr = buf;
        ssize_t nwritten;

        do {
            nwritten = write(fd_to, out_ptr, nread);

            if (nwritten >= 0)
            {
                nread -= nwritten;
                out_ptr += nwritten;
            }
            else if (errno != EINTR)
            {
                goto out_error;
            }
        } while (nread > 0);
    }

    if (nread == 0)
    {
        if (close(fd_to) < 0)
        {
            fd_to = -1;
            goto out_error;
        }
        close(fd_from);

        /* Success! */
        return 0;
    }

  out_error:
    saved_errno = errno;

    close(fd_from);
    if (fd_to >= 0)
        close(fd_to);

    errno = saved_errno;
    return -1;
}

#ifdef __cplusplus
}
#endif

#endif //k_lib_cp_h__
