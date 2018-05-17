#ifndef NER_MODEL_H_
#define NER_MODEL_H_
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/platform/env.h"
using namespace tensorflow;
class NerModel{
public:
    bool Load(const std::string &path);
    bool Eval(std::vector<std::string>& outputNames,std::vector<tensorflow::Tensor>& outputTensors);
    bool Eval(std::vector<int>&char_input_data,std::vector<int>&seg_input_data,int &length);
    bool Eval(std::vector<std::pair<std::string, tensorflow::Tensor> >& inputTensors,
            std::vector<std::string>& outputNames,std::vector<tensorflow::Tensor>& outputTensors);
private:
     std::unique_ptr<Session> session;
};

#endif // TF_TFMODEL_H_
