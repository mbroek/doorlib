/*****************************************************************************
 *
 * $Id$
 * Purpose ...............: Input functions.
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


int rawset = FALSE;

extern time_t	game_over;
time_t		time2go = 0;


/*
 * internal prototypes
 */
void		doorsetraw(void);
void		door_unsetraw(void);
int		Waitchar(unsigned char *, int);
int		Escapechar(unsigned char *);
unsigned char	Readkey(void);


/*
 * Sets raw mode and saves the terminal setup
 */
void door_setraw(void)
{
    int	    rc;

    if ((rc = tcgetattr(ttyfd, &tbufs))) {
	perror("");
	fprintf(stderr, "tcgetattr(0, save) return %d\n", rc);
	exit(101);
    }

    tbufsavs = tbufs;
    tbufs.c_iflag &= ~(INLCR | ICRNL | ISTRIP | IXON  ); /* IUCLC removed for FreeBSD */
    /*
     *  Map CRNL modes strip control characters and flow control
     */
    tbufs.c_oflag &= ~OPOST;            /* Don't do ouput character translation */
    tbufs.c_lflag &= ~(ICANON | ECHO);  /* No canonical input and no echo       */
    tbufs.c_cc[VMIN] = 1;               /* Receive 1 character at a time        */
    tbufs.c_cc[VTIME] = 0;              /* No time limit per character          */
    
    if ((rc = tcsetattr(ttyfd, TCSADRAIN, &tbufs))) {
	perror("");
	fprintf(stderr, "tcsetattr(%d, TCSADRAIN, raw) return %d\n", ttyfd, rc);
	exit(101);
    }

    rawset = TRUE;
}



/*
 * Unsets raw mode and returns state of terminal
 */
void door_unsetraw()
{
    int	    rc;

    /*
     * Only unset the mode if it is set to raw mode
     */
    if (rawset == TRUE) {
	if ((rc = tcsetattr(ttyfd, TCSAFLUSH, &tbufsavs))) {
	    perror("");
	    fprintf(stderr, "tcsetattr(%d, TCSAFLUSH, save) return %d\n", ttyfd, rc);
	    exit(101);
	}
    }
    rawset = FALSE;
}



/* 
 * This function is used to get a single character from a user ie for a 
 * menu option
 */
unsigned char door_getch(void)
{
    unsigned char   c = 0;
    
    if ((ttyfd = open ("/dev/tty", O_RDWR|O_NONBLOCK)) < 0) {
	perror("door_getch");
	exit(101);
    }
    door_setraw();

    c = Readkey();

    door_unsetraw();
    close(ttyfd);
    return(c);
}



/*
 *  Wait for a character for a maximum of wtime * 10 mSec.
 */
int Waitchar(unsigned char *ch, int wtime)
{
    int     i, rc = -1;

    for (i = 0; i < wtime; i++) {
	rc = read(ttyfd, ch, 1);
	if (rc == 1)
	    return rc;
	usleep(10000);
    }
    return rc;
}



int Escapechar(unsigned char *ch)
{
    int             rc;
    unsigned char   c;
		            
    /* 
     * Escape character, if nothing follows within 
     * 50 mSec, the user really pressed <esc>.
     */
    if ((rc = Waitchar(ch, 5)) == -1)
	return rc;

    if (*ch == '[') {
	/*
    	 *  Start of CSI sequence. If nothing follows,
	 *  return immediatly.
	 */
	if ((rc = Waitchar(ch, 5)) == -1)
	    return rc;

	/*
	 *  Test for the most important keys. Note
	 *  that only the cursor movement keys are
	 *  guaranteed to work with PC-clients.
	 */
	c = *ch;
	if (c == 'A')
	    c = KEY_UP;
	if (c == 'B')
	    c = KEY_DOWN;
	if (c == 'C')
	    c = KEY_RIGHT;
	if (c == 'D')
	    c = KEY_LEFT;
	if ((c == '1') || (c == 'H') || (c == 0))
	    c = KEY_HOME;
	if ((c == '4') || (c == 'K') || (c == 101) || (c == 144))
	    c = KEY_END;
	if (c == '2')
	    c = KEY_INS;
	if (c == '3')
	    c = KEY_DEL;
	if (c == '5')
	    c = KEY_PGUP;
	if (c == '6')
	    c = KEY_PGDN;
	memcpy(ch, &c, sizeof(unsigned char));
	return rc;
    }
    return -1;
}



/*
 *  This next function will detect the grey keys on the keyboard for
 *  VT100, VT220, Xterm, PC-ANSI, and Linux console. Works with 
 *  several terminals on serial lines (tested 1200 bps).
 *  If for example cursur keys are detected, this function returns
 *  a translated value.
 */
unsigned char Readkey(void)
{
    unsigned char   ch = 0;
    int             rc = -1;
    time_t	    Now;

    while (rc == -1) {
	/*
	 * Check if we reached the timeout set by the alarm timers.
	 */
	Now = time(NULL);
	if (game_over) {
	    if (Now >= game_over) {
		door_unsetraw();
		fprintf(stdout, "\rIdle timeout, door finished\n\n");
		fflush(stdout);
		exit(0);
	    }
	}
	if (time2go) {
	    if (Now >= time2go) {
		door_unsetraw();
		fprintf(stdout, "\rDaily timelimit reached, door finished\n\n");
		fflush(stdout);
		exit(0);
	    }
	}
	rc = Waitchar(&ch, 5);

	/*
	 * If the character is not an Escape character,
	 * then this function is finished.
	 */
	if ((rc == 1) && (ch != KEY_ESCAPE))
	    return ch;

	if ((rc == 1) && (ch == KEY_ESCAPE)) {
	    rc = Escapechar(&ch);
	    if (rc == 1)
		return ch;
	    else
		return KEY_ESCAPE;
	}
    }

    return(ch);
}



