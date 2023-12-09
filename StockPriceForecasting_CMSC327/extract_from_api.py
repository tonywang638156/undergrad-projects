from data_preprocess.twitter_api import TwitterAPI
import pandas as pd
import csv
import re


df = pd.read_csv('../Data/cleaned_Tweet.csv')

# Count the number of unique writers
writers = df['writer'].unique().tolist()
prev_length = len(writers)
writers = [writer for writer in writers if re.match('^[A-Za-z0-9_]{1,15}$', writer)]

# Print the result
print(f"filtered out {len(writers)-prev_length} as an invalid twitter name")
print('Number of unique writers:', len(writers))
# with open('../Data/follower_names.csv', 'w', newline='') as f:
#     writer = csv.writer(f)
#     for w in writers:
#         writer.writerow(f'{w}')
# test api
# twitter = TwitterAPI()
# follower_counts = twitter.get_follower_counts(["VisualStockRSRC", "OpenAI"])
# print(follower_counts)  # Output: [4166, 385495]

twitter = TwitterAPI()


# Initialize a CSV file to write the results
with open('../Data/follower_counts.csv', 'w', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(['writer', 'follower_count'])

batch_size = 100
# start_index = 20000 #mannually set, last left off
for i in range(0, len(writers), batch_size):
    batch = writers[i:i+batch_size]

    # Get the follower counts for the current batch of writers
    follower_counts = twitter.get_follower_counts(batch)

    # Write the results to the CSV file
    with open('../Data/follower_counts.csv', 'a', newline='') as f:
        print("Starting to write...")
        writer = csv.writer(f)
        for username, follower_count in follower_counts.items():
            writer.writerow([username, follower_count])
