#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include "mnist/mnist.h"
#include "cluster.h"

#define ENDING_NUM_OF_CLUSTERS 9

int main()
{
    /* measuring time */
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    /* Loading Training Data */
    std::vector<MNIST_Object *> MNIST_vector;
    initVectorWithMNISTObjects(&MNIST_vector);

    /* At first every digit(MNIST Object) is a separate cluster */
    /* clusters vector stores all currently existing clusters   */
    std::vector<Cluster> clusters;

    for (int i = 0; i < MNIST_vector.size() / 300; i++)
    {
        Cluster *cluster = new Cluster{};
        cluster->pushObject(MNIST_vector[i]);
        clusters.push_back(*cluster);
    }

    /* Clustering part (Using Complete Linkage Clustering Algorithm) */
    while (clusters.size() > ENDING_NUM_OF_CLUSTERS)
    {
        /* choose two clusters to merge by calculating max Euclidean distance between clusters */
        DistanceMatrixDataStruct distanceMatrixDataStruct{ .cluster_x = 0,
                                                           .cluster_y = 1,
                                                           .distance = ClusterFunctions::calculateDistanceBetweenObjects(clusters[0].getObjects()[0], clusters[1].getObjects()[0])};


        int y_tmp = 2;  //this variable is used to skip redundant searching (only half of the matrix is used)
        double dist_tmp;

        for (register int i = 1; i < clusters.size(); i++)
        {
            for (register int j = y_tmp; j < clusters.size(); j++)
            {
                dist_tmp = ClusterFunctions::calculateMaxDistanceBetweenClusters(&clusters[i], &clusters[j]);
                std::cout << "Distance between " << i << " and " << j << " is: " << dist_tmp << std::endl;

                if (dist_tmp < distanceMatrixDataStruct.distance)
                {
                    distanceMatrixDataStruct.distance = dist_tmp;
                    distanceMatrixDataStruct.cluster_x = (uint32_t) i;
                    distanceMatrixDataStruct.cluster_y = (uint32_t) j;
                }
            }
            y_tmp++;
        }

        std::cout << "Clusters chosen to merge: " << distanceMatrixDataStruct.cluster_x << " and " << distanceMatrixDataStruct.cluster_y << std::endl;
        clusters[distanceMatrixDataStruct.cluster_x].getObjects()[0]->display();
        clusters[distanceMatrixDataStruct.cluster_y].getObjects()[0]->display();

        /* Merge chosen clusters */
        std::vector<Cluster> clusters_tmp = clusters;
        clusters.clear();
        for (int i = 0; i < clusters_tmp.size(); i++)
        {
            if (i == distanceMatrixDataStruct.cluster_y)
            {
                continue;
            }

            clusters.push_back(clusters_tmp[i]);

            if (i == distanceMatrixDataStruct.cluster_x)
            {
                clusters[i] = ClusterFunctions::mergeClusters(&clusters_tmp[distanceMatrixDataStruct.cluster_x], &clusters_tmp[distanceMatrixDataStruct.cluster_y]);
            }
        }

        std::cout << "Current number of clusters: " << clusters.size() << std::endl;

    } /* Clustering part */

    /* Verify the clustering part */
    for (int i = 0; i < clusters.size(); i++)
    {
        for (int j = 0; j < clusters[i].getObjects().size(); j++)
        {
            std::cout << i  <<" cluster ";
            printf("%d", clusters[i].getObjects()[j]->getMNIST_Label());
            std::cout << std::endl;
        }
    }

    std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
    std::cout << "Time difference in minutes = " << std::chrono::duration_cast<std::chrono::minutes>(end - begin).count() <<std::endl;
    std::cout << "Time difference in seconds = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() <<std::endl;
    std::cout << "Time difference in milliseconds = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() <<std::endl;
    return 0;
}