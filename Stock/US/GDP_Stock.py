import requests
import pandas as pd
import numpy as np
from datetime import datetime
import yfinance as yf

# FRED API配置（仅用于GDP数据）
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
    df = df[['date', 'value']].dropna()

    return df


def fetch_yahoo_data(ticker):
    """从雅虎财经获取数据"""
    print(f"正在从雅虎财经获取 {ticker} 数据...")
    # 获取尽可能早的数据，从1950年开始
    data = yf.download(ticker, start='1950-01-01', progress=False)
    df = pd.DataFrame({
        'date': pd.to_datetime(data.index),
        'value': data['Close'].values.flatten()
    })
    df = df.reset_index(drop=True)
    df = df.dropna()
    return df


def format_date(date):
    """格式化日期为 日-月-年"""
    return date.strftime('%d-%m-%Y')


def resample_to_quarterly(df):
    """将数据重采样为季度"""
    df = df.set_index('date')
    # 重采样到每季度最后一天
    df_quarterly = df.resample('Q').last()
    # 前向填充缺失值
    df_quarterly = df_quarterly.ffill()
    df_quarterly = df_quarterly.reset_index()
    return df_quarterly


# 获取数据
print("正在获取Real GDP数据...")
df_gdp = fetch_fred_data('GDPC1')  # 美国Real GDP - 实际GDP（季度数据，经通胀调整）

print("正在从雅虎财经获取指数数据...")
df_djia = fetch_yahoo_data('^DJI')  # 道琼斯工业指数
df_sp500 = fetch_yahoo_data('^GSPC')  # S&P 500指数
df_nasdaq = fetch_yahoo_data('^IXIC')  # 纳斯达克综合指数
df_russell2000 = fetch_yahoo_data('^RUT')  # 罗素2000指数

# 转换为季度数据
print("正在处理季度数据...")
df_gdp = resample_to_quarterly(df_gdp)
df_djia = resample_to_quarterly(df_djia)
df_sp500 = resample_to_quarterly(df_sp500)
df_nasdaq = resample_to_quarterly(df_nasdaq)
df_russell2000 = resample_to_quarterly(df_russell2000)

# 创建统一的日期范围
print("正在创建统一日期范围...")
all_dfs = [df_gdp, df_djia, df_sp500, df_nasdaq, df_russell2000]
min_date = max([df['date'].min() for df in all_dfs])  # 使用最晚的开始日期
max_date = min([df['date'].max() for df in all_dfs])  # 使用最早的结束日期

# 创建统一的季度日期范围
unified_dates = pd.date_range(start=min_date, end=max_date, freq='Q')
df_unified = pd.DataFrame({'date': unified_dates})


# 将所有数据对齐到统一日期
def align_to_unified_dates(df, unified_df):
    """将数据对齐到统一日期范围"""
    merged = unified_df.merge(df, on='date', how='left')
    merged['value'] = merged['value'].ffill()
    return merged


df_gdp_aligned = align_to_unified_dates(df_gdp, df_unified.copy())
df_djia_aligned = align_to_unified_dates(df_djia, df_unified.copy())
df_sp500_aligned = align_to_unified_dates(df_sp500, df_unified.copy())
df_nasdaq_aligned = align_to_unified_dates(df_nasdaq, df_unified.copy())
df_russell2000_aligned = align_to_unified_dates(df_russell2000, df_unified.copy())

# 格式化日期
df_unified['日期'] = df_unified['date'].apply(format_date)

# 创建各个表（包含GDP数据和相关性）
df_gdp_table = pd.DataFrame({
    '日期': df_unified['日期'],
    'Real GDP (十亿美元)': df_gdp_aligned['value']
})


# 计算增长率（百分比变化）
def calculate_growth(series):
    """计算增长率"""
    return series.pct_change() * 100


