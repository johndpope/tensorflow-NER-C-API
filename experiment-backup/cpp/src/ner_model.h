#ifndef NER_MODEL_H_
#define NER_MODEL_H_
#include <string>
#include <iostream>
#include <vector>
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/platform/env.h"
using namespace tensorflow;
class NerModel{
public:
    bool Load(const std::string &path);
    bool Eval(std::vector<int>&char_input_data,std::vector<int>&seg_input_data,int &length);

private:
     Session *session;
};

#endif // TF_TFMODEL_H_
