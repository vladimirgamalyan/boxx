/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2019, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
#include "tool_setup.h"

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

#define ENABLE_CURLX_PRINTF
/* use our own printf() functions */
#include "curlx.h"

#include "tool_cfgable.h"
#include "tool_cb_prg.h"
#include "tool_util.h"
#include "tool_operate.h"

#include "memdebug.h" /* keep this as LAST include */

#ifdef HAVE_TERMIOS_H
#  include <termios.h>
#elif defined(HAVE_TERMIO_H)
#  include <termio.h>
#endif

/* 200 values generated by this perl code:

   my $pi = 3.1415;
   foreach my $i (1 .. 200) {
     printf "%d, ", sin($i/200 * 2 * $pi) * 5000 + 5000;
   }
*/
static const unsigned int sinus[] = {
  5157, 5313, 5470, 5626, 5782, 5936, 6090, 6243, 6394, 6545, 6693, 6840, 6985,
  7128, 7269, 7408, 7545, 7679, 7810, 7938, 8064, 8187, 8306, 8422, 8535, 8644,
  8750, 8852, 8950, 9045, 9135, 9221, 9303, 9381, 9454, 9524, 9588, 9648, 9704,
  9755, 9801, 9842, 9879, 9911, 9938, 9960, 9977, 9990, 9997, 9999, 9997, 9990,
  9977, 9960, 9938, 9911, 9879, 9842, 9801, 9755, 9704, 9648, 9588, 9524, 9455,
  9381, 9303, 9221, 9135, 9045, 8950, 8852, 8750, 8645, 8535, 8422, 8306, 8187,
  8064, 7939, 7810, 7679, 7545, 7409, 7270, 7129, 6986, 6841, 6694, 6545, 6395,
  6243, 6091, 5937, 5782, 5627, 5470, 5314, 5157, 5000, 4843, 4686, 4529, 4373,
  4218, 4063, 3909, 3757, 3605, 3455, 3306, 3159, 3014, 2871, 2730, 2591, 2455,
  2321, 2190, 2061, 1935, 1813, 1693, 1577, 1464, 1355, 1249, 1147, 1049, 955,
  864, 778, 696, 618, 545, 476, 411, 351, 295, 244, 198, 157, 120, 88, 61, 39,
  22, 9, 2, 0, 2, 9, 22, 39, 61, 88, 120, 156, 198, 244, 295, 350, 410, 475,
  544, 618, 695, 777, 864, 954, 1048, 1146, 1248, 1354, 1463, 1576, 1692, 1812,
  1934, 2060, 2188, 2320, 2454, 2590, 2729, 2870, 3013, 3158, 3305, 3454, 3604,
  3755, 3908, 4062, 4216, 4372, 4528, 4685, 4842, 4999
};

static void fly(struct ProgressData *bar, bool moved)
{
  char buf[256];
  int pos;
  int check = bar->width - 2;

  msnprintf(buf, sizeof(buf), "%*s\r", bar->width-1, " ");
  memcpy(&buf[bar->bar], "-=O=-", 5);

  pos = sinus[bar->tick%200] / (10000 / check);
  buf[pos] = '#';
  pos = sinus[(bar->tick + 5)%200] / (10000 / check);
  buf[pos] = '#';
  pos = sinus[(bar->tick + 10)%200] / (10000 / check);
  buf[pos] = '#';
  pos = sinus[(bar->tick + 15)%200] / (10000 / check);
  buf[pos] = '#';

  fputs(buf, bar->out);
  bar->tick += 2;
  if(bar->tick >= 200)
    bar->tick -= 200;

  bar->bar += (moved?bar->barmove:0);
  if(bar->bar >= (bar->width - 6)) {
    bar->barmove = -1;
    bar->bar = bar->width - 6;
  }
  else if(bar->bar < 0) {
    bar->barmove = 1;
    bar->bar = 0;
  }
}

/*
** callback for CURLOPT_XFERINFOFUNCTION
*/

#define MAX_BARLENGTH 256

#if (SIZEOF_CURL_OFF_T == 4)
#  define CURL_OFF_T_MAX CURL_OFF_T_C(0x7FFFFFFF)
#else
   /* assume CURL_SIZEOF_CURL_OFF_T == 8 */
#  define CURL_OFF_T_MAX CURL_OFF_T_C(0x7FFFFFFFFFFFFFFF)
#endif

