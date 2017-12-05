/**
 * Created by smelvinsky on 04.12.17.
 */

#ifndef CLUSTERING_MNIST_H
#define CLUSTERING_MNIST_H

/* Training set */
#define MNIST_TRAINING_SET_IMAGE_FILE_NAME "../data/train-images-idx3-ubyte"
#define MNIST_TRAINING_SET_LABEL_FILE_NAME "../data/train-labels-idx1-ubyte"

/* Testing set */
#define MNIST_TESTING_SET_IMAGE_FILE_NAME "../data/t10k-images-idx3-ubyte"
#define MNIST_TESTING_SET_LABEL_FILE_NAME "../data/t10k-labels-idx1-ubyte"

/* number of images+labels in the TRAINING file */
#define MNIST_TRAINING_IMAGES_NUM 60000
/* number of images+labels in the TESTING file  */
#define MNIST_TESTING_IMAGES_NUM 10000


#include "../utils/mnist_file_utils.h"
#include <vector>

class MNIST_Object
{
    private:
        MNIST_Utils::MNIST_Image MNIST_Img;
        MNIST_Utils::MNIST_Label MNIST_Lbl;
    public:
        void setMNIST_Image(MNIST_Utils::MNIST_Image);
        void setMNIST_Label(MNIST_Utils::MNIST_Label);

        MNIST_Utils::MNIST_Image getMNIST_Image();
        MNIST_Utils::MNIST_Label getMNIST_Label();

        void display();
};

void initVectorWithMNISTObjects(std::vector<MNIST_Object *> *MNIST_ObjectVector);

#endif //CLUSTERING_MNIST_H