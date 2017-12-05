//
// Created by smelvinsky on 04.12.17.
//

#include <cstdlib>
#include <cstring>
#include "mnist_file_utils.h"

namespace MNIST_Utils
{
    /* All the integers in the files are stored in the MSB first (high endian) format used by most non-Intel processors. */
    /* Users of Intel processors and other low-endian machines must flip the bytes of the header.                        */
    uint32_t swapBytes(uint32_t highEndianNumber)
    {
        uint32_t b0,b1,b2,b3;

        b0 = (highEndianNumber & 0x000000ff) <<  24u;
        b1 = (highEndianNumber & 0x0000ff00) <<   8u;
        b2 = (highEndianNumber & 0x00ff0000) >>   8u;
        b3 = (highEndianNumber & 0xff000000) >>  24u;

        return (b0 | b1 | b2 | b3);
    }

    FILE *openMNISTImageFile(char *filename)
    {
        FILE *imageFile;
        imageFile = fopen (filename, "rb");
        if (imageFile == nullptr)
        {
            fprintf(stderr, "Couldn't open MNIST image file: \"%s\"\n", filename);
            exit(EXIT_FAILURE);
        }

        MNISTImageFileHeader imgFileHead{};
        readImageFileHeader(imageFile, &imgFileHead);

        return imageFile;
    }

    FILE *openMNISTLabelFile(char *filename)
    {
        FILE *labelFile;
        labelFile = fopen (filename, "rb");
        if (labelFile == nullptr)
        {
            fprintf(stderr, "Couldn't open MNIST label file: \"%s\"\n", filename);
            exit(EXIT_FAILURE);
        }

        MNISTLabelFileHeader lblFileHead{};
        readLabelFileHeader(labelFile, &lblFileHead);

        return labelFile;
    }

    void readImageFileHeader(FILE *imageFile, MNISTImageFileHeader *imgFileHead)
    {
        imgFileHead->magicNumber =0;
        imgFileHead->maxImages   =0;
        imgFileHead->imgWidth    =0;
        imgFileHead->imgHeight   =0;

        fread(&imgFileHead->magicNumber, 4, 1, imageFile);
        imgFileHead->magicNumber = swapBytes(imgFileHead->magicNumber);

        fread(&imgFileHead->maxImages, 4, 1, imageFile);
        imgFileHead->maxImages = swapBytes(imgFileHead->maxImages);

        fread(&imgFileHead->imgWidth, 4, 1, imageFile);
        imgFileHead->imgWidth = swapBytes(imgFileHead->imgWidth);

        fread(&imgFileHead->imgHeight, 4, 1, imageFile);
        imgFileHead->imgHeight = swapBytes(imgFileHead->imgHeight);
    }

    void readLabelFileHeader(FILE *imageFile, MNISTLabelFileHeader *lblFileHead)
    {
        lblFileHead->magicNumber =0;
        lblFileHead->maxImages   =0;

        fread(&lblFileHead->magicNumber, 4, 1, imageFile);
        lblFileHead->magicNumber = swapBytes(lblFileHead->magicNumber);

        fread(&lblFileHead->maxImages, 4, 1, imageFile);
        lblFileHead->maxImages = swapBytes(lblFileHead->maxImages);
    }

    MNIST_Image getNextImage(FILE *imageFile)
    {
        MNIST_Image img{};
        size_t bytes_read;
        bytes_read = fread(&img, sizeof(img), 1, imageFile);
        if (bytes_read != 1)
        {
            fprintf(stderr, "Error while reading MNIST image file (Couldn't get the next image)\n");
            exit(EXIT_FAILURE);
        }

        return img;
    }

    MNIST_Label getNextLabel(FILE *labelFile)
    {
        MNIST_Label lbl{};
        size_t bytes_read;
        bytes_read = fread(&lbl, sizeof(lbl), 1, labelFile);
        if (bytes_read != 1)
        {
            fprintf(stderr, "Error while reading MNIST label file (Couldn't get the next label)\n");
            exit(EXIT_FAILURE);
        }

        return lbl;
    }

    static void locateCursor(const int row, const int col)
    {
        printf("%c[%d;%dH",27,row,col);
    }

    void displayImage(MNIST_Image *img, int row, int col)
    {

        char imgStr[(MNIST_IMG_HEIGHT * MNIST_IMG_WIDTH) + ((col + 1) * MNIST_IMG_HEIGHT) + 1];
        strcpy(imgStr, "");

        for (int y = 0; y < MNIST_IMG_HEIGHT; y++)
        {

            for (int o = 0; o < col - 2; o++) strcat(imgStr," ");
            strcat(imgStr,"|");

            for (int x = 0; x < MNIST_IMG_WIDTH; x++)
            {
                strcat(imgStr, img->pixel[y * MNIST_IMG_HEIGHT + x] ? "X" : "." );
            }
            strcat(imgStr,"\n");
        }

        if (col != 0 && row != 0) locateCursor(row, 0);
        printf("%s",imgStr);
    }
}
