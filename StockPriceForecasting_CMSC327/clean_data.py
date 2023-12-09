import pandas as pd

# Load the CSV file into a DataFrame
df = pd.read_csv('../Data/Tweet.csv')

# Drop the rows with empty values in the 'writer' column
df = df.dropna(subset=['writer'])

# Save the resulting DataFrame as a new CSV file
df.to_csv('../Data/cleaned_Tweet.csv', index=False)
