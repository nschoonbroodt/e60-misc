// mainly from http://elinux.org/ZipIt_Framebuffer_Example
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>

int main (int argc, char * argv[])
{
    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize = 0;
    char *fbp = 0;
    int x = 0, y = 0;
    long int location = 0;

    /* Open the file for reading and writing */
    fbfd = open ("/dev/fb0", O_RDWR);
    if (!fbfd)
    {
        printf ("Error: cannot open framebuffer device.\n");
        return 1;
    }
    printf ("The framebuffer device was opened successfully.\n");

    /* Get fixed screen information */
    if (ioctl (fbfd, FBIOGET_FSCREENINFO, &finfo))
    {
        printf ("Error reading fixed information.\n");
        return 2;
    }

    /* Get variable screen information */
    if (ioctl (fbfd, FBIOGET_VSCREENINFO, &vinfo))
    {
        printf ("Error reading variable information.\n");
        return 3;
    }

    /* Figure out the size of the screen in bytes */
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    /* Map the device to memory */
    fbp = (char *) mmap (0, screensize, PROT_READ | PROT_WRITE,
		       MAP_SHARED, fbfd, 0);
    if ((int) fbp == -1)
    {
        printf ("Error: failed to map framebuffer device to memory.\n");
        return 4;
    }
    printf ("The framebuffer device was mapped to memory successfully.\n");

    for (y = 0; y < 240; y++)
    {
        for (x = 0; x < 320; x++)
	{
            //x = 100; y = 100; /* Where we are going to put the pixel */

            /* Figure out where in memory to put the pixel */
	    location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 4) +
                       (y + vinfo.yoffset) * finfo.line_length;

	    *(fbp + location) = 255;	//Paint a black pixel.
	}
    }
    munmap (fbp, screensize);
    close (fbfd);
    return 0;
}
