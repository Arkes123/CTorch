#pragma once

#include <vector>
#include <iostream>
#include <random>

using namespace std;

template <typename T>
class Array {
public:
    vector<int> shape_;
    vector<T> data_;
    vector<int> strides_;
    int dimension_;
    int size_;

    T& operator[](int index) {
        return data_[index];
    }

    // Constructor
    Array(const vector<int>& shape) : shape_(shape) {
        dimension_ = shape_.size();

        int size = 1;
        for (int i = 0; i < dimension_; i++) {  
            size *= shape[i];
        }

        size_ = size;

        data_.resize(size);  

        for (int i = 0; i < size; i++) {
            data_[i] = 0;
        }

        calcStrides();  
    }

    // Decopy constructor
    Array(const Array* other) {
        for (int i = 0; i < other->shape_.size(); i++) {
            shape_.push_back(other->shape_[i]);
        }
        for (int i = 0; i < other->data_.size(); i++) {
            data_.push_back(other->data_[i]);
        }
        for (int i = 0; i < other->strides_.size(); i++) {
            strides_.push_back(other->strides_[i]);
        }
        dimension_ = other->dimension_;
        size_ = other->size_;
    }

    // Destructor
    ~Array() {}

    void randomize(float lower, float upper);

    // Functions
    void calcStrides();
    int flatIndex(const vector<int> indices) const;
    void transpose();

    // Overloads 
    Array<T>* operator*(T scalar);
    bool operator==(const Array& other);

    void print();

    T& at(const vector<int>& indices);

};


// FUNCTION IMPLEMENTATIONS HAVE TO STAY HERE BECAUSE OF TEMPLATE
template <typename T>
void Array<T>::transpose() {
    if (shape_.size() > 2) {
        cout << "We only support 2d matrix transposing currently! Talk to the devs ;)" << endl;
    } else {
        vector<int> shaper = shape_;
        shape_ = {shaper[1], shaper[0]};
        calcStrides();
    }
}


template <typename T>
void Array<T>::calcStrides() {
    strides_.resize(dimension_);
    strides_[dimension_ - 1] = 1;
    for (int i = dimension_ - 2; i >= 0; i--) {
        strides_[i] = strides_[i + 1] * shape_[i + 1];
    }
}

template <typename T>
bool Array<T>::operator==(const Array& other) {
    vector<T> data = data_;
    vector<T> otherData = other.data_;

    if (data.shape_ != other.shape_) {
        return false;
    } else {
        for (int i = 0; i < size_ - 1; i++) {
            if (data[i] != otherData[i]) {
                return false;
            }
        }
    }
    return true;
}

template <typename T>
void Array<T>::randomize(float lower, float upper) {
    random_device rd; 
    mt19937 gen(rd()); 
    uniform_real_distribution<float> dist(lower, upper); 
    for (int i = 0; i < size_; i++) {
        data_[i] = dist(gen);
    }
}  

template <typename T>
void Array<T>::print() {
    vector<int> indices(dimension_, 0);

    for (int i = 0; i < size_; i++) {
        cout << at(indices) << " ";

        for (int d = dimension_ - 1; d >= 0; d--) {
            indices[d]++;
            if (indices[d] < shape_[d]) {
                break;
            }
            indices[d] = 0;
        }

        if (dimension_ >= 2 && indices[dimension_ - 1] == 0) {
            cout << endl;
        }
    }
}

template <typename T>
int Array<T>::flatIndex(const vector<int> indices) const {
    int oneDimIndex = 0;
    for (int i = 0; i < dimension_; i++) {
        oneDimIndex += indices[i] * strides_[i];
    }
    return oneDimIndex;
}

template <typename T>
T& Array<T>::at(const vector<int>& indices) {
    int oneDimIndex = flatIndex(indices);
    return data_[oneDimIndex];
}

template <typename T>
Array<T>* Array<T>::operator*(T scalar) {
    Array<T>* output = new Array<T>(shape_);
    for (int i = 0; i < data_.size(); i++) {
        output->data_[i] = data_[i] * scalar;
    }
    return output;
}