import psycopg2
import json


with psycopg2.connect('dbname=projekt1 user=postgres password=05334690') as conn:
    with conn.cursor() as cur:
        with open('D:/Stvari/Faks/Napredne baze podataka/data/my_data.json') as my_file:
            data = json.load(my_file)
            cur.execute(""" 
            CREATE TABLE IF NOT EXISTS public.games
                (
                    asin character varying COLLATE pg_catalog."default" NOT NULL,
                    "questiontype" character varying COLLATE pg_catalog."default" NOT NULL,
                    question character varying COLLATE pg_catalog."default" NOT NULL,
                    answer character varying COLLATE pg_catalog."default" NOT NULL,
                    "answertime" character varying COLLATE pg_catalog."default" NOT NULL,
                    CONSTRAINT games_pkey PRIMARY KEY (asin)
                ); 
""")
            query_sql = """ insert into public.games
                select * from json_populate_recordset(NULL::public.games, %s) """
            cur.execute(query_sql, (json.dumps(data),))

""""
import json

data = []
i = 0
for line in open('D:/Stvari/Faks/Napredne baze podataka/data/games_data.json', 'r'):
    data.append(json.loads(line))

for element in data:
    element.pop('answerType', None)

for element in data:
    element.pop('unixTime', None)

for element in data:
    element["asin"] = element["asin"] + str(i)
    i = i+1

with open('data.json', 'w') as data_file:
    data = json.dump(data, data_file)




D:/Stvari/Faks/Napredne baze podataka/data/games_data.json

CITANJE U JSON

import json

def parse(path):
  g = open(path, 'r')
  for l in g:
    yield json.dumps(eval(l))

f = open("output.strict", 'w')
for l in parse("D:/Stvari/Faks/Napredne baze podataka/data/qa_Video_Games.json"):
  f.write(l + '\n')

  """
