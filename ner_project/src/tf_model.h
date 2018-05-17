#ifndef TF_MODEL_H_
#define TF_MODEL_H_
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/platform/env.h"
#include "ner_model.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filereadstream.h"
#include "sentence_breaker.h"
using namespace tensorflow;
using namespace rapidjson;
class TFModel{
public:
    bool LoadModel(const std::string &path,const char *charidpath,const char *idtagpath);
    bool Parse(std::vector<int>&char_input_data,std::vector<int>&seg_input_data,std::vector<int>&result);
    bool Parse(const std::string& sentence,std::vector<const char*>&tag);
private:
     std::unique_ptr<NerModel> ner_model;
     std::vector<std::vector<double> > trans;
     int T;
     Document char_to_id;
     Document id_to_tag;
};

#endif
