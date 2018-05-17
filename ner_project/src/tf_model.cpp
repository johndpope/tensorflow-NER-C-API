#include "time.h"
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/platform/env.h"
#include "ner_model.h"
#include "tf_model.h"
#include "viterbi_decode.h"
using namespace tensorflow;
bool TFModel::LoadModel(const std::string &path,const char *charidpath,const char *idtagpath){
	ner_model.reset(new NerModel());
	if(!ner_model->Load(path))
		return false;
	std::vector<std::string> outputNames= {"crf_loss/transitions"};
	std::vector<tensorflow::Tensor> outputs;
	ner_model->Eval(outputNames,outputs);
	T=outputs[0].shape().dim_size(1);
	auto trans_tmap=outputs[0].tensor<float,2>();
	trans.resize(T);
	for (int i = 0; i < trans.size(); i++)
         trans[i].resize(T);
	for (int i=0;i<T;i++)
      for (int j=0;j<T;j++)
           trans[i][j]=trans_tmap(i,j);
	FILE *fp = fopen(charidpath, "r");
	char readBuffer[65536];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	char_to_id.ParseStream<0>(is);
	if (char_to_id.HasParseError())
  		printf("GetParseError %s\n", char_to_id.GetParseError());

	FILE *fp2 = fopen(idtagpath, "r");
	char readBuffer2[65536];
	FileReadStream is2(fp2, readBuffer2, sizeof(readBuffer2));
	id_to_tag.ParseStream<0>(is2);
	if (id_to_tag.HasParseError())
  		printf("GetParseError %s\n", id_to_tag.GetParseError());
}

bool TFModel::Parse(const std::string& sentence,std::vector<const char*>&tag){
	UnicodeStr ustr;
	double duration;
	BasicStringUtil::u8tou16(sentence.c_str(), sentence.size(), ustr);
	std::vector<int> char_input_data;
	std::vector<int> seg_input_data;
	double cusum=0;
	for(int j=0;j<ustr.size();j++){
    	std::string str;    
    	BasicStringUtil::u16tou8(&ustr[j],1, str);
    	if(char_to_id.HasMember(str.c_str()))
        	char_input_data.push_back(char_to_id[str.c_str()].GetInt());
    	else
         	char_input_data.push_back(char_to_id["<UNK>"].GetInt());
    	seg_input_data.push_back(0);   
	}
	int L=char_input_data.size()+1;
	std::vector<int>result(L);
	Parse(char_input_data,seg_input_data,result);
	for(int i=1;i<L;i++)
  		tag.push_back(id_to_tag[std::to_string(result[i]).c_str()].GetString());
}


bool TFModel::Parse(std::vector<int>&char_input_data,std::vector<int>&seg_input_data,std::vector<int>&result){
	int ndim=char_input_data.size();
	std::vector<std::pair<std::string, tensorflow::Tensor> > input;
	Tensor char_inputs(DT_INT32,TensorShape({1, ndim}));
	auto char_inputs_map = char_inputs.tensor<int, 2>();
	for (int j = 0; j < ndim; j++)
        char_inputs_map(0, j) =char_input_data[j];
	input.push_back(std::pair<std::string, tensorflow::Tensor>("ChatInputs", char_inputs));
	Tensor seg_inputs(DT_INT32,TensorShape({1, ndim}));
	auto seg_inputs_map = seg_inputs.tensor<int, 2>();
	for (int j = 0; j < ndim; j++)
    	seg_inputs_map(0, j) = seg_input_data[j];
	input.push_back(std::pair<std::string, tensorflow::Tensor>("SegInputs", seg_inputs));
	Tensor dropout(DT_FLOAT, TensorShape());
	dropout.scalar<float>()() = 1.0;
	input.push_back(std::pair<std::string, tensorflow::Tensor>("Dropout", dropout));
	std::vector<tensorflow::Tensor> outputs;
	std::vector<std::string> outputNames= {"project/logits_output"};
	ner_model->Eval(input,outputNames, outputs);
	auto logits_tmap=outputs[0].tensor<float,3>();
	int L=ndim+1;
	std::vector<std::vector<double> > logits(L);
	for (int i = 0; i <L; i++)
		logits[i].resize(T);
	for(int i=0;i<T-1;i++)
    	logits[0][i]=-1000.0;
	logits[0][T-1]=0.0;
	for(int i=1;i<L;i++){
    	logits[i][T-1]=-1000.0;
	}
	for(int i=1;i<L;i++)
    	for(int j=0;j<T-1;j++)
        	logits[i][j]=logits_tmap(0,i-1,j);
	viterbi_decode(logits,trans,result);
}