gdp_growth = calculate_growth(df_gdp_aligned['value'])
djia_growth = calculate_growth(df_djia_aligned['value'])
sp500_growth = calculate_growth(df_sp500_aligned['value'])
nasdaq_growth = calculate_growth(df_nasdaq_aligned['value'])
russell2000_growth = calculate_growth(df_russell2000_aligned['value'])


# 计算Quadnomial（四列0/1数据，标签格式为二进制）
def calculate_quadnomial_columns(gdp_growth, index_growth):
    """
    计算GDP和指数移动方向的四种情况，返回四列数据：
    列1: (0,0) - GDP↓ & 指数↓
    列2: (0,1) - GDP↓ & 指数↑
    列3: (1,0) - GDP↑ & 指数↓
    列4: (1,1) - GDP↑ & 指数↑
    每列用1表示是该情况，0表示不是
    """
    col1, col2, col3, col4 = [], [], [], []

    for gdp_val, index_val in zip(gdp_growth, index_growth):
        if pd.isna(gdp_val) or pd.isna(index_val):
            col1.append(None)
            col2.append(None)
            col3.append(None)
            col4.append(None)
        elif gdp_val < 0 and index_val < 0:
            # (0,0)
            col1.append(1)
            col2.append(0)
            col3.append(0)
            col4.append(0)
        elif gdp_val < 0 and index_val >= 0:
            # (0,1)
            col1.append(0)
            col2.append(1)
            col3.append(0)
            col4.append(0)
        elif gdp_val >= 0 and index_val < 0:
            # (1,0)
            col1.append(0)
            col2.append(0)
            col3.append(1)
            col4.append(0)
        else:
            # (1,1)
            col1.append(0)
            col2.append(0)
            col3.append(0)
            col4.append(1)

    return col1, col2, col3, col4


djia_q1, djia_q2, djia_q3, djia_q4 = calculate_quadnomial_columns(gdp_growth, djia_growth)
sp500_q1, sp500_q2, sp500_q3, sp500_q4 = calculate_quadnomial_columns(gdp_growth, sp500_growth)
nasdaq_q1, nasdaq_q2, nasdaq_q3, nasdaq_q4 = calculate_quadnomial_columns(gdp_growth, nasdaq_growth)
russell2000_q1, russell2000_q2, russell2000_q3, russell2000_q4 = calculate_quadnomial_columns(gdp_growth,
                                                                                              russell2000_growth)

# 计算相关性
gdp_vals = df_gdp_aligned['value'].values
djia_correlation = np.corrcoef(gdp_vals, df_djia_aligned['value'].values)[0, 1]
sp500_correlation = np.corrcoef(gdp_vals, df_sp500_aligned['value'].values)[0, 1]
nasdaq_correlation = np.corrcoef(gdp_vals, df_nasdaq_aligned['value'].values)[0, 1]
russell2000_correlation = np.corrcoef(gdp_vals, df_russell2000_aligned['value'].values)[0, 1]


# 计算每行的相关性数据（使用滚动窗口相关性）
def calculate_rolling_correlation(gdp_series, index_series, window=20):
    """计算滚动相关性"""
    return gdp_series.rolling(window=window, min_periods=5).corr(index_series)


djia_rolling_corr = calculate_rolling_correlation(df_gdp_aligned['value'], df_djia_aligned['value'])
sp500_rolling_corr = calculate_rolling_correlation(df_gdp_aligned['value'], df_sp500_aligned['value'])
nasdaq_rolling_corr = calculate_rolling_correlation(df_gdp_aligned['value'], df_nasdaq_aligned['value'])
russell2000_rolling_corr = calculate_rolling_correlation(df_gdp_aligned['value'], df_russell2000_aligned['value'])

# 创建各个表（包含GDP数据、整体相关性、滚动相关性和Quadnomial四列）
df_djia_table = pd.DataFrame({
    '日期': df_unified['日期'],
    'Real GDP (十亿美元)': df_gdp_aligned['value'],
    '道琼斯工业指数': df_djia_aligned['value'],
    'Correlation': djia_correlation,
    'Rolling Correlation': djia_rolling_corr,
    '(0,0)': djia_q1,
    '(0,1)': djia_q2,
    '(1,0)': djia_q3,
    '(1,1)': djia_q4
})

