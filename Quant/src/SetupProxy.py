import os

proxy = "http://127.0.0.1:7897"
os.environ["HTTP_PROXY"] = proxy
os.environ["HTTPS_PROXY"] = proxy