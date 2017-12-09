/**
 * Created by smelvinsky on 04.12.17.
 */

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

MNIST_Utils::MNIST_Label Cluster::getLabel()
{
    return this->label;
}

void Cluster::setLabel(MNIST_Utils::MNIST_Label label)
{
    this->label = label;
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

    MNIST_Utils::MNIST_Label getDominantLabel(Cluster *cluster)
    {
        int label_count[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        int max = 0;
        MNIST_Utils::MNIST_Label dominantLabel = 255;

        for (int i = 0; i < cluster->getObjects().size(); i++)
        {
            label_count[cluster->getObjects()[i]->getMNIST_Label()]++;
        }

        for (int i = 0; i < 9; i++)
        {
            if (label_count[i] >= max)
            {
                max = label_count[i];
                dominantLabel = (MNIST_Utils::MNIST_Label) i;
            }
        }

        return dominantLabel;
    }

    float getLabelPercentageFromCluster(MNIST_Utils::MNIST_Label label, Cluster *cluster)
    {
        int labelCount = 0;

        for (int i = 0; i < cluster->getObjects().size(); i++)
        {
            if (cluster->getObjects()[i]->getMNIST_Label() == label)
            {
                labelCount++;
            }
        }

        return (float) labelCount / (cluster->getObjects().size());
    }

    Cluster *findMatchingCluster(MNIST_Object *object, std::vector<Cluster> &clusters)
    {
        /* That's the maximum distance that can be achieved between two MNIST images                      */
        /* (situation where one image is completely black (255's only) and the second one is white (0's)) */
        double minDistance = sqrt(pow(255.0,2.0) * (28*28));
        double minDistance_tmp = minDistance;
        Cluster *matchingCluster = NULL;

        for (int i = 0; i < clusters.size(); i++)
        {
            Cluster *cluster = new Cluster();
            cluster->pushObject(object);

            minDistance_tmp = calculateMaxDistanceBetweenClusters(cluster, &clusters[i]);
            if (minDistance_tmp < minDistance)
            {
                minDistance = minDistance_tmp;
                matchingCluster = &clusters[i];
            }
            delete cluster;
        }

        return matchingCluster;
    }
}