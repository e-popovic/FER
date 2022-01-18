import json
from urllib.request import urlopen
from flask import Flask
from flask import jsonify
from flask import redirect, url_for
from flask_cors import CORS
from flask_dance.contrib.twitter import make_twitter_blueprint, twitter
from flask_jsonpify import jsonify
from flask_restful import Resource, Api
from pymongo import MongoClient
from bson.objectid import ObjectId
from datetime import datetime
from bson.json_util import dumps
from pymongo import MongoClient

# config system
app = Flask(__name__)
api = Api(app)
CORS(app)

app.secret_key = "supersekrit"
blueprint = make_twitter_blueprint(
    api_key="API-KEY",
    api_secret="API-SECRET",
)
app.register_blueprint(blueprint, url_prefix="/login")

client = MongoClient(
    'mongodb://localhost:27017/admin?readPreference=primary&appname=MongoDB%20Compass&directConnection=true&ssl=false')

db_users = client.userbase
collection_users = db_users["users"]

db_help = client.help
collection_help = db_help["moviedetails"]

isAuthorised = False
myId = ''

# TMDB - pronadi popularne filmove
url_popular = "https://api.themoviedb.org/3/movie/popular?api_key=APIKEY&language=en-US&page=1"
data_movies = json.loads(urlopen(url_popular).read())['results']

db_movies = client.movies
collection_popular = db_movies["popular"]
collection_all = db_movies["all"]

db_quote = client.quotes
collection_quotes = db_quote["quotes"]

for movie in data_movies:
    # za debug mode, inace se napuni dvaput
    if collection_popular.count_documents({'id': movie['id']}) == 0:
        collection_popular.insert_one(movie)
        url_id = "https://api.themoviedb.org/3/movie/" + str(
            movie['id']) + "/external_ids?api_key=APIKEY"
        movie_ids = json.loads(urlopen(url_id).read())  # ima i fb, twitter, instagram
        collection_popular.update_one({'id': movie['id']}, {"$set": {'imdb_id': movie_ids['imdb_id']}})

        # OMDB - pronadi detalje svakog filma
        if collection_all.count_documents({'imdbID': movie_ids["imdb_id"]}) == 0:
            details_id = "http://www.omdbapi.com/?apikey=APIKEY&i=" + movie_ids["imdb_id"]
            movie_details = json.loads(urlopen(details_id).read())
            collection_all.insert_one(movie_details)


@app.route("/")
def hello():
    if not twitter.authorized:
        return redirect("http://127.0.0.1:4200/welcome-page")

    global isAuthorised
    isAuthorised = True

    resp = twitter.get("account/verify_credentials.json?include_email=true")
    # assert resp.ok
    user_data = resp.json()

    global myId
    myId = user_data["id_str"]

    if collection_users.count_documents({'id_str': user_data['id_str']}) == 0:
        one_user = {
            'id_str': user_data['id_str'],
            'email': user_data['email'],
            'name': user_data['name'],
            'screen_name': user_data['screen_name'],
        }
        collection_users.insert_one(one_user)
    return redirect("http://127.0.0.1:4200/trending")


@app.route("/login")
def index():
    return redirect(url_for("twitter.login"))


@app.route("/logout")
def logout():
    token = blueprint.token["oauth_token"]
    resp = twitter.post(
        "https://api.twitter.com/1.1/oauth/invalidate_token",
        params={"token": token},
        headers={"Content-Type": "application/x-www-form-urlencoded"}
    )
    # assert resp.ok, resp.text

    global isAuthorised
    isAuthorised = False

    del blueprint.token  # Delete OAuth token from storage
    return redirect("http://127.0.0.1:4200/welcome-page")


class TwitterLoginCheck(Resource):
    def get(self):
        return isAuthorised


class UserInfo(Resource):
    def get(self):
        global myId
        mojuser = collection_users.find_one({'id_str': myId}, {'email': 1, 'name': 1, 'screen_name': 1, '_id': 0})
        return jsonify(mojuser)


class PopularMovies(Resource):
    def get(self):
        return jsonify(list(collection_popular
                            .find({}, {'id': 1, 'poster_path': 1, 'title': 1, 'release_date': 1, '_id': 0})
                            .limit(15)))


class MovieDetails(Resource):
    def get(self):
        db_help = client.help
        collection_help = db_help["moviedetails"]
        # print(collection_help.find_one({}, {'id': 1, 'imdb_id': 1, 'original_language': 1, 'original_title': 1,
        #                                     'overview': 1, 'poster_path': 1, 'release_date': 1, 'runtime': 1,
        #                                     'genres': 1, 'content_warnings': 1, '_id': 0}))
        return jsonify(collection_help.find_one({}, {'imdb_id': 1, 'original_language': 1, 'original_title': 1,
                                            'overview': 1, 'genres': 1, 'poster_path': 1, 'release_date': 1, 'runtime': 1,
                                             'budget': 1, 'revenue': 1, 'content_warnings': 1, 'boxoffice': 1, '_id': 0}))


class HighestRatedMovies(Resource):
    def get(self):
        return jsonify(list(collection_all
                            .find({'imdbRating': {"$ne": "N/A"}},
                                  {'Title': 1, 'Year': 1, 'imdbRating': 1, 'imdbID': 1, '_id': 0}).sort("imdbRating",
                                                                                                        -1)
                            .limit(5)))


api.add_resource(PopularMovies, '/popular_movies')
api.add_resource(HighestRatedMovies, '/highest_rated_movies')
api.add_resource(UserInfo, '/userinfo')  # Route_3
api.add_resource(TwitterLoginCheck, '/login-check')

api.add_resource(MovieDetails, '/details')

if __name__ == '__main__':
    app.run(port=5000, debug=True)
    collection_popular.drop()
