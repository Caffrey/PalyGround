import wbdata
import pandas as pd
import datetime
import numpy as np

# -------------------------------
# Settings
# -------------------------------
start_date = datetime.datetime(1990, 1, 1)
end_date = datetime.datetime(2024, 1, 1)

countries = {
    'Japan': 'JP',
    'United States': 'US',
    'European Union': 'EU',
    'China': 'CN',
    'United Kingdom': 'GB',
    'Germany': 'DE',
    'Australia': 'AU',
    'World': 'WLD'
}

gdp_indicator = {'NY.GDP.MKTP.CD': 'GDP (Billion USD)'}

# -------------------------------
# Fetch GDP Data
# -------------------------------
gdp_data = {}
for country_name, country_code in countries.items():
    df = wbdata.get_dataframe(gdp_indicator, country=country_code, date=(start_date, end_date))
    df = df / 1e9  # Convert to Billion USD
    df.columns = [f'{country_name} GDP (Billion USD)']
    gdp_data[country_name] = df

df_gdp = pd.concat(gdp_data.values(), axis=1)
df_gdp = df_gdp.sort_index(ascending=False)
df_gdp = df_gdp.ffill()

world_column = 'World GDP (Billion USD)'

# -------------------------------
# GDP Share of World
# -------------------------------
df_share = pd.DataFrame(index=df_gdp.index)
for col in df_gdp.columns:
    if 'World' not in col:
        df_share[f'{col} Share (%)'] = df_gdp[col] / df_gdp[world_column]

# -------------------------------
# Year-over-Year Growth Rate
# -------------------------------
df_growth = df_gdp.pct_change(periods=-1) * 100
df_growth = df_growth.round(2)


# -------------------------------
# Rolling Correlation
# -------------------------------
def rolling_corr(df, target_col, window):
    df_corr = pd.DataFrame(index=df.index)
    for col in df.columns:
        if col != target_col:
            df_corr[col] = df[col].rolling(window=window).corr(df[target_col])
    return df_corr


window_corr = 5  # 所有滚动相关性统一使用 5 年
df_corr_5y = rolling_corr(df_gdp, world_column, window_corr)
df_corr_us_5y = rolling_corr(df_gdp, 'United States GDP (Billion USD)', window_corr)

