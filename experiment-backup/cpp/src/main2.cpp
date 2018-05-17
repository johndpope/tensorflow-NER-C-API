#include "ner_model.h"
#include <string>
#include <vector>
#include <iostream>
#include <thread>
int main(){
NerModel ner_model=NerModel();
const std::string path="../model/ner_frozen.pb";
int status=ner_model.Load(path);
std::vector<int>char_input_data={53, 11, 9, 5, 12};
std::vector<int>seg_input_data={0, 0, 1, 3, 0};
int length=0;
/*
std::vector<int>outputs_vector(4,0);
std::vector<std::vector<int> >char_input_vec;
for(int i=0;i<4;i++){
    char_input_vec.push_back(char_input_data);
}
std::vector<std::vector<int> >seg_input_vec;
for(int i=0;i<4;i++){
    seg_input_vec.push_back(seg_input_data);
}

std::vector<std::thread> threads;
for (std::size_t i = 0; i < 4; ++i) {
   threads.push_back(std::thread([&]{ner_model.Eval(char_input_vec[i],seg_input_vec[i],outputs_vector[i]);}));
}

for (std::size_t i = 0; i < 4; ++i) {
   threads[i].join();
}

for (int i=0;i<4;i++){
ner_model.Eval(char_input_vec[i],seg_input_vec[i],outputs_vector[i]);
}
for(int i=0;i<4;i++){
std::cout<<outputs_vector[i]<<std::endl;
}
*/

std::vector<int>char_input_data2=char_input_data;
std::vector<int>char_input_data3=char_input_data;
std::vector<int>char_input_data4=char_input_data;
std::vector<int>seg_input_data2=seg_input_data;
std::vector<int>seg_input_data3=seg_input_data;
std::vector<int>seg_input_data4=seg_input_data;
std::vector<std::thread> threads;
int length2=0;
int length3=0;
int length4=0;
threads.push_back(std::thread([&]{ner_model.Eval(char_input_data,seg_input_data,length);}));
threads.push_back(std::thread([&]{ner_model.Eval(char_input_data2,seg_input_data2,length2);}));
threads.push_back(std::thread([&]{ner_model.Eval(char_input_data3,seg_input_data3,length3);}));
threads.push_back(std::thread([&]{ner_model.Eval(char_input_data4,seg_input_data4,length4);}));
for (std::size_t i = 0; i < 4; ++i) {
   threads[i].join();
}
std::cout<<length<<std::endl;
std::cout<<length2<<std::endl;
std::cout<<length3<<std::endl;
std::cout<<length4<<std::endl;


}
