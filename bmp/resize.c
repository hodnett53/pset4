/**
 * resize.c
 *
 * Luke Hodnett
 * CS50x
 * Problem Set 4
 *
 * Resizes a bmp image to the size given in the CLA
 */

#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    // store CLA's
    int n = atoi(argv[1]);
    char* infile = argv[2];
    char* outfile = argv[3];

    // n error checking
    if (n < 1 || n > 100)
    {
        printf("Usage: n = [1, 100]\n");
        return 2;
    }

    // open input file
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 3;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 5;
    }

    // determine padding of infile
    int oldPadding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // store the length of the line
    int oldWidth = bi.biWidth;
    int offset = bi.biWidth * sizeof(RGBTRIPLE) + oldPadding;
    
    // create new header files
    BITMAPFILEHEADER bfNew = bf;
    BITMAPINFOHEADER biNew = bi;

    // store new width and height after the resize
    biNew.biWidth = bi.biWidth * n;
    biNew.biHeight = bi.biHeight * n;

    // determine padding for outfile
    int padding = (4 - (biNew.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // redefine relevant headers
    biNew.biSizeImage = (biNew.biWidth * sizeof(RGBTRIPLE) + padding) * abs(biNew.biHeight);
    bfNew.bfSize = biNew.biSizeImage + sizeof(biNew) + sizeof(bfNew);

    // write outfile's BITMAPFILEHEADER and BITMAP INFOHEADER
    fwrite(&bfNew, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&biNew, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        for(int j = 0; j < n; j++)
        {
            // iterate over pixels in scanline
            for (int k = 0; k < oldWidth; k++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // loop to resize the image horizontally
                for (int l = 0; l < n; l++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // skip over old padding, if any
            fseek(inptr, oldPadding, SEEK_CUR);

            // then add new padding
            for (int m = 0; m < padding; m++)
            {
                fputc(0x00, outptr);
            }

            // mover cursor back to beginning of line if repeating vertically
            if (j < n - 1)
            {
                fseek(inptr, -offset, SEEK_CUR);
            }
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
