import numpy as np
from data_utils import input_from_line,result_to_json
import tensorflow as tf
import pickle
from tensorflow.contrib.crf import viterbi_decode
def decode( logits, lengths, matrix):
        """
        :param logits: [batch_size, num_steps, num_tags]float32, logits
        :param lengths: [batch_size]int32, real length of each sequence
        :param matrix: transaction matrix for inference
        :return:
        """
        # inference final labels usa viterbi Algorithm
        paths = []
        small = -1000.0
        start = np.asarray([[small]*13 +[0]])
        for score, length in zip(logits, lengths):
            score = score[:length]
            pad = small * np.ones([length, 1])
            logits = np.concatenate([score, pad], axis=1)
            logits = np.concatenate([start, logits], axis=0)
            path, _ = viterbi_decode(logits, matrix)

            paths.append(path[1:])
        return paths

with open('maps.pkl', "rb") as f:
    char_to_id, id_to_char, tag_to_id, id_to_tag = pickle.load(f)
line='我是中国人'
_,chars,segs,tags=input_from_line(line,char_to_id)
print(chars)
print(segs)

output_graph_path='../model/ner_frozen.pb'
 
with tf.Graph().as_default():
      output_graph_def = tf.GraphDef()
      with open(output_graph_path, "rb") as f:
        output_graph_def.ParseFromString(f.read())
        _ = tf.import_graph_def(output_graph_def, name="")

      with tf.Session() as sess:
        char_inputs=sess.graph.get_tensor_by_name('ChatInputs:0')
        seg_inputs=sess.graph.get_tensor_by_name('SegInputs:0')
        dropout= sess.graph.get_tensor_by_name('Dropout:0')
        feed_dict={char_inputs:np.asarray(chars),
                   seg_inputs:np.asarray(segs),
                   dropout:1.0
                  }
        trans_node = sess.graph.get_tensor_by_name("crf_loss/transitions:0")
        lengths_node=sess.graph.get_tensor_by_name('lengths:0')
        logits_node=sess.graph.get_tensor_by_name('project/logits_output:0')
        lengths,logits,trans=sess.run([lengths_node,logits_node,trans_node],feed_dict)

batch_paths =decode(logits, lengths, trans)
print(batch_paths[0])
tags = [id_to_tag[idx] for idx in batch_paths[0]]
print(result_to_json(line, tags))
 
