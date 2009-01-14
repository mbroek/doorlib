/*****************************************************************************
 *
 * $Id$
 * Purpose ...............: Display ANSI or ASCII file to user
 *
 *****************************************************************************
 * Copyright (C) 2003-2006
 *   
 * Michiel Broek		FIDO:		2:280/2802
 * Beekmansbos 10
 * 1971 BV IJmuiden
 * the Netherlands
 *
 * This file is part of doorlib for Unix.
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * Doorlib is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with MBSE BBS; see the file COPYING.  If not, write to the Free
 * Software Foundation, 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *****************************************************************************/

#include "../config.h"
#include "door.h"


extern int	termmode;	    /* 0 = tty, 1 = ANSI		    */
extern int	time2go;	    /* Unix time when users time is over    */


void ControlCodeF(int);
void ControlCodeK(int);
void ControlCodeU(int);



/*
 * Display a textfile in ansi or ascii to the user. The full path
 * and filename is needed as parameter except the filename extension.
 * If the users ANSI is off, only the .asc file is searched for.
 */
int door_dispfile(char *textfile)
{
    FILE    *fp;
    char    *temp, *buf;
    int	    i, x;

    temp = calloc(PATH_MAX, sizeof(char));

    if (termmode) {
	snprintf(temp, PATH_MAX, "%s.ans", textfile);
	if ((fp = fopen(temp, "r")) == NULL) {
	    snprintf(temp, PATH_MAX, "%s.asc", textfile);
	    if ((fp = fopen(temp, "r")) == NULL) {
		door_log('?', "$Can't open %s", DOOR_SS(textfile));
		free(temp);
		return FALSE;
	    }
	}
    } else {
	snprintf(temp, PATH_MAX, "%s.asc", textfile);
	if ((fp = fopen(temp, "r")) == NULL) {
	    door_log('?', "$Can't open %s", DOOR_SS(textfile));
	    free(temp);
	    return FALSE;
	}
    }

    buf = calloc(16385, sizeof(char));
    door_log('+', "Displayfile %s", temp);

    while (!feof(fp)) {
	i = fread(buf, sizeof(char), 16384, fp);

	for (x = 0; x < i; x++) {
	    switch (*(buf + x)) {
		case '':  fflush(stdout);
			    fflush(stdin);
			    door_alarm_on();
			    door_getch();
			    break;

		case '':  ControlCodeF(buf[++x]);
			    break;

		case '':  ControlCodeK(buf[++x]);
			    break;
	
		case '':  door_clear();
			    break;

		case '':  fflush(stdout);
			    sleep(1);
			    break;

		case '':  ControlCodeU(buf[++x]);
			    break;
			    
		default:    printf("%c", *(buf + x));
	    }
	}
    }

    fclose(fp);
    free(buf);
    free(temp);
    return TRUE;
}


void ControlCodeF(int ch)
{
    switch (toupper(ch)) {
	case '!':
		    printf("%s", doorsys.protocol);
		    break;
	case 'A':
		    printf("%d", doorsys.uploads);
		    break;
	case 'B':
		    printf("%d", doorsys.downloads);
		    break;
	case 'C':
		    printf("%d", doorsys.downloadK);
		    break;
	case 'D':
		    printf("%d", doorsys.uploadK);
		    break;
	case 'E':
		    printf("%d", doorsys.downloadK + doorsys.uploadK);
		    break;
	case 'F':
		    printf("%d", doorsys.lim_downk); 
		    break;
	case 'G':
		    printf("%d", doorsys.downs_today);
		    break;
	default:
		    printf(" ");
    }
}



void ControlCodeU(int ch)
{
    char    *token, FirstName[81], LastName[81], UserName[81];
    int	    i, x;
    int	    Now;

    /*
     * Split username in firstname and lastname if possible.
     */
    strncpy(UserName, doorsys.username, 80);
    if ((strchr(UserName, ' ') != NULL)) {
	token = strtok(UserName, " ");
	strcpy(FirstName, token);
	token = strtok(NULL, "\0");
	i = strlen(token);
	for (x = 2; x < i; x++) {
	    if (token[x] == ' ')
		token[x] = '\0';
	}
	strcpy(LastName, token);
    } else {
	strcpy(FirstName, UserName);
	LastName[0] = '\0';
    }
    
    switch (toupper(ch)) {
	case 'A':
		    printf("%s", doorsys.username);
		    break;
	case 'B':
		    printf(doorsys.location);
		    break;
	case 'C':
		    printf(doorsys.voice_phone);
		    break;
	case 'D':
		    printf(doorsys.data_phone);
		    break;
	case 'E':
		    printf(doorsys.last_login);
		    break;
	case 'F':
		    printf("%s", doorsys.comment);
		    break;
	case 'G':
		    printf("%s", doorsys.last_login_time);
		    break;
	case 'H':
		    printf("%d", doorsys.seclevel);
		    break;
	case 'I':
		    printf("%d", doorsys.totalcalls);
		    break;
	case 'J':
		    printf("%s", doorsys.expiry_date);
		    break;
	case 'K':
		    Now = (int)time(NULL);
		    printf("%d", (time2go - Now));
		    break;
	case 'L':
		    Now = (int)time(NULL);
		    printf("%d", ((time2go - Now) / 60));
		    break;
	case 'M':
		    printf("%d", doorsys.screenlen);
		    break;
	case 'N':
		    printf(FirstName);
		    break;
	case 'O':
		    printf(LastName);
		    break;
	case 'P':
		    printf("%s", termmode ? "Yes":"No ");
		    break;
	case 'Q':
		    printf("%d", doorsys.defcolor);
		    break;
	case 'T':
		    printf(doorsys.date_of_birth);
		    break;
	case 'U':
		    printf("%d", doorsys.posted);
		    break;
	case 'Y':
		    printf(doorsys.handle);
		    break;
	default:
		    printf(" ");
    }
}



void ControlCodeK(int ch)
{
    time_t	Now;
    struct tm	*tm;
    
    Now = time(NULL);
    tm = localtime(&Now);
    
    switch (toupper(ch)) {
	case 'A':
		    printf("%02d-%02d-%04d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
		    break;
	case 'B':
		    printf("%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);
		    break;
	case 'C':
		    printf("%s", doorsys.comport);
		    break;
	case 'D':
		    printf("%d", doorsys.lbaud);
		    break;
	case 'E':
		    printf("%d", doorsys.ebaud);
		    break;
	case 'F':
		    printf("%d", doorsys.nodenumber);
		    break;
	case 'G':
		    printf("%s", doorsys.errorfree);
		    break;
	default:
		    printf(" ");
    }
}


