#include "../src/tensor.h"

template <typename T = float>
Tensor<T>* mean(Tensor<T>* input){
    int new_size = input->shape_.size();
    Tensor<T>* output = new Tensor<T>({new_size});
    (*output->prev_)[0] = input;    // update previous
    // output->operation_ = "mean";
    // output->num_prev = 1;


    // calculate mean depending on dimension
    float temp = 0.0f;
    if(input->shape_.size() == 1){
        for(int i = 0; i < input->data_->size_; i++){
            temp += (*input->data_)[i];
        }
        float n = static_cast<float>((input->data_)[0].size_);
        (*output->data_)[0] = temp / n;
    } else {
        for(int i = 0; i < input->shape_[0]; i++){
            // input->data_[i].print();
            for(int j = 0; j < input->shape_[1]; j++){
                temp += input->data_->at({i, j});
                cout << temp << endl;
            }
            float n = static_cast<float>(input->shape_[1]);
            output->data_->at({i}) = temp / n;
            temp = 0.0f;
        }
    }
    
    return output;
}

template <typename T = float>
Tensor<T>* relu(Tensor<T>* input){
    if (input->shape_.size() != 2) {
        std::cout << "Must be a 2D tensor" << std::endl;
        exit(EXIT_FAILURE);
    }
    Tensor<T>* t = new Tensor<T>(input->shape_);
    (*t->prev_)[0] = input;  // set previous
    t->operation_ = "relu";
    t->num_prev = 1;
    for(int i = 0; i < input->shape_[0]; i++){
        for(int j = 0; j < input->shape_[1]; j++){
            t->data_->at({i, j}) = input->data_->at({i, j}) > 0 ? input->data_->at({i, j}) : 0;
        }
    }
    return t;
}

template <typename T = float>
Tensor<T>* softmax(Tensor<T>* input){
    if (input->shape_.size() != 2) {
        std::cout << "Must be a 2D tensor" << std::endl;
        exit(EXIT_FAILURE);
    }

    int batch_size = input->shape_[0];
    int num_classes = input->shape_[1];
    Tensor<T>* t = new Tensor<T>(input->shape_);
    (*t->prev_)[0] = input;    
    t->operation_ = "softmax";
    t->num_prev = 1;

    for (int i = 0; i < batch_size; i++) {
        // Find the max value in the row for numerical stability
        T maxVal = input->data_->at({i, 0});
        for (int j = 1; j < num_classes; j++) {
            T val = input->data_->at({i, j});
            if (val > maxVal) {
                maxVal = val;
            }
        }

        // Compute exponentials and sum
        std::vector<T> exp_values(num_classes);
        T sum_exp = 0;
        for (int j = 0; j < num_classes; j++) {
            T exp_val = std::exp(input->data_->at({i, j}) - maxVal);
            exp_values[j] = exp_val;
            sum_exp += exp_val;
        }

        // Compute softmax
        for (int j = 0; j < num_classes; j++) {
            t->data_->at({i, j}) = exp_values[j] / sum_exp;
        }
    }

    return t;
}

template <typename T = float>
Tensor<T> mse_loss(Tensor<T>* predictions, Tensor<T>* targets) {
    // if (predictions->shape_ != targets->shape_) {
    //     std::cout << "shape should be the same" << std::endl;
    //     return nullptr;
        
    // }

    Tensor<float> diff = (*predictions) - (*targets);
    // Square the differences
    for(int i = 0; i < diff.shape_[0]; i++){
        for(int j = 0; j < diff.shape_[1]; j++){
            diff.data_->at({i, j}) = diff.data_->at({i, j}) * diff.data_->at({i, j});
        }
    }

    // Calculate the mean of the squared differences
    return *mean(&diff);
}
