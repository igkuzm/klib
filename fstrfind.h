/**
 * File              : fstrfind.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 21.05.2024
 * Last Modified Date: 21.05.2024
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/**
 * fstrrep.h
 * Copyright (c) 2024 Igor V. Sementsov <ig.kuzm@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FSTRFIND_H
#define FSTRFIND_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void fstrfind(FILE *file, const char *needle[], 
		void *userdata,
    int (*callback)(
			void *userdata, off_t *pos, bool found, int len, char *buf)) 
{
  char **np = (char **)needle; // pointer to needle array

  int minlen = BUFSIZ;
  int maxlen = 0;
  int size = 0;

  // count len
  while (*np) {
    int len = strlen(*np++);
    if (len > maxlen)
      maxlen = len;
    if (len < minlen)
      minlen = len;
    size++;
  }

  char *buf = NULL; // main buffer
  char *pbuf = NULL; // previous buffer
  char *fbuf = NULL; // fix buffer - to find
  // words splitted by main buffers

  off_t pos  = 0; // file position
  off_t ppos = 0; // prev buffer file position
  off_t fpos = 0; // fix  buffer file position

  int buflen  = 0; // buf len
  int fbuflen = 0; // fix buf len
  int pbuflen = 0; // pbuf len
  int i = 0; // iterator

  bool eof = false;
  while (1) {
    char ch = fgetc(file);
    if ((int)ch == EOF) {
      eof = true;
      goto makesearch;
    }

    // start new buffer
    if (buflen == 0) {
      pbuf = buf;
      buf = (char *)malloc(BUFSIZ);
      if (!buf)
        break;
      for (i = 0; i < BUFSIZ; i++)
        buf[i] = 0;
    }

    // fill main buffer
    if (buflen < BUFSIZ)
      buf[buflen++] = ch;

    if (buflen == BUFSIZ) {
      // start new buffer
      pbuflen = buflen;
      ppos = pos;
      buflen = 0;
    }

    // we need fix buffer to find matches if search string 
		// is splitted by main buffer
    // start fix buffer at the end of main and at start on 
		// next buffer
    if (buflen == BUFSIZ - maxlen) { // start fix buffer
      // clear fbuf
      if (fbuf)
        free(fbuf);
      fbuf = (char *)malloc(2 * maxlen);
      if (!fbuf)
        break;
      for (i = 0; i < 2 * maxlen; i++)
        buf[i] = 0;
      fbuflen = 0;
      fbuf[fbuflen++] = ch;
    }

    if (buflen < maxlen && buflen > BUFSIZ - maxlen) {
      if (fbuf)
        fbuf[fbuflen++] = ch;
    }

    if (buflen == maxlen) { // check if there is matches in fbuf
    makesearch:
      fpos = pos;
      if (fbuf) {
        // list of matches
        fstrfnda_list *flist = fstrfnda_list_new();
        // scan buffer
        strfnda(fbuf, needle, flist, fstrfnda_cb);

        if (flist->next) { // we have string matches
          // scan and callback prev buffer - maxlen
          if (pbuf) {
            char *ptr = pbuf;
            ptr[pbuflen - maxlen] = 0;
            fstrfnda_list *list = fstrfnda_list_new();
            strfnda(ptr, needle, list, fstrfnda_cb);
            if (callback)
              if (callback(userdata, ppos - maxlen, list, ptr))
                break;
            free(pbuf);
          }

          // callback fixbuffer
          if (callback)
            if (callback(userdata, fpos, flist, fbuf))
              break;

          // free fixbuffer
          free(fbuf);
          fbuflen = 0;

          // free main buffer
          if (!eof) {
            if (buf)
              free(buf);
            buflen = 0;
          }
        } else {
          // scan and callback prev buffer
          if (pbuf) {
            fstrfnda_list *list = fstrfnda_list_new();
            strfnda(pbuf, needle, list, fstrfnda_cb);
            if (callback)
              if (callback(userdata, ppos, list, pbuf))
                break;
            free(pbuf);
            pbuf = NULL;
          }
          // free fixbuffer
          free(fbuf);
          fbuflen = 0;
        }
      }
      if (eof) {
        // scan and callback main buffer
        fstrfnda_list *list = fstrfnda_list_new();
        strfnda(buf, needle, list, fstrfnda_cb);
        if (callback)
          callback(userdata, ppos - maxlen, list, buf);

        free(buf);
        break;
      }
    }

    pos++;
  }
}

#ifdef __cplusplus
}
#endif
#endif /* ifndef FSTRFIND_H */
