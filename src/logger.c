/*****************************************************************************
 *
 * $Id$
 * Purpose ...............: Logging functions
 *
 *****************************************************************************
 * Copyright (C) 2003 
 *   
 * Michiel Broek		FIDO:		2:280/2802
 * Beekmansbos 10
 * 1971 BV IJmuiden
 * the Netherlands
 *
 * This file is part of Next User door for Unix.
 *
 * This BBS is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * MBSE BBS is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with MBSE BBS; see the file COPYING.  If not, write to the Free
 * Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *****************************************************************************/

#include "../config.h"
#include "doorlib.h"


char	    *_door_logfile = NULL;
char	    *_door_doorname = NULL;
int	    _door_logdebug = FALSE;


static char *_mon[] = {
    (char *)"Jan",(char *)"Feb",(char *)"Mar", (char *)"Apr",(char *)"May",(char *)"Jun",
    (char *)"Jul",(char *)"Aug",(char *)"Sep", (char *)"Oct",(char *)"Nov",(char *)"Dec"
};



char *date(void);
char *date(void)
{
    struct tm   ptm;
    time_t      now;
    static char buf[20];

    now = time(NULL);
    ptm = *localtime(&now);
    sprintf(buf,"%02d-%s-%04d %02d:%02d:%02d", ptm.tm_mday, _mon[ptm.tm_mon], ptm.tm_year+1900,
		ptm.tm_hour, ptm.tm_min, ptm.tm_sec);
    return(buf);
}



int door_loginit(char *fname, char *dname, int debug)
{
    _door_logfile  = door_xstrcpy(fname);
    _door_doorname = door_xstrcpy(dname);
    _door_logdebug = debug;
    return TRUE;
}



void door_log(int grade, const char *format, ...)
{
    va_list va_ptr;
    char    outstr[1024];
    FILE    *fp;

    if (_door_logfile == NULL) {
	/*
	 * Logfile not set or not available
	 */
	return;
    }
    
    if (grade == '+' || grade == '-' || grade == '!' || grade == '?' || grade == ' ' || _door_logdebug) {
	va_start(va_ptr, format);
	vsprintf(outstr, format, va_ptr);
	va_end(va_ptr);

	if ((fp = fopen(_door_logfile, "a")) == NULL)
	    return;

	fprintf(fp, "%c %s ", grade, date());
	if (_door_doorname)
	    fprintf(fp, "%s", _door_doorname);
	else
	    fprintf(fp, "doorlib");
	fprintf(fp, "[%d] ", getpid());
	fprintf(fp, *outstr == '$' ? outstr+1 : outstr);
	if (*outstr == '$')
	    fprintf(fp, ": %s\n", strerror(errno));
	else
	    fprintf(fp, "\n");

	fflush(fp);
	fclose(fp);
    }
}

