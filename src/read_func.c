#include "pihm.h"

int Readable (char *cmdstr)
{
    int             readable;

    if (cmdstr[0] != '#' && cmdstr[0] != '\n' && cmdstr[0] != '\0' &&
        cmdstr[0] != '\t')
        readable = 1;
    else
        readable = 0;

    return (readable);
}

int FindLine (FILE * fid, char *token)
{
    int             success = 0;
    char            cmdstr[MAXSTRING];
    char            optstr[MAXSTRING];

    rewind (fid);

    if (strcasecmp ("BOF", token) == 0)
        success = 1;
    else
    {
        /* Initialize cmdstr */
        strcpy (cmdstr, "\0");

        while (!feof (fid))
        {
            if (Readable (cmdstr))
            {
                sscanf (cmdstr, "%s", optstr);
                if (strcasecmp (token, optstr) == 0)
                {
                    success = 1;
                    break;
                }
            }

            fgets (cmdstr, MAXSTRING, fid);
        }
    }

    return (success);
}

void NextLine (FILE * fid, char *cmdstr)
{
    /*
     * Read a non-blank line into cmdstr
     */
    strcpy (cmdstr, "\0");

    while (!Readable (cmdstr))
    {
        if (fgets (cmdstr, MAXSTRING, fid) == NULL)
        {
            strcpy(cmdstr, "EOF");
            break;
        }

    }
}

int CountLine (FILE * fid, int num_arg, ...)
{
    /*
     * Count number of non-blank lines between current location to where
     * token occurs
     */

    va_list         valist;
    char            cmdstr[MAXSTRING];
    char            optstr[MAXSTRING];
    char            token[MAXSTRING];
    int             count;
    int             success = 0;
    int             i;


    /* access all the arguments assigned to valist */
    /* Initialize cmdstr */
    strcpy (cmdstr, "\0");
    count = 0;

    while (!feof (fid))
    {
        if (Readable (cmdstr))
        {
            sscanf (cmdstr, "%s", optstr);

            /* initialize valist for num number of arguments */
            va_start (valist, num_arg);
            for (i = 0; i < num_arg; i++)
            {
                strcpy (token, va_arg (valist, char *));
                if (strcasecmp (token, optstr) == 0)
                    success = 1;
            }
            /* clean memory reserved for valist */
            va_end (valist);

            if (success)
                break;
            else
                count++;
        }

        fgets (cmdstr, MAXSTRING, fid);
    }

    return (count);
}

//int CountOccurance (FILE *fid, char *token)
//{
//    /*
//     * Count number of occurance of keyword from the current line to the end
//     * of file
//     */
//
//    char            cmdstr[MAXSTRING];
//    char            optstr[MAXSTRING];
//    int             count;
//
//    /* Initialize cmdstr */
//    strcpy (cmdstr, "\0");
//    count = 0;
//
//    while (!feof (fid))
//    {
//        if (Readable (cmdstr))
//        {
//            sscanf (cmdstr, "%s", optstr);
//            if (strcasecmp (token, optstr) == 0)
//                count++;
//        }
//        
//        fgets (cmdstr, MAXSTRING, fid);
//    }
//
//    return (count);
//}

void CheckFile (FILE * fid, char *fn)
{
    if (fid == NULL)
    {
        printf ("\n ERROR: %s is in use or does not exist!\n", fn);
        PihmExit (1);
    }
    else
    {
        if (verbose_mode)
        {
            printf (" Reading %s\n", fn);
        }
    }
}

void ReadTS (char *cmdstr, int *ftime, double *data, int nvrbl)
{
    int             match;
    FILE           *stream;
    struct tm      *timeinfo;
    int             i;

    timeinfo = (struct tm *)malloc (sizeof (struct tm));

    if (1 == nvrbl)
    {
        match = sscanf (cmdstr, "%d-%d-%d %d:%d %lf",
            &timeinfo->tm_year, &timeinfo->tm_mon, &timeinfo->tm_mday,
            &timeinfo->tm_hour, &timeinfo->tm_min, &data[0]);
        timeinfo->tm_sec = 0;
        if (match != nvrbl + 5)
        {
            printf ("ERROR: Forcing format error!\n");
            PihmExit (1);
        }

        timeinfo->tm_year = timeinfo->tm_year - 1900;
        timeinfo->tm_mon = timeinfo->tm_mon - 1;
        *ftime = timegm (timeinfo);
    }
    else
    {
        stream = fmemopen (cmdstr, strlen (cmdstr), "r");
        fscanf (stream, "%d-%d-%d %d:%d",
            &timeinfo->tm_year, &timeinfo->tm_mon, &timeinfo->tm_mday,
            &timeinfo->tm_hour, &timeinfo->tm_min);
        timeinfo->tm_sec = 0;
        for (i = 0; i < nvrbl; i++)
        {
            fscanf (stream, "%lf", &data[i]);
        }
        timeinfo->tm_year = timeinfo->tm_year - 1900;
        timeinfo->tm_mon = timeinfo->tm_mon - 1;
        *ftime = timegm (timeinfo);
        fclose (stream);
    }

    free (timeinfo);
}

void ReadKeywordDouble (char *buffer, char *keyword, double *value)
{
    int             match;
    char            optstr[MAXSTRING];

    match = sscanf (buffer, "%s %lf", optstr, value);
    if (match != 2 || strcasecmp (keyword, optstr) != 0)
    {
        printf ("ERROR: Expected keyword \"%s\"!\n", keyword);
        PihmExit (1);
    }
}

void ReadKeywordInt (char *buffer, char *keyword, int *value)
{
    int             match;
    char            optstr[MAXSTRING];

    match = sscanf (buffer, "%s %d", optstr, value);
    if (match != 2 || strcasecmp (keyword, optstr) != 0)
    {
        printf ("ERROR: Expected keyword \"%s\"!\n", keyword);
        PihmExit (1);
    }
}

void ReadKeywordTime (char *buffer, char *keyword, int *value)
{
    char            optstr[MAXSTRING];
    int             match;
    struct tm      *timeinfo;

    timeinfo = (struct tm *)malloc (sizeof (struct tm));

    match = sscanf (buffer, "%s %d-%d-%d %d:%d", optstr,
        &timeinfo->tm_year, &timeinfo->tm_mon, &timeinfo->tm_mday,
        &timeinfo->tm_hour, &timeinfo->tm_min);
    timeinfo->tm_sec = 0;
    if (match != 6 || strcasecmp (keyword, optstr) != 0)
    {
        printf ("ERROR: Expected keyword \"%s\"!\n", keyword);
        PihmExit (1);
    }

    timeinfo->tm_year = timeinfo->tm_year - 1900;
    timeinfo->tm_mon = timeinfo->tm_mon - 1;
    *value = timegm (timeinfo);

    free (timeinfo);
}

void ReadKeywordStr (char *buffer, char *keyword, char *value)
{
    int             match;
    char            optstr[MAXSTRING];

    match = sscanf (buffer, "%s %[^\n]", optstr, value);
    if (match != 2 || strcasecmp (keyword, optstr) != 0)
    {
        printf ("ERROR: Expected keyword \"%s\"!\n", keyword);
        PihmExit (1);
    }
}
