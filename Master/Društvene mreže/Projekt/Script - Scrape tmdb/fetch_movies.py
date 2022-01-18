import requests,json,csv
from pymongo import MongoClient
from urllib.request import urlopen

client = MongoClient(
    'mongodb://localhost:27017/admin?readPreference=primary&appname=MongoDB%20Compass&directConnection=true&ssl=false')

db_movies = client.movies
collection_movies = db_movies["tmdb_movies"]

#document all the parameters as variables
API_key = '9d8b2e460dbec147195c50b31bf64246'

def get_data(API_key, Movie_ID):
    query = 'https://api.themoviedb.org/3/movie/'+Movie_ID+'?api_key='+API_key+'&language=en-US'
    response = requests.get(query)
    if response.status_code == 200:
        with open('movies_26_996.json', 'a') as f:
            json_data = json.dumps(response.json())
            json_without_slash = json.loads(json_data)
            json.dump(json_without_slash, f)
            f.write(',')

        print('Called movie '+Movie_ID)

max_id = 923526

for movie in range(1, max_id+1):
    get_data(API_key, str(movie))