int tool_progress_cb(void *clientp,
                     curl_off_t dltotal, curl_off_t dlnow,
                     curl_off_t ultotal, curl_off_t ulnow)
{
  /* The original progress-bar source code was written for curl by Lars Aas,
     and this new edition inherits some of his concepts. */

  struct timeval now = tvnow();
  struct per_transfer *per = clientp;
  struct OutStruct *outs = &per->outs;
  struct OperationConfig *config = outs->config;
  struct ProgressData *bar = &per->progressbar;
  curl_off_t total;
  curl_off_t point;

  /* Calculate expected transfer size. initial_size can be less than zero
     when indicating that we are expecting to get the filesize from the
     remote */
  if(bar->initial_size < 0 ||
     ((CURL_OFF_T_MAX - bar->initial_size) < (dltotal + ultotal)))
    total = CURL_OFF_T_MAX;
  else
    total = dltotal + ultotal + bar->initial_size;

  /* Calculate the current progress. initial_size can be less than zero when
     indicating that we are expecting to get the filesize from the remote */
  if(bar->initial_size < 0 ||
     ((CURL_OFF_T_MAX - bar->initial_size) < (dlnow + ulnow)))
    point = CURL_OFF_T_MAX;
  else
    point = dlnow + ulnow + bar->initial_size;

  if(bar->calls) {
    /* after first call... */
    if(total) {
      /* we know the total data to get... */
      if(bar->prev == point)
        /* progress didn't change since last invoke */
        return 0;
      else if((tvdiff(now, bar->prevtime) < 100L) && point < total)
        /* limit progress-bar updating to 10 Hz except when we're at 100% */
        return 0;
    }
    else {
      /* total is unknown */
      if(tvdiff(now, bar->prevtime) < 100L)
        /* limit progress-bar updating to 10 Hz */
        return 0;
      fly(bar, point != bar->prev);
    }
  }

  /* simply count invokes */
  bar->calls++;

  if((total > 0) && (point != bar->prev)) {
    char line[MAX_BARLENGTH + 1];
    char format[40];
    double frac;
    double percent;
    int barwidth;
    int num;
    if(point > total)
      /* we have got more than the expected total! */
      total = point;

    frac = (double)point / (double)total;
    percent = frac * 100.0;
    barwidth = bar->width - 7;
    num = (int) (((double)barwidth) * frac);
    if(num > MAX_BARLENGTH)
      num = MAX_BARLENGTH;
    memset(line, '#', num);
    line[num] = '\0';
    msnprintf(format, sizeof(format), "\r%%-%ds %%5.1f%%%%", barwidth);
    fprintf(bar->out, format, line, percent);
  }
  fflush(bar->out);
  bar->prev = point;
  bar->prevtime = now;

  if(config->readbusy) {
    config->readbusy = FALSE;
    curl_easy_pause(per->curl, CURLPAUSE_CONT);
  }

  return 0;
}

void progressbarinit(struct ProgressData *bar,
                     struct OperationConfig *config)
{
  char *colp;
  memset(bar, 0, sizeof(struct ProgressData));

  /* pass this through to progress function so
   * it can display progress towards total file
   * not just the part that's left. (21-may-03, dbyron) */
  if(config->use_resume)
    bar->initial_size = config->resume_from;

  colp = curlx_getenv("COLUMNS");
  if(colp) {
    char *endptr;
    long num = strtol(colp, &endptr, 10);
    if((endptr != colp) && (endptr == colp + strlen(colp)) && (num > 20) &&
       (num < 10000))
      bar->width = (int)num;
    curl_free(colp);
  }

  if(!bar->width) {
    int cols = 0;

#ifdef TIOCGSIZE
    struct ttysize ts;
    if(!ioctl(STDIN_FILENO, TIOCGSIZE, &ts))
      cols = ts.ts_cols;
#elif defined(TIOCGWINSZ)
    struct winsize ts;
    if(!ioctl(STDIN_FILENO, TIOCGWINSZ, &ts))
      cols = ts.ws_col;
#elif defined(WIN32)
    {
      HANDLE  stderr_hnd = GetStdHandle(STD_ERROR_HANDLE);
      CONSOLE_SCREEN_BUFFER_INFO console_info;

      if((stderr_hnd != INVALID_HANDLE_VALUE) &&
         GetConsoleScreenBufferInfo(stderr_hnd, &console_info)) {
        /*
         * Do not use +1 to get the true screen-width since writing a
         * character at the right edge will cause a line wrap.
         */
        cols = (int)
          (console_info.srWindow.Right - console_info.srWindow.Left);
      }
    }
#endif /* TIOCGSIZE */
    bar->width = cols;
  }

  if(!bar->width)
    bar->width = 79;
  else if(bar->width > MAX_BARLENGTH)
    bar->width = MAX_BARLENGTH;

  bar->out = config->global->errors;
  bar->tick = 150;
  bar->barmove = 1;
}
