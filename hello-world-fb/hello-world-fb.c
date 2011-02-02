// mainly from http://elinux.org/ZipIt_Framebuffer_Example
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>

#include "s3c_epd_userland.h"

int main (int argc, char * argv[])
{
    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize = 0;
    char *fbp = 0;
    int x = 0, y = 0;
    long int location = 0;

    struct epd_control_data data;

    data.x=0;
    data.y=0;
    data.data_x=0;
    data.data_y=0;
    data.dfmt=2;
    data.width=600;
    data.height=800;
    data.erase_flag=0;
    data.buf_index=0;
    data.async_mtd=0;
    data.overlap_check=0;
    data.update_only=0;
    data.type=eFULL;


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

    printf("size vert:%i\thori:%i\tbpp:%i\n", vinfo.yres, vinfo.xres, vinfo.bits_per_pixel);
    printf("offset vert:%i\thori:%i\n", vinfo.yoffset, vinfo.xoffset);

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

    // les modif que j'ai faites ne sont pas generiques
    // suppose que les offset = 0

#ifdef READ_SCREEN
    printf("recuperation du contenu de l'ecran\n");
    for (y = 0; y < vinfo.yres; y++)
    {
        printf("%03i:\t", y);
        for (x = 0; x < vinfo.xres; x=x+2)
	{
            location = x * 0.5 + y * finfo.line_length;
	    printf("%02X", fbp[location]);
	}
        printf("\n");
    }
#endif

    for (y = 0; y < vinfo.yres; y++)
    {
        for (x = 0; x < vinfo.xres; x=x+2)
	{
            location = x * 0.5 + y * finfo.line_length;
	    fbp[location] = 0xFF;
	}
    }

    ioctl(fbfd, 0x4004405A, &data);
    // la c'est blanc normalement

    int bande;
    for (bande = 0; bande < 16; bande++)
    {
        for (y = 0; y < vinfo.yres; y++)
        {
            for (x = bande*vinfo.xres/16; x < (bande+1)*vinfo.xres/16; x=x+2)
            {
                location = x * 0.5 + y * finfo.line_length;
	        fbp[location] = bande*0x11;
            }
        }
    }
    ioctl(fbfd, 0x4004405A, &data);
    // et la c'est un zebre (oui je suis nul en dessin de zebre)

#ifdef READ_SCREEN
    printf("recuperation du contenu de l'ecran\n");
    for (y = 0; y < vinfo.yres; y++)
    {
        printf("%03i:\t", y);
        for (x = 0; x < vinfo.xres; x=x+2)
	{
            location = x * 0.5 + y * finfo.line_length;
	    printf("%02X", fbp[location]);
	}
        printf("\n");
    }
#endif

    munmap (fbp, screensize);
    close (fbfd);

    //printf("%i\n", data.buf_index);

    return 0;
}