df_sp500_table = pd.DataFrame({
    '日期': df_unified['日期'],
    'Real GDP (十亿美元)': df_gdp_aligned['value'],
    'S&P 500指数': df_sp500_aligned['value'],
    'Correlation': sp500_correlation,
    'Rolling Correlation': sp500_rolling_corr,
    '(0,0)': sp500_q1,
    '(0,1)': sp500_q2,
    '(1,0)': sp500_q3,
    '(1,1)': sp500_q4
})

df_nasdaq_table = pd.DataFrame({
    '日期': df_unified['日期'],
    'Real GDP (十亿美元)': df_gdp_aligned['value'],
    '纳斯达克指数': df_nasdaq_aligned['value'],
    'Correlation': nasdaq_correlation,
    'Rolling Correlation': nasdaq_rolling_corr,
    '(0,0)': nasdaq_q1,
    '(0,1)': nasdaq_q2,
    '(1,0)': nasdaq_q3,
    '(1,1)': nasdaq_q4
})

df_russell2000_table = pd.DataFrame({
    '日期': df_unified['日期'],
    'Real GDP (十亿美元)': df_gdp_aligned['value'],
    '罗素2000指数': df_russell2000_aligned['value'],
    'Correlation': russell2000_correlation,
    'Rolling Correlation': russell2000_rolling_corr,
    '(0,0)': russell2000_q1,
    '(0,1)': russell2000_q2,
    '(1,0)': russell2000_q3,
    '(1,1)': russell2000_q4
})


# 创建Quadnomial统计表（计数和比例）
def count_quadnomial_columns(q1, q2, q3, q4):
    """统计四种情况的数量和比例"""
    # 计算有效数据总数（排除None）
    valid_count = sum(1 for x in q1 if x is not None)

    count1 = sum(1 for x in q1 if x == 1)
    count2 = sum(1 for x in q2 if x == 1)
    count3 = sum(1 for x in q3 if x == 1)
    count4 = sum(1 for x in q4 if x == 1)

    if valid_count > 0:
        ratio1 = count1 / valid_count * 100
        ratio2 = count2 / valid_count * 100
        ratio3 = count3 / valid_count * 100
        ratio4 = count4 / valid_count * 100
    else:
        ratio1 = ratio2 = ratio3 = ratio4 = 0

    return count1, count2, count3, count4, ratio1, ratio2, ratio3, ratio4


djia_counts = count_quadnomial_columns(djia_q1, djia_q2, djia_q3, djia_q4)
sp500_counts = count_quadnomial_columns(sp500_q1, sp500_q2, sp500_q3, sp500_q4)
nasdaq_counts = count_quadnomial_columns(nasdaq_q1, nasdaq_q2, nasdaq_q3, nasdaq_q4)
russell2000_counts = count_quadnomial_columns(russell2000_q1, russell2000_q2, russell2000_q3, russell2000_q4)

# 统计表 - 次数
df_quadnomial_count = pd.DataFrame({
    '情况': ['(0,0)', '(0,1)', '(1,0)', '(1,1)'],
    '道琼斯': [djia_counts[0], djia_counts[1], djia_counts[2], djia_counts[3]],
    'S&P500': [sp500_counts[0], sp500_counts[1], sp500_counts[2], sp500_counts[3]],
    '纳斯达克': [nasdaq_counts[0], nasdaq_counts[1], nasdaq_counts[2], nasdaq_counts[3]],
    '罗素2000': [russell2000_counts[0], russell2000_counts[1], russell2000_counts[2], russell2000_counts[3]]
})

