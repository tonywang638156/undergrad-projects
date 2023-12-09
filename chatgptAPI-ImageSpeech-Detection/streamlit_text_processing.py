import streamlit as st
import warnings
warnings.filterwarnings("ignore")

# Libraries
from google.cloud import vision, translate_v2 as translate, texttospeech
from openai import OpenAI
import pprint
import json

# Translator client
translate_client = translate.Client.from_service_account_json('...')

# OpenAI API key
OPEN_API_KEY = "..."
client = OpenAI(api_key=OPEN_API_KEY)

st.title(".....")

# Dropdown menu for languages
languages = {'English': 'en', 'Spanish': 'es', 'Chinese': 'zh-CN', 'Portuguese': 'pt', 'Urdu': 'ur', 'Hindi': 'hi'}
selected_language = st.selectbox('Choose a language:', list(languages.keys()))

# Display language
st.write(f'You selected: {selected_language}')

question = st.text_input(f'Ask ... a question in English and he will respond in {selected_language}')

# Process only if there is a question
if question:
    # Input for question
    messages = [{'role': 'system', 'content': 'You are a helpful assistant.'},
                {'role': 'user', 'content': question}]
    model = "gpt-3.5-turbo-1106"

    # Request the conversation completion
    completion = client.chat.completions.create(
        model=model,
        messages=messages
    )

    # Get response
    answer = completion.choices[0].message.content

    # Translate the response, if the selected language is not English
    if selected_language != 'English':
        translated_answer = translate_client.translate(answer, target_language=languages[selected_language])['translatedText']
        st.write("... translated:")
        st.write(translated_answer)
        text_to_speak = translated_answer
    else:
        st.write("...:")
        st.write(answer)
        text_to_speak = answer

    # Text-to-speech client
    tts_client = texttospeech.TextToSpeechClient.from_service_account_json('...')

    # Prepare the Text Input for TTS
    text_input = texttospeech.SynthesisInput(text=text_to_speak)

    # Set Up the Voice
    voice = texttospeech.VoiceSelectionParams(
        language_code=languages[selected_language], 
        ssml_gender=texttospeech.SsmlVoiceGender.MALE  # or FEMALE
    )

    # Configure the Audio Output
    audio_config = texttospeech.AudioConfig(
        audio_encoding=texttospeech.AudioEncoding.MP3
    )

    # Generate the Speech
    response = tts_client.synthesize_speech(
        input=text_input,
        voice=voice,
        audio_config=audio_config
    )

    # Save the Audio File
    audio_file_name = 'output.mp3'
    with open(audio_file_name, 'wb') as audio_file:
        audio_file.write(response.audio_content)

    # Play the Audio in Streamlit
    st.audio(audio_file_name, format='audio/mp3')
    st.write(answer)