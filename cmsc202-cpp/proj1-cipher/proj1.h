/**************************************************
 CMSC 202 - Fall 2015 - Project 1 Header File
 
 THIS FILE MAY NOT BE MODIFIED

***************************************************/

#ifndef PROJ1_H
#define PROJ1_H

const int ALPHALEN   = 26; // number of valid characters (A-Z)
const int MAXMSGLEN  = 60; // maximum message length
const int NUMMSGS    = 10; // number of messages
const int NUMCRIBS   =  5; // number of cribs
const int MAXCRIBLEN =  8; // maximum crib length

/***************************************************
   cipher[][] contains the 10 intercepted Zendian
   mesages stored as C-strings.  cipher[0] is the 
   first message, cipher[1] the second, etc.
****************************************************/

const char cipher[NUMMSGS][MAXMSGLEN] = {
  "HAAHJR HA KHDU AVTVYYVD",
  "DHFGS NBKNBJ ZMC ZKK HR VDKK",
  "Q PIDM JQMJMZ NMDMZ",
  "JCTFGT DGVVGT HCUVGT UVTQPIGT",
  "LRPYE I HTWW XPPE JZF LE ESP NZXXZYD",
  "KLSQ GML LGG DSLW YGL FGLZAF AF EQ TJSAF",
  "QEBC GUR ZVPEBSVYZ ORUVAQ GUR FGNGHR",
  "GZSGZWD NX XTRJBMJWJ JQXJ FY UWJXJSY",
  "RZVOCZM AJM OJHJMMJR HJMIDIB RVMH RDOC GJR XGJPYN",
  "ROBO MYWO LKN XOGC DKVUSXQ DRSC KXN DRKD"
};

/***************************************************
   cribs[][] contains the eight "interesting" 
   message beginings stored as C-strings.  cribs[0] 
   is the first crib, cribs[1] is the second, etc.
****************************************************/

const char cribs[NUMCRIBS][MAXCRIBLEN] = {
  "ATTACK", 
  "MEET", 
  "DROP", 
  "AGENT", 
  "WEATHER"
};

/******************************************************
   Decipher() - decipher a Caesar-enciphered mesage.
   Preconditions: cip[] is char array of maximum 
     length MAXMSGLEN containing a cipher message as
     a null-terminated C-string.  key is a capital 
     letter (A-Z) and is the Caesar cipher key.
   Postconditions: cip[] is overwritten with the 
     deciphered message. 
******************************************************/

void Decipher(char cip[], char key);

/******************************************************
   SolveCipher() - decipher a Caesar-enciphered mesage
     trying all possible keys and checking if the 
     beginning of the decrypted messages matches one of 
     the cribs.  If so, return the key and return the 
     deciphered message in the dec parameter.
   Preconditions: cip is a char array of maximum
     length MAXMSGLEN which contains the cipher 
     message; dec is a char array of length MAXMSGLEN.
   Postconditions: Returns the key if the deciphered
     message begins with a crib; returns '\0' 
     otherwise.  If the key is non-zero, the 
     deciphered message is returned in dec.
******************************************************/

char SolveCipher(const char cip[], char dec[]);

#endif
