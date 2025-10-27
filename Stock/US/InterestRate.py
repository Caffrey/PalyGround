import requests
import pandas as pd
import numpy as np
from datetime import datetime

# FRED API配置
API_KEY = '78ee2a0b88a6417d321613e9f4fb1e14'
BASE_URL = 'https://api.stlouisfed.org/fred/series/observations'


def fetch_fred_data(series_id):
    """从FRED API获取数据"""
    params = {
        'series_id': series_id,
        'api_key': API_KEY,
        'file_type': 'json'
    }
    response = requests.get(BASE_URL, params=params)
    data = response.json()

    # 转换为DataFrame
    observations = data['observations']
    df = pd.DataFrame(observations)
    df['date'] = pd.to_datetime(df['date'])
    df['value'] = pd.to_numeric(df['value'], errors='coerce')
    df = df[['date', 'value']]

    return df


def format_date(date):
    """格式化日期为 日-月-年"""
    return date.strftime('%d-%m-%Y')


# 获取数据
print("正在获取数据...")
df_1m = fetch_fred_data('DGS1MO')  # 1个月
df_3m = fetch_fred_data('DGS3MO')  # 3个月
df_6m = fetch_fred_data('DGS6MO')  # 6个月
df_1y = fetch_fred_data('DGS1')  # 1年
df_2y = fetch_fred_data('DGS2')  # 2年
df_3y = fetch_fred_data('DGS3')  # 3年
df_5y = fetch_fred_data('DGS5')  # 5年
df_7y = fetch_fred_data('DGS7')  # 7年
df_10y = fetch_fred_data('DGS10')  # 10年
df_20y = fetch_fred_data('DGS20')  # 20年
df_30y = fetch_fred_data('DGS30')  # 30年

# 重采样为月度数据并前向填充
print("正在处理月度数据...")


def resample_to_monthly(df):
    """将数据重采样为月度，并使用前向填充"""
    df = df.set_index('date')
    # 重采样到每月最后一天
    df_monthly = df.resample('ME').last()
    # 前向填充缺失值（没有的数据延续上一行）
    df_monthly = df_monthly.ffill()
    df_monthly = df_monthly.reset_index()
    return df_monthly


# 对所有数据进行月度重采样
df_1m = resample_to_monthly(df_1m)
df_3m = resample_to_monthly(df_3m)
df_6m = resample_to_monthly(df_6m)
df_1y = resample_to_monthly(df_1y)
df_2y = resample_to_monthly(df_2y)
df_3y = resample_to_monthly(df_3y)
df_5y = resample_to_monthly(df_5y)
df_7y = resample_to_monthly(df_7y)
df_10y = resample_to_monthly(df_10y)
df_20y = resample_to_monthly(df_20y)
df_30y = resample_to_monthly(df_30y)

# 计算10年-2年利差
df_spread = pd.merge(df_2y, df_10y, on='date', suffixes=('_2y', '_10y'))
df_spread['value'] = df_spread['value_10y'] - df_spread['value_2y']
df_spread = df_spread[['date', 'value']]

# 创建统一的日期范围
print("正在创建统一日期范围...")
all_dfs = [df_1m, df_3m, df_6m, df_1y, df_2y, df_3y, df_5y, df_7y, df_10y, df_20y, df_30y]
min_date = min([df['date'].min() for df in all_dfs])
max_date = max([df['date'].max() for df in all_dfs])

# 创建统一的月度日期范围
unified_dates = pd.date_range(start=min_date, end=max_date, freq='ME')
df_unified = pd.DataFrame({'date': unified_dates})


# 将所有数据对齐到统一日期并前向填充
def align_to_unified_dates(df, unified_df):
    """将数据对齐到统一日期范围"""
    merged = unified_df.merge(df, on='date', how='left')
    merged['value'] = merged['value'].ffill()
    return merged


df_1m_aligned = align_to_unified_dates(df_1m, df_unified.copy())
df_3m_aligned = align_to_unified_dates(df_3m, df_unified.copy())
df_6m_aligned = align_to_unified_dates(df_6m, df_unified.copy())
df_1y_aligned = align_to_unified_dates(df_1y, df_unified.copy())
df_2y_aligned = align_to_unified_dates(df_2y, df_unified.copy())
df_3y_aligned = align_to_unified_dates(df_3y, df_unified.copy())
df_5y_aligned = align_to_unified_dates(df_5y, df_unified.copy())
df_7y_aligned = align_to_unified_dates(df_7y, df_unified.copy())
df_10y_aligned = align_to_unified_dates(df_10y, df_unified.copy())
df_20y_aligned = align_to_unified_dates(df_20y, df_unified.copy())
df_30y_aligned = align_to_unified_dates(df_30y, df_unified.copy())

# 计算利差（基于对齐后的数据）
df_spread_aligned = df_unified.copy()
df_spread_aligned['value'] = df_10y_aligned['value'] - df_2y_aligned['value']

# 格式化日期（用于各个独立表）
for df in [df_1m, df_3m, df_6m, df_1y, df_2y, df_3y, df_5y, df_7y, df_10y, df_20y, df_30y, df_spread]:
    df['日期'] = df['date'].apply(format_date)
    df['利率(%)'] = df['value']
    df.drop(['date', 'value'], axis=1, inplace=True)

