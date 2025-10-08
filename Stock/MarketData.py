import yfinance as yf
import pandas as pd
import datetime as dt
import os

from openpyxl.xml.functions import tostring
from yfinance.utils import auto_adjust

def colToExcel(col): # col is 1 based
    excelCol = str()
    div = col
    while div:
        (div, mod) = divmod(div-1, 26) # will return (x, 0 .. 25)
        excelCol = chr(mod + 65) + excelCol

    return excelCol

class ColData:
    def __init__(self,Name,Col):
        self.Col = Col
        self.ColName = colToExcel(self.Col)
        self.Col = Col-1# start from 0
        self.Name = Name
        self.StartRow = 1
    def write_title(self,work_sheet):
        work_sheet.write(self.ColName+"1",self.Name)

    def write_format(self,work_sheet):
        work_sheet.write(1,self.Col,self.Name)

proxy = "http://127.0.0.1:7897"
os.environ["HTTP_PROXY"] = proxy
os.environ["HTTPS_PROXY"] = proxy

def download_stock_data(ticker):
    t = yf.Ticker(ticker)
    data = t.history(period="max",auto_adjust=False)
    return data

def save_to_csv(data,filename):
    data.to_csv(filename)

def add_close_chart(sheet_name, stock_df, work_sheet, work_book, cols ):
    work_sheet.set_column('A:A', 20)

    chart = work_book.add_chart({'type': 'line'})

    max_row = len(stock_df) + 1
    for i in range(0,len(cols)):
        colData = cols[i]
        col = colData.Col
        date_col = 0
        start_row = 1
        name_row = 0
        chart.add_series({
            'name': [sheet_name, name_row, col],
            'categories': [sheet_name, start_row, date_col, max_row, date_col],
            'values': [sheet_name, start_row, col, max_row, col],
            'line': {'width': 1.00},
        })

    chart.set_x_axis({'name': 'Data', 'date_axis': True})
    chart.set_y_axis({'name': 'Close', 'major_gridlines': {'visible': False}})
    chart.show_na_as_empty_cell()
    work_sheet.insert_chart("L4", chart,{'x_scale': 2, 'y_scale': 1})

def run_series(excel_name,sotck_set, stock_excels):
    writer = pd.ExcelWriter("./StocksFiles/"+excel_name+".xlsx", engine='xlsxwriter')
    workbook = writer.book
    for i in range(len(sotck_set)):
        stock_name = sotck_set[i]
        excel_name = stock_excels[i]
        stock_data = download_stock_data(stock_name)
        stock_data.index = stock_data.index.tz_convert(None)
        stock_data.index = pd.to_datetime(stock_data.index, format="%Y-%m-%d")
        stock_data = stock_data.sort_values(by=['Date'],ascending=False)
        stock_data = stock_data.drop(['Dividends','Stock Splits'],axis=1)


        #


        stock_data.to_excel(writer, sheet_name=excel_name)

        # insert chart
        worksheet = writer.sheets[excel_name]
        total_rows = len(stock_data)+1

        Col_Close = ColData("Close",5)
        Col_RollHigbhIndex = ColData("Rolling Index High",7)
        Col_RollBearIndex = ColData("Rolling Bear Level",8)
        Col_BullIndex =  ColData("Bull Index",9)
        Col_BearIndex =  ColData("Bear Index",10)

        Col_RollHigbhIndex.write_title(worksheet)
        Col_RollBearIndex.write_title(worksheet)
        Col_BullIndex.write_title(worksheet)
        Col_BearIndex.write_title(worksheet)

        for i in range(2,total_rows):
            formula = "=MAX(E{StartRow}:E{EndRow})"
            cellName = Col_RollHigbhIndex.ColName + str(i)
            worksheet.write_formula(cellName,formula=formula.format(StartRow=i+1 ,EndRow=total_rows))

            formula = "={RowName}{Index} * 0.8"
            cellName = Col_RollBearIndex.ColName + str(i)
            worksheet.write_formula(cellName,formula=formula.format(RowName=Col_RollHigbhIndex.ColName,Index = i))

            formula = "=IF({CloseRowName}{RowIndex}>{BearRollingRowName}{RowIndex},{CloseRowName}{RowIndex},#N/A)"
            cellName = Col_BullIndex.ColName + str(i)
            worksheet.write_formula(cellName,formula=formula.format(CloseRowName="E",BearRollingRowName=Col_RollBearIndex.ColName,RowIndex=i))

            formula = "=IF({CloseRowName}{RowIndex}<={BearRollingRowName}{RowIndex},{CloseRowName}{RowIndex},#N/A)"
            cellName = Col_BearIndex.ColName + str(i)
            worksheet.write_formula(cellName,
            formula=formula.format(CloseRowName="E", BearRollingRowName=Col_RollBearIndex.ColName,
                                                       RowIndex=i))


        add_close_chart(excel_name, stock_data, worksheet, workbook,[Col_Close,Col_BearIndex])


    writer.close()


def RunMarket():
    # 市场配置字典
    markets = {
        "US": (["^GSPC", "^DJI", "^IXIC", "^NDX"], ["SP500", "DJIA", "NASDAQ-Composite", "Nasdaq-100"]),
        "Japan": (["^N225"], ["Nikke255"]),
        "EU": (["^STOXX50E", "^STOXX"], ["Eurostoxx 50", "Eurostoxx 600"]),
        "UK": (["^FTSE", "^FTMC", "^FTLC"], ["FTSE100", "FTSE250", "FTSE350"]),
        "GE": (["^GDAXI"], ["DAX 40"]),
        "HK": (["^HSI", "^HSCE"], ["HSI", "HSCEI"]),
        "AU": (["^AXJO"], ["ASX 200"])
    }

    all_tickers = []
    all_names = []

    # 循环每个市场调用 run_series
    for market, (tickers, names) in markets.items():
        run_series(market, tickers, names)
        # 汇总
        all_tickers += tickers
        all_names += names

    # 调用 All 市场
    run_series("All", all_tickers, all_names)

RunMarket()