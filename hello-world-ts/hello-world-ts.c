#include <stdio.h>
#include <fcntl.h>
#include <poll.h>

#include "ts.h"

int main (int argc, char * argv[])
{
    struct pollfd ts[1];
    pixel_node_t pixel;
    TOUCH_TYPE tt = eHANWANG_6;
    int restant = 1000;

    ts[0].fd = open("/dev/input/s5p_tsp", O_RDWR|O_NOCTTY|O_NONBLOCK);
    if (!ts[0].fd)
    {
        printf ("Error: open file\n");
        return 1;
    }
    printf ("file open ok\n");

    ts[0].events = POLLIN;
    ts[0].revents = 0;


    /* Get information */
    if (ioctl (ts[0].fd, TSP_SET_TOUCHSCREEN, &tt))
    {
        printf ("Error setting touchscreen.\n");
        return 2;
    }

    /* Start parsing */
    if (ioctl (ts[0].fd, TSP_BEGIN_PARSER, 0))
    {
        printf ("Error start parsing.\n");
        return 3;
    }

    while (restant > 0)
    {
        if (poll(ts, 1, 10*1000))
        {
            if(ts[0].revents & POLLIN)
            {
                ssize_t readed = read(ts[0].fd, &pixel, 16);
		printf("x:\t%i\ty:\t%i\tpression:\t%i\tereaser(wtf):\t%i\n", pixel.x, pixel.y, pixel.pr, pixel.eraser);
            } else {
                printf ("poll error.\n");
            }
        } else {
            printf ("timeout error.\n");
        }
        restant--;
    }

    /* Stop parsing */
    /* if (ioctl (ts[0].fd, TSP_END_PARSER, 0))
    {
        printf ("Error stop parsing.\n");
        return 4;
    }*/
    /* le stop semble buggé ou alors c'etait pas a retester */


    close (ts[0].fd);

    return 0;
}
