import openai
import json

openai.api_key = ""

responseList = [
    "The following is a conversation with an AI assistant.\n\n",
    "Human: Hello, who are you?\n",
    "AI: I am an AI created by OpenAI.\n"
]

while(True):
    userInput = input()
    responseList.append("User: " + userInput + "\n")

    query = ""
    for l in responseList:
        query += l

    response = openai.Completion.create(
        model = "text-davinci-002",
        prompt = query,
        temperature = 0.9,
        max_tokens = 2000,
        top_p = 1,
        frequency_penalty = 0,
        presence_penalty = 0.6,
        stop = [" Human:", " AI:"]
    )

    jsonResponse = json.loads(str(response))
    responseParse = jsonResponse["choices"][0]["text"].replace("\n", "")

    print(responseParse)
    
    responseList.append(responseParse + "\n")
