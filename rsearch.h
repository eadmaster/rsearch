/*----------------------------------------------------------------------------*/
/*--  rsearch.h                                                             --*/
/*--  Relative Search header                                                --*/
/*--  Copyright (C) 2013-2014 CUE                                           --*/
/*--                                                                        --*/
/*--  This program is free software: you can redistribute it and/or modify  --*/
/*--  it under the terms of the GNU General Public License as published by  --*/
/*--  the Free Software Foundation, either version 3 of the License, or     --*/
/*--  (at your option) any later version.                                   --*/
/*--                                                                        --*/
/*--  This program is distributed in the hope that it will be useful,       --*/
/*--  but WITHOUT ANY WARRANTY; without even the implied warranty of        --*/
/*--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          --*/
/*--  GNU General Public License for more details.                          --*/
/*--                                                                        --*/
/*--  You should have received a copy of the GNU General Public License     --*/
/*--  along with this program. If not, see <http://www.gnu.org/licenses/>.  --*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
#ifndef _RSEARCH_H_ // _RSEARCH_H_
#define _RSEARCH_H_

/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

/*----------------------------------------------------------------------------*/
#define WILDCARD '?'

#define VERSION "1.4"

/*----------------------------------------------------------------------------*/
void  Title(void);
void  Usage(void);

int   FileSize(char *filename);
void *FileLoad(char *filename);
void *Allocate(int length, int size);
void *ReAllocate(void *buffer, int length, int size);
void  Free(void *buffer);
void  Exit(const char *format, ...);

void  RSearch(char *filename, char *str, int min, int max, int inc);

/*----------------------------------------------------------------------------*/
void Title(void) {
  /*printf(
    "\n"
    "RSEARCH %s - Relative Search\n"
    "Copyright (C) 2013-2014 CUE\n"
    "\n",
    VERSION
  );*/
}

/*----------------------------------------------------------------------------*/
void Usage(void) {
  Exit(
    "Usage: RSEARCH filename text [min [max [inc]]]\n"
    "\n"
    "- filename ... file to look\n"
    "- text ....... text to search or hexadecimal codes\n"
    "- min ........ min separation between characters (1 by default)\n"
    "- max ........ max separation between characters (1 by default)\n"
    "- inc ........ optional code increment (1 by default)\n"
    "\n"
    "* 'text' can include the wilcard character '?'\n"
    "* 'text' must be at least two real characters\n"
    "* 'text' can be a hexadecimal string starting with '$'\n"
    "* hexadecimal string do not support wilcards\n"
  );
}

// https://stackoverflow.com/questions/50119172/how-to-get-the-file-length-in-c-on-linux
size_t filelength(int filedes)
{
    off_t pos = lseek(filedes, 0, SEEK_CUR);
    if (pos != (off_t)-1)
    {
        off_t size = lseek(filedes, 0, SEEK_END);
        lseek(filedes, pos, SEEK_SET);
        return (size_t)size;
    }
    return (off_t)-1;
}

/*----------------------------------------------------------------------------*/
int FileSize(char *filename) {
  FILE *fp;
  int   fs;

  if ((fp = fopen(filename, "rb")) == NULL) Exit("File open error\n");
  fs = filelength(fileno(fp));
  if (fclose(fp) == EOF) Exit("File close error\n");

  return(fs);
}

/*----------------------------------------------------------------------------*/
void *FileLoad(char *filename) {
  FILE *fp;
  char *fb;
  int   fs;

  if ((fp = fopen(filename, "rb")) == NULL) Exit("File open error\n");
  fs = filelength(fileno(fp));
  fb = Allocate(fs, sizeof(char));
  if (fread(fb, 1, fs, fp) != fs) Exit("File read error\n");
  if (fclose(fp) == EOF) Exit("File close error\n");

  return(fb);
}

/*----------------------------------------------------------------------------*/
void *Allocate(int length, int size) {
  char *fb;

  fb = (char *)malloc(length * size);
  if (fb == NULL) Exit("Allocate memory error\n");

  return(fb);
}

/*----------------------------------------------------------------------------*/
void Free(void *buffer) {
  free(buffer);
}

/*----------------------------------------------------------------------------*/
void Exit(const char *format, ...) {
  va_list args;
  
  va_start(args, format);
  vfprintf(stdout, format, args);
  va_end(args);
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
#endif // _RSEARCH_H_

/*----------------------------------------------------------------------------*/
/*--  EOF                                      Copyright (C) 2013-2014 CUE  --*/
/*----------------------------------------------------------------------------*/
