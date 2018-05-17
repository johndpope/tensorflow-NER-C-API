import pickle
f = open('maps.pkl','rb')
char_to_id, id_to_char, tag_to_id, id_to_tag = pickle.load(f)
import json
json.dump(char_to_id,open('model/char_to_id.json','w'),ensure_ascii=False)
json.dump(id_to_char,open('model/id_to_char.json','w'),ensure_ascii=False)
json.dump(tag_to_id,open('model/tag_to_id.json','w'),ensure_ascii=False)
json.dump(id_to_tag,open('model/id_to_tag.json','w'),ensure_ascii=False)
