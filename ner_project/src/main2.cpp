#include "ner_model.h"
#include "tf_model.h"
#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h> 
#include "basic_string_util.h"
#include "sentence_breaker.h"
#include "time.h"
int main(){
    TFModel tf_model=TFModel();
    tf_model.LoadModel("../model/ner_frozen.pb","../model/char_to_id.json","../model/id_to_tag.json");
    std::vector<const char*>tag;
    clock_t start, finish; 
    double duration; 
    std::ifstream fin("test.txt");
    if(!fin.is_open())
        printf("No file found\n");
    string line;
    while(getline(fin,line)){
        std::vector<const char*>tag;
        if(line.length()<=1)
            continue;
        start = clock(); 
        tf_model.Parse(line,tag);
        for(int i=0;i<tag.size();i++)
            std::cout<<tag[i]<<" ";
        finish = clock();
        duration = (double)(finish - start) / CLOCKS_PER_SEC; 
        std::cout<<duration<<std::endl;
    }
}
