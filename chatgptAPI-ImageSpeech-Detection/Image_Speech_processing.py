from openai import OpenAI
import pprint

# chat api key from prof.saif
OPEN_API_KEY = "...."

client = OpenAI(api_key = OPEN_API_KEY)

# Convert image to base64
import base64

res = ""
cond = True

while (cond):
    decision = input("please provide valid input decision!  [image] or [url]  ")
    if (decision == 'image'):
        res = input("please enter the name of the image that's in jpg format")
        # this is using the image from the current folder
        with open(f"{res}.jpg", "rb") as image_file:
            base64_image = base64.b64encode(image_file.read()).decode('utf-8')
        # The conversation to send to ChatGPT:
        messages = [{'role': 'user', 
                     'content': [
                         {'type': 'text', 'text': 'What’s in this image?'},
                         {'type': 'image_url', 'image_url':{'url': f"data:image/jpeg;base64,{base64_image}", 'detail':'low'}}
                     ]}
                   ]
        cond = False
    elif (decision == 'url'):
        res = input("please enter the url of the image")
        # this is using url of the image
        # The conversation to send to ChatGPT. The image url now includes a variable telling ChatGPT to use low detail.
        messages = [{'role': 'user', 
                     'content': [
                         {'type': 'text', 'text': 'What’s in this image?'},
                         {'type': 'image_url', 'image_url':{'url':res, 'detail': 'low'}}
                     ]}
                   ]
        cond = False
    else:
        print("invalid input")






# The model has to be ChatGPT 4 with vision
model = "gpt-4-vision-preview"

# Request the conversation completion (increasing the number of tokens to allow for a more detailed response)
completion = client.chat.completions.create(
  model=model,
  messages=messages,
  max_tokens = 1000
)

# Calculate the costs
totalCost = (completion.usage.prompt_tokens*0.01 + completion.usage.completion_tokens*0.03)/1000

contents = completion.choices[0].message.content
# Print out the response:
#print("Response from ChatGPT:", contents)
print("Total Tokens:", completion.usage.total_tokens)
print("Total Costs:", totalCost)


#################################################################################################################
# Create a prompt for an image
imagePrompt = f"please do the following description image in cartoon, here is the description: {contents}" 

# Ask Dall-E 3 to create an image
imageResponse = client.images.generate(
    model = "dall-e-3",
    prompt = imagePrompt,
    quality = "standard", # You can set this to hd but it costs more money
    style = "natural", # vivid creates hyper-real and dramatic images, natural creates less hyper-real images.
    response_format = 'url' # You can also get back base64 responses
    )

# Import the ability to show images
from IPython.display import Image

revisedContents = imageResponse.data[0].revised_prompt
# Show the prompt and the image
#print("Revised Prompt:", revisedContents)
generatedImage = Image(url= imageResponse.data[0].url, width = 1000)


###################################################################################################################
import IPython
# Create text for the text-to-speech engine
theText = revisedContents
# List of available voices
listOfVoices = ["alloy", "echo", "fable", "onyx", "nova", "shimmer"]
passed = True
while (passed):
    selectedVoice = input(f"here are list of voices you can pick: {listOfVoices}, pick the one you like")
    if (selectedVoice in listOfVoices):
        passed = False
    else:
        print("sorry, the voice you pick is currently not available, please pick the voice from the lists")

# Ask ChatGPT to convert this text to speech
audioResponse = client.audio.speech.create(
    model="tts-1",
    voice=selectedVoice,
    input=theText
)

display(generatedImage)
# Play the sound
IPython.display.Audio(audioResponse.content)