# -------------------------------
# Save to Excel
# -------------------------------
excel_filename = 'gdp_analysis_full_5y_corr.xlsx'
with pd.ExcelWriter(excel_filename, engine='xlsxwriter') as writer:
    workbook = writer.book

    # --- GDP Data Sheet ---
    df_gdp.to_excel(writer, sheet_name='GDP Data')
    worksheet_gdp = writer.sheets['GDP Data']

    # --- GDP Share Sheet ---
    df_share.to_excel(writer, sheet_name='GDP Share')
    worksheet_share = writer.sheets['GDP Share']

    # --- GDP Growth Rate Sheet ---
    df_growth.to_excel(writer, sheet_name='GDP Growth Rate')
    worksheet_growth = writer.sheets['GDP Growth Rate']

    # --- Global 5Y Correlation Sheet ---
    df_corr_5y.to_excel(writer, sheet_name='5Y Correlation')
    worksheet_corr_5y = writer.sheets['5Y Correlation']

    # --- US Correlation Sheet ---
    df_corr_us_5y.to_excel(writer, sheet_name='Correlation with US')
    worksheet_corr_us = writer.sheets['Correlation with US']

    # -------------------------------
    # Create Charts
    nrows = len(df_gdp)

    # --- GDP Line Chart ---
    chart_sheet = workbook.add_worksheet('GDP Chart')
    chart_gdp = workbook.add_chart({'type': 'line'})
    for i, col in enumerate(df_gdp.columns):
        chart_gdp.add_series({
            'name': ['GDP Data', 0, i + 1],
            'categories': ['GDP Data', 1, 0, nrows, 0],
            'values': ['GDP Data', 1, i + 1, nrows, i + 1],
        })
    chart_gdp.set_title({'name': 'GDP Trends (Billion USD)'})
    chart_gdp.set_x_axis({'name': 'Year', 'reverse': True})
    chart_gdp.set_y_axis({'name': 'GDP (Billion USD)'})
    chart_gdp.set_legend({'position': 'bottom'})
    chart_sheet.insert_chart('B2', chart_gdp, {'x_scale': 3, 'y_scale': 2})

    # --- GDP Growth Chart ---
    growth_chart_sheet = workbook.add_worksheet('GDP Growth Chart')
    chart_growth = workbook.add_chart({'type': 'line'})
    for i, col in enumerate(df_growth.columns):
        chart_growth.add_series({
            'name': ['GDP Growth Rate', 0, i + 1],
            'categories': ['GDP Growth Rate', 1, 0, nrows, 0],
            'values': ['GDP Growth Rate', 1, i + 1, nrows, i + 1],
        })
    chart_growth.set_title({'name': 'GDP Year-over-Year Growth Rate (%)'})
    chart_growth.set_x_axis({'name': 'Year', 'reverse': True})
    chart_growth.set_y_axis({'name': 'Growth Rate (%)'})
    chart_growth.set_legend({'position': 'bottom'})
    growth_chart_sheet.insert_chart('B2', chart_growth, {'x_scale': 3, 'y_scale': 2})

    # --- Global 5Y Correlation Chart ---
    corr_chart_5y_sheet = workbook.add_worksheet('5Y Corr Chart')
    chart_corr_5y = workbook.add_chart({'type': 'line'})
    for i, col in enumerate(df_corr_5y.columns):
        chart_corr_5y.add_series({
            'name': ['5Y Correlation', 0, i + 1],
            'categories': ['5Y Correlation', 1, 0, nrows, 0],
            'values': ['5Y Correlation', 1, i + 1, nrows, i + 1],
        })
    chart_corr_5y.set_title({'name': '5-Year Rolling Correlation (%)'})
    chart_corr_5y.set_y_axis({'name': 'Correlation (%)'})
    chart_corr_5y.set_x_axis({'name': 'Year', 'reverse': True})
    corr_chart_5y_sheet.insert_chart('B2', chart_corr_5y, {'x_scale': 3, 'y_scale': 2})

    # --- US 5Y Correlation Chart ---
    corr_chart_us_sheet = workbook.add_worksheet('US Corr Chart')
    chart_corr_us = workbook.add_chart({'type': 'line'})
    for i, col in enumerate(df_corr_us_5y.columns):
        chart_corr_us.add_series({
            'name': ['Correlation with US', 0, i + 1],
            'categories': ['Correlation with US', 1, 0, nrows, 0],
            'values': ['Correlation with US', 1, i + 1, nrows, i + 1],
        })
    chart_corr_us.set_title({'name': '5-Year Rolling Correlation with US (%)'})
    chart_corr_us.set_y_axis({'name': 'Correlation (%)'})
    chart_corr_us.set_x_axis({'name': 'Year', 'reverse': True})
    corr_chart_us_sheet.insert_chart('B2', chart_corr_us, {'x_scale': 3, 'y_scale': 2})

    # -------------------------------
    # Formatting
    gdp_format = workbook.add_format({'num_format': '#,##0.00'})
    percent_format = workbook.add_format({'num_format': '0.00%'})

    for i, col in enumerate(df_gdp.columns):
        worksheet_gdp.set_column(i + 1, i + 1, 15, gdp_format)
    for i, col in enumerate(df_share.columns):
        worksheet_share.set_column(i + 1, i + 1, 15, percent_format)
    for i, col in enumerate(df_growth.columns):
        worksheet_growth.set_column(i + 1, i + 1, 15, gdp_format)
    for i, col in enumerate(df_corr_5y.columns):
        worksheet_corr_5y.set_column(i + 1, i + 1, 15, percent_format)
    for i, col in enumerate(df_corr_us_5y.columns):
        worksheet_corr_us.set_column(i + 1, i + 1, 15, percent_format)

print(f"Excel file saved: {excel_filename}")
