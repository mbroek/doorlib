#ifndef	_DOORLIB_H
#define	_DOORLIB_H

/* $Id$ */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <stdarg.h>
#include <errno.h>


#ifndef	TRUE
#define	TRUE	1
#define	FALSE	0
#endif


/*
 *  Returned function keys
 */
#define KEY_BACKSPACE   8
#define KEY_LINEFEED    10
#define KEY_ENTER       13
#define KEY_ESCAPE      27
#define KEY_RUBOUT      127
#define KEY_UP          200
#define KEY_DOWN        201
#define KEY_LEFT        202
#define KEY_RIGHT       203
#define KEY_HOME        204
#define KEY_END         205
#define KEY_INS         206
#define KEY_DEL         207
#define KEY_PGUP        208
#define KEY_PGDN        209



/*
 * ANSI colors
 */
#define BLACK           0
#define BLUE            1
#define GREEN           2
#define CYAN            3
#define RED             4
#define MAGENTA         5
#define BROWN           6
#define LIGHTGRAY       7
#define DARKGRAY        8
#define LIGHTBLUE       9
#define LIGHTGREEN      10
#define LIGHTCYAN       11
#define LIGHTRED        12
#define LIGHTMAGENTA    13
#define YELLOW          14
#define WHITE           15



/*
 * Some ANSI sequences
 */
#define ANSI_RED        "\x1B[31;1m"
#define ANSI_YELLOW     "\x1B[33;1m"
#define ANSI_BLUE       "\x1B[34;1m"
#define ANSI_GREEN      "\x1B[32;1m"
#define ANSI_WHITE      "\x1B[37;1m"
#define ANSI_CYAN       "\x1B[36;1m"
#define ANSI_MAGENTA    "\x1B[35m"

#define ANSI_HOME       "\x1B[H"
#define ANSI_UP         "\x1B[A"
#define ANSI_DOWN       "\x1B[B"
#define ANSI_RIGHT      "\x1B[C"
#define ANSI_LEFT       "\x1B[D"

#define ANSI_BOLD       "\x1B[1m"
#define ANSI_NORMAL     "\x1B[0m"
#define ANSI_CLEAR      "\x1B[2J"
#define ANSI_CLREOL     "\x1B[K"


/*
 * Handy macros
 */
#define	DOOR_SS(x) (x)?(x):"(null)"


int		ttyfd;              /* Filedescriptor for raw mode  */
struct termios	tbufs, tbufsavs;    /* Structure for raw mode	    */


typedef struct	_keytab {	    /* Configuration table	    */
    char	*key;		    /* Keyword			    */
    int		(*prc)(char **);    /* Function to parse keyword    */
    char**	dest;		    /* Destination value to fill    */
} keytab_t;


typedef struct	_doorsys {	    /* 52 lines door.sys file	    */
    char	*comport;	    /* DOS style COM port	    */
    int		ebaud;		    /* Effective baudrate	    */
    int		databits;	    /* Databits			    */
    int		nodenumber;	    /* Multiline nodenumber	    */
    int		lbaud;		    /* Locked baudrate		    */
    char	*screen_display;    /* Screen display Y/N	    */
    char	*printer_on;	    /* Printer on Y/N		    */
    char	*page_bell;	    /* Paging bell Y/N		    */
    char	*caller_alarm;	    /* Caller alarm Y/N		    */
    char	*username;	    /* User name		    */
    char	*location;	    /* User's location		    */
    char	*voice_phone;	    /* User's voice phone	    */
    char	*data_phone;	    /* User's data phone	    */
    char	*password;	    /* User's password		    */
    int		seclevel;	    /* User's security level	    */
    int		totalcalls;	    /* User's total calls	    */
    char	*last_login;	    /* last login date MM-DD-YYYY   */
    int		sec_timeleft;	    /* Seconds time left	    */
    int		min_timeleft;	    /* Minutes time left	    */
    char	*graphics;	    /* Graphics GR or NG	    */
    int		screenlen;	    /* User's screen length	    */
    char	*usermode;	    /* Always Y ??		    */
    char	*extra1;
    char	*extra2;
    char	*expiry_date;	    /* User's expiry date	    */
    int		grecno;		    /* User's record number	    */
    char	*protocol;	    /* Transfer protocol	    */
    int		uploads;	    /* Total uploads		    */
    int		downloads;	    /* Total downloads		    */
    int		lim_downk;	    /* Download limit in Kb.	    */
    int		lim_downk2;	    /* Download limit in Kb.	    */
    char	*date_of_birth;	    /* Date of birth		    */
    char	*userbase;	    /* Path to userbase		    */
    char	*msgbase;	    /* Path to msgbase		    */
    char	*sysopname;	    /* Sysop name		    */
    char	*handle;	    /* User's alias		    */
    char	*nextevent;	    /* Next event time or none	    */
    char	*errorfree;	    /* Error free connection	    */
    char	*allwaysN;
    char	*allwaysY;
    int		defcolor;	    /* Default textcolor	    */
    int		allways0;
    char	*last_login2;	    /* Last login date		    */
    char	*time_login;	    /* Login time		    */
    char	*last_login_time;   /* Last login time		    */
    int		maxinteger;	    /* DOS maxint value		    */
    int		downs_today;	    /* Downloads today		    */
    int		uploadK;	    /* Uploads in Kb		    */
    int		downloadK;	    /* Downloads in Kb		    */
    char	*comment;	    /* User's comment		    */
    int		allways0_2;
    int		posted;		    /* Messages posted		    */
} doorsys_t;

doorsys_t	doorsys;	    /* Loaded door.sys data	    */


unsigned char	door_getch(void);
void		door_getstrp(char *, int, int);
void		door_getstr(char *, int);
void		door_getname(char *, int);
void		door_getnum(char *, int);
void		door_waitenter(char *);
void		door_enter(int);
void		door_cout(int, int, char *);
void		door_color(int, int);
void		door_center(char *);
void		door_clear(void);
void		door_locate(int, int);
void		door_line(int);
void		door_alarm_set(int);
void		door_alarm_on(void);
void		door_alarm_off(void);
char		*door_xmalloc(size_t);
char		*door_xstrcpy(char *);
char		*door_xstrcat(char *, char *);
int		door_load_doorsys(void);
int		door_loginit(char *, char *, int);
void		door_log(int, const char *, ...);
int		door_parse_getstr(char **);
int		door_parse_getlong(char **);
int		door_parse_config(char *, keytab_t[]);
int		door_dispfile(char *);


#endif
