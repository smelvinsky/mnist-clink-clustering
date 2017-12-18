/**
 * Created by smelvinsky on 04.12.17.
 *
 * Not really time efficient, but not memory consuming.
 * Created to port to ZedBoard Zynq-7000 using Vivado HLS
 *
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <getopt.h>
#include "mnist/mnist.h"
#include "cluster.h"

#define RUNTIME_DEBUG
#define PROGRESS_DEBUG

void print_usage()
{
    std::cout << "Usage: ./mnist-clink-clustering -t <testing_set_length> -e <ending_number_of_clusters>" << std::endl;
}

int main(int argc, char **argv)
{
    /* program argument handling */
    int trainingObjectsToLoad = 0;
    int endingNumOfClusters = 0;
    int opt;

    if (argc != 5)
    {
        print_usage();
        exit(EXIT_FAILURE);
    }

    while ((opt = getopt(argc, argv, "t:e:")) != -1)
    {
        switch (opt)
        {
            case 't':
                trainingObjectsToLoad = atoi(optarg);
                if (trainingObjectsToLoad > MNIST_TRAINING_IMAGES_NUM)
                {
                    fprintf(stderr, "Invalid argument\n");
                }
                break;
            case 'e':
                endingNumOfClusters = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Invalid argument\n");
                print_usage();
                exit(EXIT_FAILURE);
        }
    }

    #ifdef RUNTIME_DEBUG
    /* measuring time */
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    #endif

    #ifdef PROGRESS_DEBUG
    std::cout << "Loading training data..." << std::endl;
    #endif

    /* Load Training Data */
    std::vector<MNIST_Object *> MNIST_vector;
    initVectorWithMNISTObjects(&MNIST_vector, MNIST_TRAINING_SET);

    /* At first, every digit(MNIST Object) is a separate cluster */
    /* clusters vector stores all currently existing clusters    */
    std::vector<Cluster> clusters;

    for (int i = 0; i < trainingObjectsToLoad; i++)
    {
        Cluster *cluster = new Cluster();
        cluster->pushObject(MNIST_vector[i]);
        clusters.push_back(*cluster);

        #ifdef PROGRESS_DEBUG
        std::cout << "Loading MNIST digits: " << i + 1 << " of " << trainingObjectsToLoad << "\r" << std::flush;
        #endif
    }

    #ifdef PROGRESS_DEBUG
    std::cout << "Clustering training set..." << std::endl;
    #endif

    /* Clustering part (Using Complete Linkage Clustering Algorithm) */
    while (clusters.size() > endingNumOfClusters)
    {
        DistanceMatrixDataStruct distanceMatrixDataStruct = ClusterFunctions::choseClustersToMerge(clusters);

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
                clusters[i] = ClusterFunctions::mergeClusters(&clusters_tmp[distanceMatrixDataStruct.cluster_x],
                                                              &clusters_tmp[distanceMatrixDataStruct.cluster_y]);
            }
        }
    } /* End of clustering part */

    #ifdef PROGRESS_DEBUG
    std::cout << "Calculating dominant digits (MNIST objects) of final clusters..." << std::endl;
    #endif

    /* Calculate and print dominant digit of clusters and digit rate */
    for (int i = 0; i < clusters.size(); i++)
    {
        clusters[i].setLabel(ClusterFunctions::getDominantLabel(&clusters[i]));

        #ifdef PROGRESS_DEBUG
        std::cout << "Dominant digit of cluster no. " << i << " is " << (int) clusters[i].getLabel();
        std::cout << "\t(" << 0 << " - " << std::setprecision(3) << ClusterFunctions::getLabelPercentageFromCluster(0, &clusters[i]) * 100 << "%  \t";
        std::cout << 1 << " - " << ClusterFunctions::getLabelPercentageFromCluster(1, &clusters[i]) * 100 << "%  \t";
        std::cout << 2 << " - " << ClusterFunctions::getLabelPercentageFromCluster(2, &clusters[i]) * 100 << "%  \t";
        std::cout << 3 << " - " << ClusterFunctions::getLabelPercentageFromCluster(3, &clusters[i]) * 100 << "%  \t";
        std::cout << 4 << " - " << ClusterFunctions::getLabelPercentageFromCluster(4, &clusters[i]) * 100 << "%  \t";
        std::cout << 5 << " - " << ClusterFunctions::getLabelPercentageFromCluster(5, &clusters[i]) * 100 << "%  \t";
        std::cout << 6 << " - " << ClusterFunctions::getLabelPercentageFromCluster(6, &clusters[i]) * 100 << "%  \t";
        std::cout << 7 << " - " << ClusterFunctions::getLabelPercentageFromCluster(7, &clusters[i]) * 100 << "%  \t";
        std::cout << 8 << " - " << ClusterFunctions::getLabelPercentageFromCluster(8, &clusters[i]) * 100 << "%  \t";
        std::cout << 9 << " - " << ClusterFunctions::getLabelPercentageFromCluster(9, &clusters[i]) * 100 << "%)\n";
        #endif
    }

    #ifdef PROGRESS_DEBUG
    std::cout << "Loading testing data..." << std::endl;
    #endif

    /* Load testing data */
    std::vector<MNIST_Object *> MNIST_testingVector;
    initVectorWithMNISTObjects(&MNIST_testingVector, MNIST_TESTING_SET);

    /* Calculate distances to currently existing clusters and verify the correctness */
    Cluster *matchingCluster;
    int correctPredictions = 0;

    for (int i = 0; i < MNIST_testingVector.size(); i++)
    {
        matchingCluster = ClusterFunctions::findMatchingCluster(MNIST_testingVector[i], clusters);

        #ifdef PROGRESS_DEBUG
        std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
                  << "Image: " << std::endl;
        MNIST_testingVector[i]->display();
        std::cout << "Actual value: " << (int) MNIST_testingVector[i]->getMNIST_Label() << "\tPredicted: " << (int) matchingCluster->getLabel()
                  << "\nVerifying... \n" << "Part " << i + 1 << " of " << MNIST_testingVector.size() << std::endl;
        #endif
        if (MNIST_testingVector[i]->getMNIST_Label() == matchingCluster->getLabel())
        {
            correctPredictions++;
        }
    }

    /* Summarize correctness */
    #ifdef PROGRESS_DEBUG
    std::cout << "Correctness: " << 100.0 * (float) correctPredictions / (float) MNIST_testingVector.size() << "%.\n"
              << "(Based on " << trainingObjectsToLoad << " training and " << MNIST_testingVector.size() << " testing samples)." << std::endl;
    #endif

    #ifdef RUNTIME_DEBUG
    std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();

    int hours = (int) std::chrono::duration_cast<std::chrono::hours>(end - begin).count();
    int minutes = (int) std::chrono::duration_cast<std::chrono::minutes>(end - begin).count() - (hours * 60);
    int seconds = (int) std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() - (minutes * 60);

    std::cout << "Runtime: " << hours << " hours " << minutes << " minutes " << seconds << " seconds." << std::endl;
    #endif

    return 0;
}
