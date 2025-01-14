/*----------------------------------------------------------------------------*/
/*--  rsearch.c                                                             --*/
/*--  Relative Search                                                       --*/
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
#include "rsearch.h"

/*----------------------------------------------------------------------------*/
int main(int argc, char **argv) {
  int min, max, inc;

  Title();
  if ((argc < 3) || (argc > 6)) Usage();

  min = argc < 4 ? 1 : strtol(argv[3], NULL, 0);
  if (min < 1) Exit("Bad minimum value\n");

  max = argc < 5 ? 1 : strtol(argv[4], NULL, 0);
  if (max < min) Exit("Bad maximum value\n");

  inc = argc < 6 ? 1 : strtol(argv[5], NULL, 0);
  if (!inc) Exit("Bad increment value\n");

  RSearch(argv[2], argv[1], min, max, inc);

  //printf("\nDone\n");

  exit(EXIT_SUCCESS);
}

/*----------------------------------------------------------------------------*/
void RSearch(char *filename, char *str, int min, int max, int inc) {
  unsigned char text[1024], *buffer, *diff, *src, *tgt;
  unsigned int  length, len, start;
  unsigned int  i, j, l;

  if (*str == '$') {
    j = 1;
    for (len = 0; str[j] && (len < 1024); len++) {
      l = str[j++];
      if      ((l >= '0') && (l <= '9')) l -= 48;
      else if ((l >= 'A') && (l <= 'F')) l -= 55;
      else if ((l >= 'a') && (l <= 'f')) l -= 87;
      else Exit("Bad hexadecimal value\n");
      text[len] = l << 4;
      l = str[j++];
      if      ((l >= '0') && (l <= '9')) l -= 48;
      else if ((l >= 'A') && (l <= 'F')) l -= 55;
      else if ((l >= 'a') && (l <= 'f')) l -= 87;
      else Exit("Bad hexadecimal value\n");
      text[len] |= l;
    }

    if (len < 2) Exit("Code too short\n");

    start = 0;
  } else {
    for (len = 0; str[len] && (len < 1024); len++) text[len] = str[len];

    l = 0;
    for (i = 0; i < len; i++) if (text[i] != WILDCARD) l++;
    if (l < 2) Exit("Text too short\n");

    for (start = 0; text[start] == WILDCARD; start++);
  }

  //printf("- reading file '%s'\n", filename);
  puts(filename);

  length = FileSize(filename);
  buffer = FileLoad(filename);

  //if (*str == '$') printf("- searching hexadecimal '%s'\n", str);
  //else             printf("- searching string '%s'\n", str);

  diff = Allocate(len, sizeof(char));
  for (l = 0; l < len; l++) diff[l] = 0;

  l = start;
  for (i = l++; l < len; l++) {
    if ((*str == '$') || (text[l] != WILDCARD)) {
      diff[l] = (text[l] - text[i]) * inc;
      i = l;
    }
  }

  for (i = 0; i < length; i++) {
    for (j = min; j <= max; j++) {
      if (i > length - len * j + j - 1) break;

      src = buffer + i + start * j;
      tgt = src + j;

      for (l = start + 1; l < len; l++) {
        if ((*str == '$') || (text[l] != WILDCARD)) {
          if ((unsigned char) (*src + diff[l]) != *tgt) break;
          src = tgt;
        }
        tgt += j;
      }

      if (l == len) {
        printf("%08X:%X ", i, j);
        for (l = 0; l < len; l++) {
          if ((*str == '$') || (text[l] != WILDCARD))
            printf(" %02X ", buffer[i+l*j]);
          else
            printf("(%02X)", buffer[i+l*j]);
        }
        printf("\n");
      }
    }
  }

  Free(diff);
  Free(buffer);
}

/*----------------------------------------------------------------------------*/
/*--  EOF                                      Copyright (C) 2013-2014 CUE  --*/
/*----------------------------------------------------------------------------*/
