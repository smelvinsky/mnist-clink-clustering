//
// Created by smelvinsky on 04.12.17.
//

#include <cmath>
#include "cluster.h"

std::vector<MNIST_Object *> Cluster::getObjects()
{
    return this->objects;
}

void Cluster::pushObject(MNIST_Object *object)
{
    this->objects.push_back(object);
}

namespace ClusterFunctions
{
    double calculateDistanceBetweenObjects(MNIST_Object *object1, MNIST_Object *object2)
    {
        double tmp = 0;

        for (int i = 0; i < (28 * 28); i++)
        {
            tmp = tmp + pow(object1->getMNIST_Image().pixel[i] - object2->getMNIST_Image().pixel[i], 2.0);
        }

        return sqrt(tmp);
    }

    double calculateMaxDistanceBetweenClusters(Cluster *cluster1, Cluster *cluster2)
    {
        double maxDistance = 0;

        for (int i = 0; i < cluster1->getObjects().size(); i++)
        {
            for (int j = 0; j < cluster2->getObjects().size(); j++)
            {
                if (calculateDistanceBetweenObjects(cluster1->getObjects()[i], cluster2->getObjects()[j]) > maxDistance)
                {
                    maxDistance = calculateDistanceBetweenObjects(cluster1->getObjects()[i], cluster2->getObjects()[j]);
                }
            }
        }

        return maxDistance;
    }

    Cluster mergeClusters(Cluster *cluster1, Cluster *cluster2)
    {
        Cluster *cluster = new Cluster{};
        *cluster = *cluster1;

        for (int i = 0; i < cluster2->getObjects().size(); i++)
        {
            cluster->pushObject(cluster2->getObjects()[i]);
        }

        return *cluster;
    }
}