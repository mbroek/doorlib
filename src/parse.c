/*****************************************************************************
 *
 * $Id$
 * Purpose ...............: Configuration file parser
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


static char	    *door_k, *door_v;
static int	    door_linecnt = 0;
static char	    *door_configname;



/*
 * Get a string
 */
int door_parse_getstr(char **dest)
{
    *dest = door_xstrcpy(door_v);
    return 0;
}



/*
 * Get a long
 */
int door_parse_getlong(char **dest)
{
    if (strspn(door_v,"0123456789") != strlen(door_v)) 
	fprintf(stderr,"%s(%d): %s %s - bad numeric\n", DOOR_SS(door_configname), door_linecnt, DOOR_SS(door_k), DOOR_SS(door_v));
    else 
	*((long*)dest) = atol(door_v);
    return 0;
}



/*
 * Parse configuration file
 */
int door_parse_config(char *configfile, keytab_t keytab[])
{
    int	    i, rc = FALSE;
    FILE    *fp;
    char    *p, buf[256];

    if ((fp = fopen(configfile, "r")) == NULL) {
	perror("");
	fprintf(stderr, "Can't open %s\n", configfile);
	return TRUE;
    }
    door_configname = door_xstrcpy(configfile);

    while (fgets(buf, sizeof(buf) -1, fp)) {
	door_linecnt++;
	if (*(p = buf + strlen(buf) -1) != '\n') {
	    fprintf(stderr, "%s(%d): \"%s\" - line too long\n", configfile, door_linecnt, buf);
	    rc = TRUE;
	    break;
	}
	*p-- = '\0';
	while ((p >= buf) && isspace(*p))
	    *p-- = '\0';
	door_k = buf;
	while (*door_k && isspace(*door_k))
	    door_k++;
	p = door_k;
	while (*p && !isspace(*p))
	    p++;
	*p++='\0';
	door_v = p;
	while (*door_v && isspace(*door_v))
	    door_v++;

	if ((*door_k == '\0') || (*door_k == '#')) {
	    continue;
	}
	
	for (i = 0; keytab[i].key; i++)
	    if (strcasecmp(door_k,keytab[i].key) == 0)
		break;

	if (keytab[i].key == NULL) {
	    fprintf(stderr, "%s(%d): %s %s - unknown keyword\n", configfile, door_linecnt, DOOR_SS(door_k), DOOR_SS(door_v));
	    rc = TRUE;
	    break;
	} else if ((keytab[i].prc(keytab[i].dest))) {
	    rc = TRUE;
	    break;
	}
    }
    fclose(fp);

    return rc;
}



