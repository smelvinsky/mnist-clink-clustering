/**
 * Created by smelvinsky on 04.12.17.
 */

#include <iostream>
#include "mnist.h"

MNIST_Utils::MNIST_Image MNIST_Object::getMNIST_Image()
{
    return this->MNIST_Img;
}

MNIST_Utils::MNIST_Label MNIST_Object::getMNIST_Label()
{
    return this->MNIST_Lbl;
}

void MNIST_Object::setMNIST_Image(MNIST_Utils::MNIST_Image img)
{
    this->MNIST_Img = img;
}

void MNIST_Object::setMNIST_Label(MNIST_Utils::MNIST_Label lbl)
{
    this->MNIST_Lbl = lbl;
}

void MNIST_Object::display()
{
    MNIST_Utils::displayImage(&(this->MNIST_Img), 6,6);
}

void initVectorWithMNISTObjects(std::vector<MNIST_Object *> *MNIST_ObjectVector, int MNIST_set)
{
    // open MNIST files
    FILE *imageFile, *labelFile;
    if (MNIST_set == MNIST_TRAINING_SET)
    {
        imageFile = MNIST_Utils::openMNISTImageFile((char *) MNIST_TRAINING_SET_IMAGE_FILE_NAME);
        labelFile = MNIST_Utils::openMNISTLabelFile((char *) MNIST_TRAINING_SET_LABEL_FILE_NAME);

        for (int i = 0; i < MNIST_TRAINING_IMAGES_NUM; i++)
        {
            MNIST_Object *MNIST_Obj = new MNIST_Object;

            // Reading next image and corresponding label
            MNIST_Utils::MNIST_Image img = MNIST_Utils::getNextImage(imageFile);
            MNIST_Utils::MNIST_Label lbl = MNIST_Utils::getNextLabel(labelFile);

            MNIST_Obj->setMNIST_Image(img);
            MNIST_Obj->setMNIST_Label(lbl);

            MNIST_ObjectVector->push_back(MNIST_Obj);
        }
    }
    else if (MNIST_set == MNIST_TESTING_SET)
    {
        imageFile = MNIST_Utils::openMNISTImageFile((char *) MNIST_TESTING_SET_IMAGE_FILE_NAME);
        labelFile = MNIST_Utils::openMNISTLabelFile((char *) MNIST_TESTING_SET_LABEL_FILE_NAME);

        for (int i = 0; i < MNIST_TESTING_IMAGES_NUM; i++)
        {
            MNIST_Object *MNIST_Obj = new MNIST_Object;

            // Reading next image and corresponding label
            MNIST_Utils::MNIST_Image img = MNIST_Utils::getNextImage(imageFile);
            MNIST_Utils::MNIST_Label lbl = MNIST_Utils::getNextLabel(labelFile);

            MNIST_Obj->setMNIST_Image(img);
            MNIST_Obj->setMNIST_Label(lbl);

            MNIST_ObjectVector->push_back(MNIST_Obj);
        }
    }
    else
    {
        std::cout << "Wrong MNIST set number (0 - Training set, 1 - Testing set)" << std::endl;
    }
}