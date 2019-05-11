#include <termios.h>

static struct termios old, news;

/* Initialize new terminal i/o settings */
void initTermios()
{
    tcgetattr(0, &old); /* grab old terminal i/o settings */
    news = old; /* make new settings same as old settings */
    news.c_lflag &= ~ICANON; /* disable buffered i/o */

    news.c_lflag &= ~ECHO; /* set no echo mode */

    tcsetattr(0, TCSANOW, &news); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios()
{
    tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch()
{
    char ch;
    initTermios();
    ch = getchar();
    resetTermios();
    return ch;
}
