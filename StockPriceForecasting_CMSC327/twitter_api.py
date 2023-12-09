from dotenv import load_dotenv
import os
import requests
import time

class TwitterAPI:
    def __init__(self):
        # Load environment variables from .env file
        load_dotenv()
        # Get the bearer token from the environment variable
        self.bearer_token = os.getenv('BEARER_TOKEN')
        self.user_fields = "user.fields=public_metrics"

    def create_url(self, usernames_list):
        # Join usernames with commas
        usernames = ','.join(usernames_list)
        url = f"https://api.twitter.com/2/users/by?usernames={usernames}&{self.user_fields}"
        return url

    def bearer_oauth(self, r):
        r.headers["Authorization"] = f"Bearer {self.bearer_token}"
        r.headers["User-Agent"] = "v2UserLookupPython"
        return r

    def connect_to_endpoint(self, url):
        response = requests.request("GET", url, auth=self.bearer_oauth)
        if response.status_code != 200:
            if response.status_code == 429:
                print("Too many requests. Waiting for 15 minutes...")
                time.sleep(15 * 60)  # Wait for 15 minutes
                return self.connect_to_endpoint(url)
            elif response.status_code == 400:
                print(f"Request returned an error: {response.status_code} {response.text}")
                return None
            else:
                raise Exception(f"Request returned an error: {response.status_code} {response.text}")
        return response.json()

    
        #some twitter account has been deactivated, use dict to make sure they are filtered out
    def get_follower_counts(self, usernames_list):
        url = self.create_url(usernames_list)
        json_response = self.connect_to_endpoint(url)
        follower_counts = {user_data['username']: user_data['public_metrics']['followers_count'] for user_data in json_response['data']}
        return follower_counts
    
