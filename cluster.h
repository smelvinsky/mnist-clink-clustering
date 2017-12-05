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
    public:
        std::vector<MNIST_Object *> getObjects();
        void pushObject(MNIST_Object *object);
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
}

#endif //CLUSTER_H
