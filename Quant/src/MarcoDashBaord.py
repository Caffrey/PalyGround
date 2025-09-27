import pandas as pd
from tests.test_fig_tools import values

import SetupProxy
import dbnomics as db
import plotly.graph_objects as go
import plotly.express as px

def GetData():
    df = db.fetch_series("STATJP", "CPIm", '001')
    return df;

def RunExcel():
    df = GetData()

    writer = pd.ExcelWriter('test.xlsx', engine='xlsxwriter')
    df.to_excel(writer)

    workbook = writer.book
    worksheet = writer.sheets['Sheet1']

    chart = workbook.add_chart({'type': 'line'})

def RunNoteBook():
    df = GetData()

    print("tsekjljk")

    fig = go.Figure(data=[go.Table(
        header=dict(values=["original_period","original_value"],
                    fill_color='paleturquoise',
                    align='left'),  # 指定标题

        cells=dict(values=[df['original_period'], df['original_value']],
                   fill_color='lavender',  # 给出数据，颜色和对齐方式
                   align='left'))
    ])


    fig.show()
    Data = go.Scatter(x=df['original_period'], y=df['original_value'],mode="lines")
    fig2 = go.Figure(Data)
    fig2.show()

RunNoteBook()