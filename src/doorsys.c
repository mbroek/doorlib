/*****************************************************************************
 *
 * $Id$
 * Purpose ...............: Read door.sys
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


extern time_t	time2go;
extern int	termmode;


void Striplf(char *String)
{
    int i;

    for (i = 0; i < strlen(String); i++) {
	if (*(String + i) == '\0')
	    break;
	if ((*(String + i) == '\n') || (*(String + i) == '\r') || (*(String + i) == '\032'))
	    *(String + i) = '\0';

    }
}


/*
 * Macro's for load the door.sys data
 */
#define	L_STR(result, str, fp) fgets(str, 80, fp); Striplf(str); result = door_xstrcpy(str);
#define L_INT(result, str, fp) fgets(str, 80, fp); Striplf(str); result = atoi(str);



int door_load_doorsys(void)
{
    char    *temp;
    FILE    *fp;
    time_t  Now;

    memset(&doorsys, 0, sizeof(doorsys));
    temp = calloc(PATH_MAX, sizeof(char));
    sprintf(temp, "door.sys");
    if ((fp = fopen(temp, "r")) == NULL) {
	sprintf(temp, "DOOR.SYS");
	if ((fp = fopen(temp, "r")) == NULL) {
	    fprintf(stderr, "\rCan't open door.sys in current directory\n");
	    fflush(stdout);
	    exit(100);
	}
    }

    /*
     * Read and process door.sys
     */
    L_STR(doorsys.comport, temp, fp);
    L_INT(doorsys.ebaud, temp, fp);
    L_INT(doorsys.databits, temp, fp);
    L_INT(doorsys.nodenumber, temp, fp);
    L_INT(doorsys.lbaud, temp, fp);
    L_STR(doorsys.screen_display, temp, fp);
    L_STR(doorsys.printer_on, temp, fp);
    L_STR(doorsys.page_bell, temp, fp);
    L_STR(doorsys.caller_alarm, temp, fp);
    L_STR(doorsys.username, temp, fp);
    L_STR(doorsys.location, temp, fp);
    L_STR(doorsys.voice_phone, temp, fp);
    L_STR(doorsys.data_phone, temp, fp);
    L_STR(doorsys.password, temp, fp);
    L_INT(doorsys.seclevel, temp, fp);
    L_INT(doorsys.totalcalls, temp, fp);
    L_STR(doorsys.last_login, temp, fp);
    L_INT(doorsys.sec_timeleft, temp, fp);
    L_INT(doorsys.min_timeleft, temp, fp);
    L_STR(doorsys.graphics, temp, fp);
    L_INT(doorsys.screenlen, temp, fp);
    L_STR(doorsys.usermode, temp, fp);
    L_STR(doorsys.extra1, temp, fp);
    L_STR(doorsys.extra2, temp, fp);
    L_STR(doorsys.expiry_date, temp, fp);
    L_INT(doorsys.grecno, temp, fp);
    L_STR(doorsys.protocol, temp, fp);
    L_INT(doorsys.uploads, temp, fp);
    L_INT(doorsys.downloads, temp, fp);
    L_INT(doorsys.lim_downk, temp, fp);
    L_INT(doorsys.lim_downk2, temp, fp);
    L_STR(doorsys.date_of_birth, temp, fp);
    L_STR(doorsys.userbase, temp, fp);
    L_STR(doorsys.msgbase, temp, fp);
    L_STR(doorsys.sysopname, temp, fp);
    L_STR(doorsys.handle, temp, fp);
    L_STR(doorsys.nextevent, temp, fp);
    L_STR(doorsys.errorfree, temp, fp);
    L_STR(doorsys.allwaysN, temp, fp);
    L_STR(doorsys.allwaysY, temp, fp);
    L_INT(doorsys.defcolor, temp, fp);
    L_INT(doorsys.allways0, temp, fp);
    L_STR(doorsys.last_login2, temp, fp);
    L_STR(doorsys.time_login, temp, fp);
    L_STR(doorsys.last_login_time, temp, fp);
    L_INT(doorsys.maxinteger, temp, fp);
    L_INT(doorsys.downs_today, temp, fp);
    L_INT(doorsys.uploadK, temp, fp);
    L_INT(doorsys.downloadK, temp, fp);
    L_STR(doorsys.comment, temp, fp);
    L_INT(doorsys.allways0_2, temp, fp);
    L_INT(doorsys.posted, temp, fp);

    /*
     * Set daily logout time
     */
    Now = time(NULL);
    if (doorsys.sec_timeleft)
	time2go = Now + doorsys.sec_timeleft;
    else if (doorsys.min_timeleft)
	time2go = Now + (doorsys.min_timeleft * 60);
    else
	time2go = 0;

    if (strcmp(doorsys.graphics, "GR") == 0)
	termmode = 1;
    else
	termmode = 0;

    fclose(fp);
    free(temp);
    return TRUE;
}


