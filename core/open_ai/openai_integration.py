import os

import openai

from utils.anilex_helper import get_api_key

os.environ["OPENAI_API_KEY"] = get_api_key()
client = openai.OpenAI()


try:
    response = client.chat.completions.create(
        model="gpt-3.5-turbo",
        messages=[{"role": "user", "content": "Hallo"}]
    )
    print(response.choices[0].message.content)
except Exception as e:
    print("Fehler:", e)
