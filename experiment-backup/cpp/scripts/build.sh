#!/bin/bash

# freeze the graph and the weights
python freeze_graph.py --input_graph=../model/ner.pbtxt --input_checkpoint=../ckpt/ner.ckpt --output_graph=../model/ner_frozen.pb --output_node_names=crf_loss/transitions,project/logits_output,lengths