# 合并所有数据到汇总表（使用对齐后的数据）
df_all = df_unified.copy()
df_all['日期'] = df_all['date'].apply(format_date)
df_all['1个月利率(%)'] = df_1m_aligned['value']
df_all['3个月利率(%)'] = df_3m_aligned['value']
df_all['6个月利率(%)'] = df_6m_aligned['value']
df_all['1年利率(%)'] = df_1y_aligned['value']
df_all['2年利率(%)'] = df_2y_aligned['value']
df_all['3年利率(%)'] = df_3y_aligned['value']
df_all['5年利率(%)'] = df_5y_aligned['value']
df_all['7年利率(%)'] = df_7y_aligned['value']
df_all['10年利率(%)'] = df_10y_aligned['value']
df_all['20年利率(%)'] = df_20y_aligned['value']
df_all['30年利率(%)'] = df_30y_aligned['value']
df_all['10年-2年利差(%)'] = df_spread_aligned['value']
df_all = df_all.drop('date', axis=1)
df_all = df_all[['日期', '1个月利率(%)', '3个月利率(%)', '6个月利率(%)', '1年利率(%)',
                 '2年利率(%)', '3年利率(%)', '5年利率(%)', '7年利率(%)',
                 '10年利率(%)', '20年利率(%)', '30年利率(%)', '10年-2年利差(%)']]

# 创建Excel文件
print("正在创建Excel文件...")
output_file = 'federal_rates.xlsx'

with pd.ExcelWriter(output_file, engine='xlsxwriter') as writer:
    workbook = writer.book

    # 添加各个独立的表和图表
    datasets = [
        ('1个月利率', df_1m),
        ('3个月利率', df_3m),
        ('6个月利率', df_6m),
        ('1年利率', df_1y),
        ('2年利率', df_2y),
        ('3年利率', df_3y),
        ('5年利率', df_5y),
        ('7年利率', df_7y),
        ('10年利率', df_10y),
        ('20年利率', df_20y),
        ('30年利率', df_30y),
        ('10年-2年利差', df_spread)
    ]

    for name, df in datasets:
        # 写入数据
        df.to_excel(writer, sheet_name=name, index=False)
        worksheet = writer.sheets[name]

        # 创建折线图
        chart = workbook.add_chart({'type': 'line'})

        # 设置数据范围
        max_row = len(df)
        chart.add_series({
            'name': f'{name}',
            'categories': [name, 1, 0, max_row, 0],
            'values': [name, 1, 1, max_row, 1],
            'line': {'width': 1.5}
        })

        # 设置图表标题和轴标签
        chart.set_title({'name': f'{name}'})
        chart.set_x_axis({'name': '日期', 'label_position': 'low'})
        chart.set_y_axis({'name': '利率(%)', 'label_position': 'low'})
        chart.set_size({'width': 720, 'height': 400})
        chart.set_legend({'position': 'bottom'})

        # 添加图表到工作表
        worksheet.insert_chart('D2', chart)

    # 添加汇总表
    print("正在创建汇总表...")
    df_all.to_excel(writer, sheet_name='所有利率汇总', index=False, startrow=0)
    worksheet_all = writer.sheets['所有利率汇总']

    # 创建汇总折线图
    chart_line = workbook.add_chart({'type': 'line'})

    max_row_all = len(df_all)
    # 添加所有数据系列
    for col in range(1, 13):  # 12个利率列
        chart_line.add_series({
            'name': ['所有利率汇总', 0, col],
            'categories': ['所有利率汇总', 1, 0, max_row_all, 0],
            'values': ['所有利率汇总', 1, col, max_row_all, col],
            'line': {'width': 1.5}
        })

    chart_line.set_title({'name': '所有利率折线图'})
    chart_line.set_x_axis({'name': '日期', 'label_position': 'low'})
    chart_line.set_y_axis({'name': '利率(%)', 'label_position': 'low'})
    chart_line.set_size({'width': 900, 'height': 450})
    chart_line.set_legend({'position': 'bottom'})

    worksheet_all.insert_chart('O2', chart_line)

    # 创建汇总柱形图
    chart_bar = workbook.add_chart({'type': 'column'})

    # 添加所有数据系列
    for col in range(1, 13):  # 12个利率列
        chart_bar.add_series({
            'name': ['所有利率汇总', 0, col],
            'categories': ['所有利率汇总', 1, 0, max_row_all, 0],
            'values': ['所有利率汇总', 1, col, max_row_all, col],
        })

    chart_bar.set_title({'name': '所有利率柱形图'})
    chart_bar.set_x_axis({'name': '日期', 'label_position': 'low'})
    chart_bar.set_y_axis({'name': '利率(%)', 'label_position': 'low'})
    chart_bar.set_size({'width': 900, 'height': 450})
    chart_bar.set_legend({'position': 'bottom'})

    worksheet_all.insert_chart('O28', chart_bar)

print(f"完成！文件已保存为: {output_file}")
