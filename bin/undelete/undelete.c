/* $NetBSD: rm.c,v 1.50 2011/08/29 14:48:46 joerg Exp $ */

/*-
 * Copyright (c) 1990, 1993, 1994, 2003
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
#ifndef lint
__COPYRIGHT("@(#) Copyright (c) 1990, 1993, 1994\
 The Regents of the University of California.  All rights reserved.");
#endif /* not lint */

#ifndef lint
#if 0
static char sccsid[] = "@(#)rm.c	8.8 (Berkeley) 4/27/95";
#else
__RCSID("$NetBSD: rm.c,v 1.50 2011/08/29 14:48:46 joerg Exp $");
#endif
#endif /* not lint */

#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <fts.h>
#include <grp.h>
#include <locale.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef __minix
# ifndef O_SYNC
#  define O_SYNC 0
# endif
# ifndef O_RSYNC
#  define O_RSYNC 0
# endif
#endif


/*
 * undelete
 */
int
main(int argc, char *argv[])
{
    int rv;

    setprogname(argv[0]);
    (void)setlocale(LC_ALL, "");

    if(argc != 2)
    {
		printf("Format Error. Usage Example: undelete [Filename]");
        rv = -1;
    }
    else if(argc == 2)
    {
        rv = fundelete(argv[1]);
		if(rv != OK)
		{
			if(rv == EINVAL){
			    printf("Error: Invalid Parameter");
		    }
		    else if (rv == EBUSY){
			    printf("Error: Timer Is Busy");
		    }
		    else if (rv == EROFS){
			    printf("Error: Write Protected");
	    	}
		    else{
		    	printf("Error: Other Errors")
		    }
		}
		
    }
    exit(rv);
    /* NOTREACHED */
}

