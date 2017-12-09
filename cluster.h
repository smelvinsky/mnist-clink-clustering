/**
 * Created by smelvinsky on 04.12.17.
 */

#ifndef CLUSTER_H
#define CLUSTER_H

#include <vector>
#include "mnist/mnist.h"

class Cluster
{
    private:
        std::vector<MNIST_Object *> objects;
        MNIST_Utils::MNIST_Label label;
    public:
        std::vector<MNIST_Object *> getObjects();
        void pushObject(MNIST_Object *object);

        void setLabel(MNIST_Utils::MNIST_Label label);
        MNIST_Utils::MNIST_Label getLabel();

        Cluster()
        {
            /* default label has to be greater than 9 to avoid debugging problems */
            label = 255;
        }
};

/* structure used to store y and x indexes, and distance between them */
struct DistanceMatrixDataStruct
{
    /* x index - representing cluster in dist. matrix */
    uint32_t cluster_x;
    /* y index - representing cluster in dist. matrix */
    uint32_t cluster_y;
    /* distance between x and y */
    double distance;
};

namespace ClusterFunctions
{
    double calculateMaxDistanceBetweenClusters(Cluster *cluster1, Cluster *cluster2);
    double calculateDistanceBetweenObjects(MNIST_Object *object1, MNIST_Object *object2);
    Cluster mergeClusters(Cluster *cluster1, Cluster *cluster2);
    MNIST_Utils::MNIST_Label getDominantLabel(Cluster *cluster);
    float getLabelPercentageFromCluster(MNIST_Utils::MNIST_Label label, Cluster *cluster);
    Cluster *findMatchingCluster(MNIST_Object *object, std::vector<Cluster> &clusters);
}

#endif //CLUSTER_H