/*
 * Get a character string with cursor position
 */
void door_getstrp(char *sStr, int iMaxLen, int Position)
{
    unsigned char   ch = 0;
    int		    iPos = Position;

    if ((ttyfd = open("/dev/tty", O_RDWR|O_NONBLOCK)) < 0) {
	perror("door_getstrp");
	return;
    }
    door_setraw();
    door_alarm_on();

    while (ch != KEY_ENTER) {

	fflush(stdout);
	ch = Readkey();

	if ((ch == KEY_BACKSPACE) || (ch == KEY_DEL) || (ch == KEY_RUBOUT)) {
	    if (iPos > 0) {
		printf("\b \b");
		sStr[--iPos] = '\0';
	    } else
		putchar('\007');
	}

	if (ch > 31 && ch < 127) {
	    if (iPos <= iMaxLen) {
		iPos++;
		sprintf(sStr, "%s%c", sStr, ch);
		printf("%c", ch);
	    } else
		putchar('\007');
	}
    }

    door_unsetraw();
    close(ttyfd);
    printf("\n");
}



/*
 * Get a character string
 */
void door_getstr(char *sStr, int iMaxlen)
{
    unsigned char   ch = 0;
    int		    iPos = 0;

    fflush(stdout);
    if ((ttyfd = open ("/dev/tty", O_RDWR|O_NONBLOCK)) < 0) {
	perror("door_getstr");
	return;
    }
    door_setraw();
    strcpy(sStr, "");
    door_alarm_on();

    while (ch != 13) {

	fflush(stdout);
	ch = Readkey();

	if ((ch == 8) || (ch == KEY_DEL) || (ch == 127)) {
	    if (iPos > 0) {
		printf("\b \b");
		sStr[--iPos] = '\0';
	    } else
		putchar('\007');
	}

	if ((ch > 31) && (ch < 127) && (ch != ',')) {
	    if (iPos <= iMaxlen) {
		iPos++;
		sprintf(sStr, "%s%c", sStr, ch);
		printf("%c", ch);
	    } else
		putchar('\007');
	}
    }

    door_unsetraw();
    close(ttyfd);
    printf("\n");
}



/*
 * Get a username, captitalize first letters.
 */
void door_getname(char *sStr, int iMaxlen)
{
    unsigned char   ch = 0; 
    int             iPos = 0, iNewPos = 0;

    fflush(stdout);
    strcpy(sStr, "");

    if ((ttyfd = open ("/dev/tty", O_RDWR|O_NONBLOCK)) < 0) {
	perror("door_getname");
	return;
    }
    door_setraw();
    door_alarm_on();

    while (ch != 13) {
	fflush(stdout);
	ch = Readkey();

	if ((ch == 8) || (ch == KEY_DEL) || (ch == 127)) {
	    if (iPos > 0) {
		printf("\b \b");
		sStr[--iPos]='\0';
	    } else {
		putchar('\007');
	    }
	}

	if (ch > 31 && (ch < 127)) {
	    if (iPos < iMaxlen) {
		iPos++;
		if (iPos == 1)
		    ch = toupper(ch);

		if (ch == 32) {
		    iNewPos = iPos;
		    iNewPos++;
		}
		if (iNewPos == iPos)
		    ch = toupper(ch);
		else
		    ch = tolower(ch);
		if (iPos == 1)
		    ch = toupper(ch);
		sprintf(sStr, "%s%c", sStr, ch);
		printf("%c", ch);
	    } else {
		putchar('\007');
	    }
	}
    }

    door_unsetraw();
    close(ttyfd);
    printf("\n");
}



/*
 * Get a number, allow digits, spaces, minus sign, points and comma's
 */
void door_getnum(char *sStr, int iMaxlen)
{
    unsigned char   ch = 0; 
    int             iPos = 0;

    fflush(stdout);

    if ((ttyfd = open ("/dev/tty", O_RDWR|O_NONBLOCK)) < 0) {
	perror("door_getnum");
	return;
    }
    door_setraw();
    strcpy(sStr, "");
    door_alarm_on();

    while (ch != 13) {
	fflush(stdout);
	ch = Readkey();
	
	if ((ch == 8) || (ch == KEY_DEL) || (ch == 127)) {
	    if (iPos > 0) {
		printf("\b \b");
		sStr[--iPos]='\0';
	    } else {
		putchar('\007');
	    }
	}

	if ((ch >= '0' && ch <= '9') || (ch == '-') || (ch == ' ') || (ch == ',') || (ch == '.')) {
	    if (iPos <= iMaxlen) {
		iPos++;
		sprintf(sStr, "%s%c", sStr, ch);
		printf("%c", ch);
	    } else {
		putchar('\007');
	    }
	}
    }

    door_unsetraw();
    close(ttyfd);
    printf("\n");
}



void door_waitenter(void)
{
    int	    i, x;
    char    *string;

    string = malloc(81);

    sprintf(string, "\rPress (Enter) to continue: ");
    door_color(WHITE, BLACK);
    printf(string);
	
    do {
	fflush(stdout);
	fflush(stdin);
	door_alarm_on();
	i = door_getch();
    } while ((i != '\r') && (i != '\n'));

    x = strlen(string);
    for(i = 0; i < x; i++)
	printf("\b");
    for(i = 0; i < x; i++)
	printf(" ");
    for(i = 0; i < x; i++)
	printf("\b");
    fflush(stdout);

    free(string);
}


