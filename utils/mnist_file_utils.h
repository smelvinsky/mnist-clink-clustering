//
// Created by smelvinsky on 04.12.17.
//

#ifndef MNIST_UTILS_H
#define MNIST_UTILS_H

/* image width in pixels */
#define MNIST_IMG_WIDTH 28
/* image height in pixels */
#define MNIST_IMG_HEIGHT 28

#include <cstdint>
#include <cstdio>

namespace MNIST_Utils
{
    typedef struct MNIST_Image MNIST_Image;
    typedef uint8_t MNIST_Label;

    /* Image structure containing 0-255 values of every pixel (flattened) */
    struct MNIST_Image{
        uint8_t pixel[28*28];
    };

    /* This structure is used for the header size reference only        */
    /* Fields are not used/declared                                     */
    /* See http://yann.lecun.com/exdb/mnist/ for details for more info  */
    struct MNISTImageFileHeader{
        uint32_t magicNumber;
        uint32_t maxImages;
        uint32_t imgWidth;
        uint32_t imgHeight;
    };

    /* This structure is used for the header size reference only        */
    /* Fields are not used/declared                                     */
    /* See http://yann.lecun.com/exdb/mnist/ for details for more info  */
    struct MNISTLabelFileHeader{
        uint32_t magicNumber;
        uint32_t maxImages;
    };

    uint32_t swapBytes(uint32_t highEndianNumber);
    FILE *openMNISTImageFile(char *filename);
    FILE *openMNISTLabelFile(char *filename);
    void readImageFileHeader(FILE *imageFile, MNISTImageFileHeader *imgFileHead);
    void readLabelFileHeader(FILE *imageFile, MNISTLabelFileHeader *lblFileHead);
    MNIST_Image getNextImage(FILE *imageFile);
    MNIST_Label getNextLabel(FILE *labelFile);

    void displayImage(MNIST_Image *img, int row, int col);
}

#endif //MNIST_UTILS_H
