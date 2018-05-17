#include "tensorflow/core/public/session.h"
#include "tensorflow/core/platform/env.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filereadstream.h"
#include <thread>
#include <iostream>
#include <vector>
#include "viterbi_decode.h"
using namespace rapidjson;
using namespace std;
using namespace tensorflow;

int main(int argc, char* argv[]) {
//new session
  Session* session;
  Status status = NewSession(SessionOptions(), &session);
  if (!status.ok()) {
    std::cout << status.ToString() << "\n";
    return 1;
  }
//load graph
GraphDef graph_def;
  status = ReadBinaryProto(Env::Default(), "../model/ner_frozen.pb", &graph_def);
  if (!status.ok()) {
    std::cout << status.ToString() << "\n";
    return 1;
  }

  status = session->Create(graph_def);
  if (!status.ok()) {
    std::cout << status.ToString() << "\n";
    return 1;
  }
//all_input
std::vector<std::pair<std::string, tensorflow::Tensor> > input;
int ndim = 5;
//char_input_data
FILE *fp = fopen("../model/char_to_id.json", "r");
char readBuffer[65536];
FileReadStream is(fp, readBuffer, sizeof(readBuffer));
Document bank;
bank.ParseStream<0>(is);
if (bank.HasParseError())
{
  printf("GetParseError %s\n", bank.GetParseError());
}
std::vector<int> char_input_data;
char_input_data.push_back(bank["我"].GetInt());
char_input_data.push_back(bank["是"].GetInt());
char_input_data.push_back(bank["中"].GetInt());
char_input_data.push_back(bank["国"].GetInt());
char_input_data.push_back(bank["人"].GetInt());
for (int i;i<char_input_data.size();i++)
{
    cout<<char_input_data[i]<<" ";
}


Tensor char_inputs(DT_INT32,TensorShape({1, ndim}));
auto char_inputs_map = char_inputs.tensor<int, 2>();

for (int j = 0; j < ndim; j++) {
        char_inputs_map(0, j) = char_input_data[j];
    }
input.push_back(std::pair<std::string, tensorflow::Tensor>("ChatInputs", char_inputs));

std::vector<int> seg_input_data={0, 0, 1, 3, 0};
Tensor seg_inputs(DT_INT32,TensorShape({1, ndim}));
auto seg_inputs_map = seg_inputs.tensor<int, 2>();
for (int j = 0; j < ndim; j++) {
        seg_inputs_map(0, j) = seg_input_data[j];
    }
input.push_back(std::pair<std::string, tensorflow::Tensor>("SegInputs", seg_inputs));

Tensor dropout(DT_FLOAT, TensorShape());
dropout.scalar<float>()() = 1.0;
input.push_back(std::pair<std::string, tensorflow::Tensor>("Dropout", dropout));
std::vector<tensorflow::Tensor> outputs;

std::vector<std::vector<tensorflow::Tensor> >outputs_vector(4);
std::vector<std::thread> threads;
for (std::size_t i = 0; i < 4; ++i) {
   threads.push_back(std::thread([&]{session->Run(input, {"crf_loss/transitions","lengths","project/logits_output"}, {}, &outputs_vector[i]); })); 
}
for (std::size_t i = 0; i < 4; ++i) {
   threads[i].join();
}/*
for(int j =0;j<outputs_vector[0].size();j++){
    cout<<outputs_vector[0][j].DebugString()<<endl;
}*/
/*
status = session->Run(input, {"crf_loss/transitions","lengths","project/logits_output"}, {}, &outputs);
if (!status.ok()) {
    std::cout << status.ToString() << "\n";
    return 1;

}

int L=outputs[2].shape().dim_size(1)+1;
int T=outputs[2].shape().dim_size(2)+1;
auto logits_tmap=outputs[2].tensor<float,3>();
auto trans_tmap=outputs[0].tensor<float,2>();

std::vector<std::vector<double> > logits(L);
for (int i = 0; i <L; i++)
     logits[i].resize(T);
for(int i=0;i<T-1;i++){
    logits[0][i]=-1000.0;
}
logits[0][T-1]=0.0;
for(int i=1;i<L;i++){
    logits[i][T-1]=-1000.0;
}

for(int i=1;i<L;i++){
    for(int j=0;j<T-1;j++){
        logits[i][j]=logits_tmap(0,i-1,j);
}
}

std::vector<std::vector<double> > trans(T);    
for (int i = 0; i < trans.size(); i++)
         trans[i].resize(T);
for (int i=0;i<T;i++)
   {
      for (int j=0;j<T;j++){
           trans[i][j]=trans_tmap(i,j);
   }
}
std::vector<int>result(L);
viterbi_decode(&logits,&trans,&result);

FILE *fp2 = fopen("../model/id_to_tag.json", "r");
char readBuffer2[65536];
FileReadStream is2(fp2, readBuffer2, sizeof(readBuffer2));
Document bank2;
bank2.ParseStream<0>(is2);
if (bank2.HasParseError())
{
  printf("GetParseError %s\n", bank2.GetParseError());
}

for(int i=1;i<L;i++){
    std::cout<<bank2[std::to_string(result[i]).c_str()].GetString()<<" ";
}

*/

}
