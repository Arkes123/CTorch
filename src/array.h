#pragma once

#include <vector>
#include <iostream>
#include <random>

using namespace std;

template <typename T>
class Array {
private:
    vector<int> shape;
    vector<T> data;
    vector<int> strides;
    int dimension;
    int size;

public:
    // Setter
    void setDim(int dim) { dimension = dim; }
    void setSize(int s) { size = s; }
    void setData(int index, T value) { data[index] = value; }

    // Constructor
    Array(const vector<int>& shape) : shape(shape) {
        setDim(shape.size());
        int size = 1;
        for (int i = 0; i < getDim(); i++) {
            size *= shape[i];
        }
        setSize(size);
        data.resize(getSize());
        for (int i = 0; i < size; i++) {
            setData(i, 0);
        }

        calcStrides(strides, shape, dimension);
    }

    // Destructor
    ~Array() {}

    // Functions
    void zeroTensor();
    void calcStrides(vector<int>& strides, vector<int> shape, int dimension);
    int flatIndex(const vector<int> indices) const;
    void randomize(float upper, float lower);
    
    // Getter
    int getDim() { return dimension; }
    int getSize() { return size; }
    vector<int> getShape() { return shape; }
    vector<int> getStrides() { return strides; }
    void print();

    T& at(const vector<int>& indices);

    //transpose functionality (inverting shape)
};




// FUNCTION IMPLEMENTATIONS HAVE TO STAY HERE BECAUSE OF TEMPLATE 
template <typename T>
void Array<T>::calcStrides(vector<int>& strides, vector<int> shape, int dimension) {
    strides.resize(dimension);
    strides[dimension - 1] = 1;
    for (int i = dimension - 2; i >= 0; i--) {
        strides[i] = strides[i + 1] * shape[i + 1];
    }
}

template <typename T>
void Array<T>::randomize(float upper, float lower) {
    random_device rd; 
    mt19937 gen(rd()); 
    uniform_real_distribution<float> dist(lower, upper); 
    for (int i = 0; i < getSize(); i++) {
        setData(i, dist(gen));
    }
}

// work on
template <typename T>
void Array<T>::print() {
    vector<int> indices(getDim(), 0);

    for (int i = 0; i < getSize(); i++) {
        // bracket printing
        for (int d = 0; d < getDim(); d++) {
            if (indices[d] == 0) {
                cout << "[";
            }
        }

        cout << at(indices);

        // Update indices to traverse to the next element
        for (int d = getDim() - 1; d >= 0; d--) {
            indices[d]++;
            if (indices[d] < shape[d]) {
                break;
            }
            indices[d] = 0; 
            cout << "]";    
        }

        if (i < getSize() - 1) {
            cout << ", ";
        }
    }

    for (int d = 0; d < getDim(); d++) {
        cout << "]";
    }
    cout << endl;
}

template <typename T>
int Array<T>::flatIndex(const vector<int> indices) const {
    int oneDimIndex = 0;
    for (int i = 0; i < dimension; i++) {
        oneDimIndex += indices[i] * strides[i];
    }
    return oneDimIndex;
}

template <typename T>
T& Array<T>::at(const vector<int>& indices) {
    int oneDimIndex = flatIndex(indices);
    return data[oneDimIndex];
}