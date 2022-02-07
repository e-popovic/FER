import datetime
import gzip
import json
import os
from itertools import islice
from pymongo import MongoClient, errors

try:
    conn = MongoClient('mongodb://localhost:27018',
                       username='root',
                       password='rootnmbp', )
    print("Connected successfully!")
except:
    print("Could not connect to MongoDB")
    quit()

db = conn.projekt3
collection = db.amazon_reviews
filename = 'amazon_data.txt'
with gzip.open('Arts.txt.gz', 'rb') as f, open(filename, 'wb') as f_out:
    f_out.write(f.read())

dict1 = []
fields_product = ['title', 'price']
fields_review = ['profileName', 'helpfulness', 'score', 'text']

with open(filename) as fh:
    while True:
        next_n_lines = list(islice(fh, 11))
        if not next_n_lines:
            break

        dict_id = {}
        dict_product = {}
        dict_review = {}

        dict_id["userId"] = next_n_lines[3].split(": ", 1)[1].strip()
        dict_id["productId"] = next_n_lines[0].split(": ", 1)[1].strip()
        dict_id["summary"] = next_n_lines[8].split(": ", 1)[1].strip()
        date_object = datetime.datetime.fromtimestamp(int(next_n_lines[7].split(": ", 1)[1].strip()))
        dict_id["time"] = date_object

        dict_product[fields_product[0]] = next_n_lines[1].split(": ", 1)[1].strip()
        try:
            dict_product[fields_product[1]] = float(next_n_lines[2].split(": ", 1)[1].strip())
        except ValueError:
            dict_product[fields_product[1]] = None

        for i in range(2):
            dict_review[fields_review[i]] = next_n_lines[i + 4].split(": ", 1)[1].strip()
        try:
            dict_review[fields_review[2]] = float(next_n_lines[6].split(": ", 1)[1].strip())
        except ValueError:
            dict_review[fields_review[2]] = None
        dict_review[fields_review[3]] = next_n_lines[9].split(": ", 1)[1].strip()

        dict_one_review = {}
        dict_whole_doc = {}
        dict_one_review["product"] = dict_product
        dict_one_review["review"] = dict_review

        dict_whole_doc["_id"] = dict_id
        dict_whole_doc["value"] = dict_one_review
        dict1.append(dict_whole_doc)

os.remove(filename)
try:
    collection.insert_many(dict1, ordered=False)
except errors.BulkWriteError as e:
    panic_list = list(filter(lambda x: x['code'] != 11000, e.details['writeErrors']))
    if len(panic_list) > 0:
        print(f"these are not duplicate errors {panic_list}")