# 统计表 - 比例(%)
df_quadnomial_ratio = pd.DataFrame({
    '情况': ['(0,0)', '(0,1)', '(1,0)', '(1,1)'],
    '道琼斯(%)': [djia_counts[4], djia_counts[5], djia_counts[6], djia_counts[7]],
    'S&P500(%)': [sp500_counts[4], sp500_counts[5], sp500_counts[6], sp500_counts[7]],
    '纳斯达克(%)': [nasdaq_counts[4], nasdaq_counts[5], nasdaq_counts[6], nasdaq_counts[7]],
    '罗素2000(%)': [russell2000_counts[4], russell2000_counts[5], russell2000_counts[6], russell2000_counts[7]]
})

# 创建Excel文件
print("正在创建Excel文件...")
output_file = 'gdp_indices.xlsx'

with pd.ExcelWriter(output_file, engine='xlsxwriter') as writer:
    workbook = writer.book

    # GDP单独表
    df_gdp_table.to_excel(writer, sheet_name='GDP', index=False)
    worksheet = writer.sheets['GDP']

    # 创建GDP折线图
    chart = workbook.add_chart({'type': 'line'})
    max_row = len(df_gdp_table)

    chart.add_series({
        'name': 'Real GDP (十亿美元)',
        'categories': ['GDP', 1, 0, max_row, 0],
        'values': ['GDP', 1, 1, max_row, 1],
        'line': {'width': 1.5}
    })

    chart.set_title({'name': 'Real GDP'})
    chart.set_x_axis({'name': '日期', 'label_position': 'low'})
    chart.set_y_axis({'name': 'Real GDP (十亿美元)', 'label_position': 'low'})
    chart.set_size({'width': 720, 'height': 400})
    chart.set_legend({'position': 'bottom'})

    worksheet.insert_chart('D2', chart)

    # 指数数据集（包含GDP和相关性），添加统计行
    indices_datasets = [
        ('道琼斯工业指数', df_djia_table, '道琼斯工业指数', 2, djia_counts),
        ('S&P500指数', df_sp500_table, 'S&P 500指数', 2, sp500_counts),
        ('纳斯达克指数', df_nasdaq_table, '纳斯达克指数', 2, nasdaq_counts),
        ('罗素2000指数', df_russell2000_table, '罗素2000指数', 2, russell2000_counts)
    ]

    # 为每个指数创建表和图表
    for name, df_table, col_name, index_col, counts in indices_datasets:
        # 写入主数据
        df_table.to_excel(writer, sheet_name=name, index=False, startrow=0)
        worksheet = writer.sheets[name]
        max_row = len(df_table)

        # 添加两行统计数据
        stats_start_row = max_row + 2

        # 统计行1: 数量
        worksheet.write(stats_start_row, 0, '统计-数量')
        worksheet.write(stats_start_row, 5, counts[0])  # (0,0)
        worksheet.write(stats_start_row, 6, counts[1])  # (0,1)
        worksheet.write(stats_start_row, 7, counts[2])  # (1,0)
        worksheet.write(stats_start_row, 8, counts[3])  # (1,1)

        # 统计行2: 比例(%)
        worksheet.write(stats_start_row + 1, 0, '统计-比例(%)')
        worksheet.write(stats_start_row + 1, 5, counts[4])  # (0,0)%
        worksheet.write(stats_start_row + 1, 6, counts[5])  # (0,1)%
        worksheet.write(stats_start_row + 1, 7, counts[6])  # (1,0)%
        worksheet.write(stats_start_row + 1, 8, counts[7])  # (1,1)%

        # 创建指数自己的折线图
        chart_index = workbook.add_chart({'type': 'line'})

        chart_index.add_series({
            'name': col_name,
            'categories': [name, 1, 0, max_row, 0],
            'values': [name, 1, index_col, max_row, index_col],
            'line': {'width': 1.5}
        })

        chart_index.set_title({'name': f'{name}'})
        chart_index.set_x_axis({'name': '日期', 'label_position': 'low'})
        chart_index.set_y_axis({'name': col_name, 'label_position': 'low'})
        chart_index.set_size({'width': 720, 'height': 400})
        chart_index.set_legend({'position': 'bottom'})

        worksheet.insert_chart('K2', chart_index)

        # 创建Correlation折线图（整体相关系数）
        chart_corr = workbook.add_chart({'type': 'line'})

        chart_corr.add_series({
            'name': 'Correlation',
            'categories': [name, 1, 0, max_row, 0],
            'values': [name, 1, 3, max_row, 3],  # Correlation列
            'line': {'width': 1.5}
        })

        chart_corr.set_title({'name': f'{name} - Correlation'})
        chart_corr.set_x_axis({'name': '日期', 'label_position': 'low'})
        chart_corr.set_y_axis({'name': 'Correlation', 'label_position': 'low'})
        chart_corr.set_size({'width': 720, 'height': 400})
        chart_corr.set_legend({'position': 'bottom'})

        worksheet.insert_chart('K24', chart_corr)

        # 创建Rolling Correlation折线图（滚动相关系数）
        chart_rolling = workbook.add_chart({'type': 'line'})

        chart_rolling.add_series({
            'name': 'Rolling Correlation',
            'categories': [name, 1, 0, max_row, 0],
            'values': [name, 1, 4, max_row, 4],  # Rolling Correlation列
            'line': {'width': 1.5}
        })

        chart_rolling.set_title({'name': f'{name} - Rolling Correlation'})
        chart_rolling.set_x_axis({'name': '日期', 'label_position': 'low'})
        chart_rolling.set_y_axis({'name': 'Rolling Correlation', 'label_position': 'low'})
        chart_rolling.set_size({'width': 720, 'height': 400})
        chart_rolling.set_legend({'position': 'bottom'})

        worksheet.insert_chart('K46', chart_rolling)

    # 添加Quadnomial统计表 - 次数
    df_quadnomial_count.to_excel(writer, sheet_name='Quadnomial统计-次数', index=False)
    worksheet_quad_count = writer.sheets['Quadnomial统计-次数']

    # 创建柱形图显示统计数据（次数）
    chart_quad_count = workbook.add_chart({'type': 'column'})

    for col in range(1, 5):  # 4个指数
        chart_quad_count.add_series({
            'name': ['Quadnomial统计-次数', 0, col],
            'categories': ['Quadnomial统计-次数', 1, 0, 4, 0],
            'values': ['Quadnomial统计-次数', 1, col, 4, col],
        })

    chart_quad_count.set_title({'name': 'GDP与指数移动方向统计（次数）'})
    chart_quad_count.set_x_axis({'name': '情况', 'label_position': 'low'})
    chart_quad_count.set_y_axis({'name': '次数', 'label_position': 'low'})
    chart_quad_count.set_size({'width': 720, 'height': 400})
    chart_quad_count.set_legend({'position': 'bottom'})

    worksheet_quad_count.insert_chart('G2', chart_quad_count)

    # 添加Quadnomial统计表 - 比例
    df_quadnomial_ratio.to_excel(writer, sheet_name='Quadnomial统计-比例', index=False)
    worksheet_quad_ratio = writer.sheets['Quadnomial统计-比例']

    # 创建柱形图显示统计数据（比例）
    chart_quad_ratio = workbook.add_chart({'type': 'column'})

    for col in range(1, 5):  # 4个指数
        chart_quad_ratio.add_series({
            'name': ['Quadnomial统计-比例', 0, col],
            'categories': ['Quadnomial统计-比例', 1, 0, 4, 0],
            'values': ['Quadnomial统计-比例', 1, col, 4, col],
        })

    chart_quad_ratio.set_title({'name': 'GDP与指数移动方向统计（比例%）'})
    chart_quad_ratio.set_x_axis({'name': '情况', 'label_position': 'low'})
    chart_quad_ratio.set_y_axis({'name': '比例(%)', 'label_position': 'low'})
    chart_quad_ratio.set_size({'width': 720, 'height': 400})
    chart_quad_ratio.set_legend({'position': 'bottom'})

    worksheet_quad_ratio.insert_chart('G2', chart_quad_ratio)

print(f"完成！文件已保存为: {output_file}")

