# 基于tensorflow-C++ API的命名实体识别

#编译tensorflow，提取需要的头文件

1、tensorflow目录下./configure

2、bazel build //tensorflow:libtensorflow_cc.so

3、复制bazel-genfiles/ tensorflow third_party bazel-bin/tensorflow/libtensorflow_cc.so bazel-bin/tensorflow/libtensorflow_framework.so bazel-tensorflow-1.4.1到ner_project目录下

#用python训练好模型，并固化好图

1、在python_model_preprocess训练完模型后，运行pickle2json.py

2、将model文件夹和ckpt文件夹复制到ner_project目录下

3、在ner_project目录下的freeze_graph下执行build.sh


#编译c++代码

执行src下makefile
