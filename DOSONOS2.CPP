// file tim_exec.cpp timed program execution with automatic
// killing of program after a specified number of hours, minutes and seconds.

#define INCL_DOSPROCESS
#define INCL_DOSSESMGR
#define INCL_NOPMAPI
#include <os2.h>

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

void usage(char * p)
{
    printf(
        "TIM_EXEC, DOS on OS/2 timed task run/terminate program, by William Herrera.\n"
        "Version 1.01 of 5 May 1992. May be freely distributed and used.\n"
        "Usage:\n"
        "  %s progname -d[days] -h[hours] -m[minutes] -s[seconds] -a[Arguments]\n"
        "\nExample:\n"
        "  %s doit -h2 -m13 -s49 -a\"c: d:\"\n"
        "    will run the MSDOS program doit with arguments c: d:\n"
        "      for 2 hours, 13 minutes, 49 seconds.\n\n"
        "Note: To execute an OS/2 file, use a program name of \"\"\n"
        "  and put \"/c progname args\" for arguments, as in\n"
        "    %s \"\" -m10 -a\"/c doit.cmd args\"\n"
    , p, p, p);
}

int main(int argc, char * argv[])
{
    long days = 0;
    long hours = 0;
    long minutes = 0;
    long seconds = 0;
    long duration = 0;
    PID pidChild;
    STARTDATA   StartData;    /* Start session data structure */
    ULONG       SessID;       /* Session ID (returned) */
    char       PgmTitle[200]; /* Program title string */
    char       PgmName[128];  /* Program pathname string */
    char       PgmArgs[128];
    char       ObjBuf[100];  /* Object buffer */
    APIRET rc;
    static char * title = "Timed Execution of Process";

    int i = argc;
    if(i < 2)
    {
        usage(argv[0]);
        return 1;
    }
    strcpy(PgmName, argv[1]);

    for(i = 2; i < argc; ++i)
	{
		char buf[256];
        char * p = buf;
        strcpy(p, argv[i]);
		if(*p == '-' || *p == '/')
		{
			++p;
			switch(toupper(*p++))
			{
			case '?':
				usage(argv[0]);
				return 0;
            case 'A':
                strcpy(PgmArgs, p);
                break;
			case 'H':
				hours = atoi(p);
				break;
			case 'D':
				days = atoi(p);
				break;
			case 'M':
				minutes = atoi(p);
				break;
			case 'S':
				seconds = atoi(p);
				break;
			default:
                usage(argv[0]);
                return 1;
				break;
			}
		}
	}
 
    duration = days * 24;
    duration += hours;     // now in hours
    duration *= 60;
    duration += minutes;  // now in minutes
    duration *= 60;
    duration += seconds;  // now in seconds
    duration *= 1000;     // now in milliseconds

    StartData.Length = sizeof(STARTDATA);
                           /* Length of STARTDATA structure */

    StartData.Related = SSF_RELATED_CHILD;
                           /* Independent session */

    StartData.FgBg = SSF_FGBG_BACK;
                           /* Start session in background */

    StartData.TraceOpt = SSF_TRACEOPT_NONE;
                           /* Don't trace session */

    sprintf(PgmTitle, "%s -- %s", title, PgmName);
    StartData.PgmTitle = PgmTitle;
                           /* Session Title string */

    StartData.PgmName = PgmName;
                           /* Program path-name string */

    StartData.PgmInputs = PgmArgs;
                           /* Input arguments passed to the program */

    StartData.TermQ = 0;   /* Assume no termination queue */

    StartData.Environment = 0;
                           /* Assume no environment string */

    StartData.InheritOpt = SSF_INHERTOPT_SHELL;
                           /* Inherit environment and open */
                           /*   file handles from shell   */

    StartData.SessionType = SSF_TYPE_DEFAULT;
                           /* Allow the Shell to establish */
                           /*   the session type           */

    StartData.IconFile = 0;
                           /* Assume no specific icon file */
                           /*   is provided                */

    StartData.PgmHandle = 0;
                           /* Do not use the installation file */

    StartData.PgmControl = SSF_CONTROL_VISIBLE
                             | SSF_CONTROL_MAXIMIZE;
                           /* Start the program as visible */
                           /*   and maximized              */

    StartData.InitXPos = 30;
    StartData.InitYPos = 40;
    StartData.InitXSize = 200;    /* Initial window coordinates */
    StartData.InitYSize = 140;    /*   and size                 */

    StartData.Reserved = 0;
                           /* Reserved, must be zero */

    StartData.ObjectBuffer = ObjBuf;
                           /* Object buffer to hold DosExecPgm */
                           /*   failure causes                 */

    StartData.ObjectBuffLen = 100;
                           /* Size of object buffer */

    printf(
            "\nBeginning run of %s.\n"
            "Program to be run:  '%s'.\n"
            "Argument list:      '%s'.\n"
            "Allowable run time:\n"
            "  %d days,\n"
            "  %d hours,\n"
            "  %d minutes, and\n"
            "  %d seconds.\n"
            "To see the program running, press Ctrl-Esc and choose the task entitled\n"
            "  '%s'.\nTo leave the child process running indefinitely, press Esc key.\n"
            , argv[0], argv[1], PgmArgs, days, hours, minutes, seconds, PgmTitle);

    rc = DosStartSession(&StartData, &SessID, &pidChild);
                          /* On successful return, the variable  */
                          /*   SessID contains the session ID    */
                          /*   of the new session, and the       */
                          /*   variable PID contains the process */
                          /*   ID of the new process             */

    if (rc != 0)
      {
        printf("DosStartSession error: return code = %ld", rc);
        return 3;
      }

    long n = duration / 960;
    while(n-- > 0)
    {
        DosSleep(959);
        if(kbhit() && getch() == 27)
        {
            printf("\nSession '%s' will run indefinitely.\n"
                "  To terminate, press Ctrl-C.\n",
                PgmTitle);
            while(1)
                DosSleep(10000);
        }
    }

    DosKillProcess(DKP_PROCESSTREE, pidChild);

    return 0;
}


