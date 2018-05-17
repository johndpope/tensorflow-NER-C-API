#include "ner_model.h"
bool NerModel::Load(const std::string &path){
    session.reset(NewSession(SessionOptions()));
    GraphDef graph_def;
    Status status = ReadBinaryProto(Env::Default(),path.c_str(), &graph_def);
    if (!status.ok()) {
        std::cout << status.ToString() << "\n";
        return 0;
    }
    status=session->Create(graph_def);
    if (!status.ok()) {
        std::cout << status.ToString() << "\n";
        return 0;
    }
    graph_def.Clear();
    return 1;
}


bool NerModel::Eval(std::vector<int> &char_input_data,std::vector<int> &seg_input_data,int &length){
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
    Status status = session->Run(input, {"crf_loss/transitions","lengths","project/logits_output"}, {}, &outputs);
    if (!status.ok()) {
        std::cout << status.ToString() << "\n";
        return 1;
    }
    auto output_tmap=outputs[1].tensor<int,1>();
    length=output_tmap(0);
}

bool NerModel::Eval(std::vector<std::pair<std::string, tensorflow::Tensor> >& inputTensors,
         std::vector<std::string>& outputNames,std::vector<tensorflow::Tensor>& outputTensors){
    Status status=session->Run(inputTensors,outputNames,{},&outputTensors);   
    if(!status.ok()){
       std::cout<<status.ToString()<<"\n";
       return 0;
    }
    return 1;
}
bool NerModel::Eval(std::vector<std::string>& outputNames,std::vector<tensorflow::Tensor>& outputTensors){
    Status status=session->Run({},outputNames,{},&outputTensors);
    if(!status.ok()){
        std::cout<<status.ToString()<<"\n";
        return 0;
    }
   return 1;
}

