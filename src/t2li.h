/* ----------------------------------------------------------------------

   Name:             t2li.h
   Author:           Corbin Covault
   Project:          Auger@TA
   Purpose:

       This header file contains the structure definitions for the
       T2LI bank. This is the structure which the fill routines expect
       the data in and which the bank will be unpacked. 

   Revisions:

      CEC 11/01/2021    Starting on details

   ---------------------------------------------------------------------- */

/* Define the DIG0 bank structure */


#ifndef MAX_T2_TIMESTAMPS_PER_SECOND    
#define MAX_T2_TIMESTAMPS_PER_SECOND    (128)
#endif

struct  T2LI{  
long      gps_second;
int       flag;        /* This is a status flag for use with comms handshaking */ 
int       nt2;
int       scaler;
int       num[MAX_T2_TIMESTAMPS_PER_SECOND]; 
int       tag[MAX_T2_TIMESTAMPS_PER_SECOND]; 
int       musec[MAX_T2_TIMESTAMPS_PER_SECOND];   /* Microsecconds */

};



/*   This is the packed data record presented as an array of unsigned bytes */

/*  gps_second since 0H UTC Jan 1 2021  = 1609459200  
    We get 31,536,000 seconds 
    2^28 equals 268,435,456 seoonds  = 8.5 years   */

    struct  T2LI_BANK{

        /*  First 16 bytes are a header */
        unsigned char syncword[4];          /* Start every record with this = 0xacdf  */
        unsigned char bankname[4];          /* Bank name = TILI  4 characters  */
        unsigned char bank_size[4];       /* In bytes.  Actual size to be written */
        unsigned char bank_version[4];       /* Extra space here for other things later  like CRC */


        /* Remaining bytes are the T2 data */
        unsigned char gps_seconds[4];          /* First 4 bytes */
        unsigned char nt2[2];              /* 5th and 6th bytes */
        unsigned char scaler[2];           /* 7th and 8th bytes */
        unsigned char  trifold_t2_data[3*MAX_T2_TIMESTAMPS_PER_SECOND];  /* 3 bytes per T2 timestamp */
    };