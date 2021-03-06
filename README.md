# MNIST + complete-link clustering

A simple **MNIST** handwritten digits recognition example using **complete-linkage clustering** algorithm.

For more info about MNIST:
http://yann.lecun.com/exdb/mnist/

<img src="https://upload.wikimedia.org/wikipedia/commons/thumb/2/27/MnistExamples.png/220px-MnistExamples.png" width="400">

## Compile and run the source code


Once you have cloned the repository to your local machine, follow the directions below:


If you haven't installed cmake yet open your ` terminal ` and type:
```
$ sudo apt-get install cmake
```

Then, `` cd `` into main directory of the project ( this one including  `` CMakeLists.txt `` file ) and run `cmake` to generate ` Makefile `:

```
$ cmake CMakeLists.txt
```
After that compile the program with:

```
$ make
```

Program can be executed by calling:


```
$ ./mnist-clink-clustering -t <testing_set_length> -e <ending_number_of_clusters>
```
where the ` <testing_set_length> ` is the number of MNIST digits used in clustering process and the ` <ending_number_of_clusters> `is the number of clusters where the custering part should end and begin to verify and test clustering correctness (using diffrent - testing set of digits).
