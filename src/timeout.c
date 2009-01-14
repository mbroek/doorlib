/*****************************************************************************
 *
 * $Id$
 * Purpose ...............: Timeout functions
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


int	game_over;		    /* Local time at when time is over	*/


void door_alarm_set(int val)
{
    int	    Now;

    if (val) {
	Now = (int)time(NULL);
	game_over = Now + val;
    } else {
	game_over = 0;
    }
}



void door_alarm_on(void)
{
    door_alarm_set(60 * 10);
}



void door_alarm_off(void)
{
    door_alarm_set(0);
}


