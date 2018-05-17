#include "ner_model.h"
bool NerModel::Load(const std::string &path){
  Status status = NewSession(SessionOptions(), &session);
  if (!status.ok()) {
    std::cout << status.ToString() << "\n";
    return 1;
  }
GraphDef graph_def;
status = ReadBinaryProto(Env::Default(),path.c_str(), &graph_def);
if (!status.ok()) {
std::cout << status.ToString() << "\n";
return 1;
}
status=session->Create(graph_def);
  if (!status.ok()) {
    std::cout << status.ToString() << "\n";
    return 1;
  }
return 0;

}


bool NerModel::Eval(std::vector<int> &char_input_data,std::vector<int> &seg_input_data,int &length)
{
int ndim=char_input_data.size();
std::cout<<ndim<<std::endl;
std::vector<std::pair<std::string, tensorflow::Tensor> > input;
Tensor char_inputs(DT_INT32,TensorShape({1, ndim}));
auto char_inputs_map = char_inputs.tensor<int, 2>();
for (int j = 0; j < ndim; j++) {
        char_inputs_map(0, j) =char_input_data[j];
}
input.push_back(std::pair<std::string, tensorflow::Tensor>("ChatInputs", char_inputs));

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
Status status = session->Run(input, {"crf_loss/transitions","lengths","project/logits_output"}, {}, &outputs);
std::cout<<outputs[1].tensor<int,1>()(0)<<std::endl;

if (!status.ok()) {
    std::cout << status.ToString() << "\n";
    return 1;

}

auto output_tmap=outputs[1].tensor<int,1>();
length=output_tmap(0);
}


