import yfinance as yf
import pandas as pd
import datetime as dt
import os

from yfinance.utils import auto_adjust

proxy = "http://127.0.0.1:7897"
os.environ["HTTP_PROXY"] = proxy
os.environ["HTTPS_PROXY"] = proxy


def download_stock_data(ticker):
    t = yf.Ticker(ticker)
    data = t.history(period="max",auto_adjust=False,interval="1mo")
    return data

def save_to_csv(data,filename):
    data.to_csv(filename)

stock_data = download_stock_data("PEGA")
stock_data.to_csv("F:/stock_data_PEGA2.csv")
