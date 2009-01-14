/*****************************************************************************
 *
 * $Id$
 * Purpose ...............: Memory allocation
 *
 *****************************************************************************
 * Copyright (C) 2003 
 *   
 * Michiel Broek		FIDO:		2:280/2802
 * Beekmansbos 10
 * 1971 BV IJmuiden
 * the Netherlands
 *
 * This file is part of doorlib for Unix.
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
 * Software Foundation, 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *****************************************************************************/

#include "../config.h"
#include "door.h"



char *door_xmalloc(size_t size)
{
    char *tmp;

    tmp = malloc(size);
    if (!tmp) 
	abort();
			            
    return tmp;
}



char *door_xstrcpy(char *src)
{
    char    *tmp;

    if (src == NULL) 
	return(NULL);
    tmp = door_xmalloc(strlen(src)+1);
    strcpy(tmp, src);
    return tmp;
}



char *door_xstrcat(char *src, char *add)
{
    char    *tmp;
    size_t  size = 0;

    if ((add == NULL) || (strlen(add) == 0))
	    return src;
    if (src)
	size = strlen(src);
    size += strlen(add);
    tmp = door_xmalloc(size + 1);
    *tmp = '\0';
    if (src) {
	strcpy(tmp, src);
	free(src);
    }
    strcat(tmp, add);
    return tmp;
}



