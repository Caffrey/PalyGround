import requests
import pandas as pd
from datetime import datetime

# FRED API Configuration
API_KEY = '78ee2a0b88a6417d321613e9f4fb1e14'
BASE_URL = 'https://api.stlouisfed.org/fred/series/observations'


def fetch_fred_data(series_id):
    """Fetch data from FRED API"""
    params = {
        'series_id': series_id,
        'api_key': API_KEY,
        'file_type': 'json'
    }
    response = requests.get(BASE_URL, params=params)
    data = response.json()

    # Check for errors
    if 'error_message' in data:
        print(f"Error: {series_id} - {data['error_message']}")
        return pd.DataFrame(columns=['date', 'value'])

    # Check for observations
    if 'observations' not in data:
        print(f"Warning: {series_id} returned no data")
        print(f"Response: {data}")
        return pd.DataFrame(columns=['date', 'value'])

    # Convert to DataFrame
    observations = data['observations']
    df = pd.DataFrame(observations)
    df['date'] = pd.to_datetime(df['date'])
    df['value'] = pd.to_numeric(df['value'], errors='coerce')
    df = df[['date', 'value']].dropna()

    return df


def format_date(date):
    """Format date as DD-MM-YYYY"""
    return date.strftime('%d-%m-%Y')


# Fetch ISM data
print("Fetching ISM data...")

# ISM Manufacturing PMI
print("- ISM Manufacturing PMI...")
df_ism_manufacturing = fetch_fred_data('MANEMP')  # ISM Manufacturing PMI

# ISM Non-Manufacturing/Services PMI
print("- ISM Non-Manufacturing PMI...")
df_ism_services = fetch_fred_data('NMFCI')  # ISM Non-Manufacturing Index

# ISM Manufacturing New Orders Index
print("- ISM New Orders Index...")
df_ism_new_orders = fetch_fred_data('NEWORDER')  # ISM New Orders Index

# ISM Manufacturing Employment Index
print("- ISM Employment Index...")
df_ism_employment = fetch_fred_data('ISMMFGEMP')  # ISM Employment Index

# ISM Manufacturing Prices Index
print("- ISM Prices Paid Index...")
df_ism_prices = fetch_fred_data('PPIFGS')  # ISM Prices Paid Index

# ISM Manufacturing Inventories Index
print("- ISM Inventories Index...")
df_ism_inventories = fetch_fred_data('BUSINV')  # ISM Inventories Index

# ISM Manufacturing Supplier Deliveries Index
print("- ISM Supplier Deliveries Index...")
df_ism_deliveries = fetch_fred_data('DLVRYIDX')  # ISM Supplier Deliveries Index

# Format dates
for df in [df_ism_manufacturing, df_ism_services, df_ism_new_orders,
           df_ism_employment, df_ism_prices, df_ism_inventories, df_ism_deliveries]:
    if not df.empty:
        df['Date'] = df['date'].apply(format_date)
        df['Index Value'] = df['value']
        df.drop(['date', 'value'], axis=1, inplace=True)

# Create Excel file
print("Creating Excel file...")
output_file = 'ism_data.xlsx'

with pd.ExcelWriter(output_file, engine='xlsxwriter') as writer:
    workbook = writer.book

    # Define datasets
    datasets = [
        ('ISM Manufacturing PMI', df_ism_manufacturing, 'ISM Manufacturing PMI'),
        ('ISM Non-Manufacturing PMI', df_ism_services, 'ISM Non-Manufacturing PMI'),
        ('ISM New Orders Index', df_ism_new_orders, 'ISM New Orders Index'),
        ('ISM Employment Index', df_ism_employment, 'ISM Employment Index'),
        ('ISM Prices Paid Index', df_ism_prices, 'ISM Prices Paid Index'),
        ('ISM Inventories Index', df_ism_inventories, 'ISM Inventories Index'),
        ('ISM Supplier Deliveries', df_ism_deliveries, 'ISM Supplier Deliveries Index')
    ]

    # Create worksheet and chart for each dataset
    for sheet_name, df_data, chart_title in datasets:
        # Check if data is empty
        if df_data.empty:
            print(f"Skipping {sheet_name}: no data available")
            continue

        # Write data
        df_data.to_excel(writer, sheet_name=sheet_name, index=False)
        worksheet = writer.sheets[sheet_name]

        # Create line chart
        chart = workbook.add_chart({'type': 'line'})
        max_row = len(df_data)

        chart.add_series({
            'name': chart_title,
            'categories': [sheet_name, 1, 0, max_row, 0],
            'values': [sheet_name, 1, 1, max_row, 1],
            'line': {'width': 1.5}
        })

        chart.set_title({'name': chart_title})
        chart.set_x_axis({'name': 'Date', 'label_position': 'low'})
        chart.set_y_axis({'name': 'Index Value', 'label_position': 'low'})
        chart.set_size({'width': 900, 'height': 450})
        chart.set_legend({'position': 'bottom'})

        # Add 50 baseline (PMI expansion/contraction threshold)
        if 'PMI' in chart_title or 'Index' in chart_title:
            chart.set_y_axis({
                'name': 'Index Value',
                'label_position': 'low',
                'min': 30,
                'max': 70
            })

        worksheet.insert_chart('D2', chart)

    # Create summary sheet
    print("Creating summary sheet...")

    # Find common date range
    all_dfs = [df_ism_manufacturing, df_ism_services, df_ism_new_orders,
               df_ism_employment, df_ism_prices, df_ism_inventories, df_ism_deliveries]

    # Extract dates from all dataframes
    all_dates = []
    for df in all_dfs:
        if not df.empty:
            all_dates.extend(df['Date'].tolist())
    unique_dates = sorted(set(all_dates))

    if not unique_dates:
        print("Warning: No data available to create summary sheet")
        print("Please check FRED API connection or series IDs")

    # Create summary DataFrame
    df_summary = pd.DataFrame({'Date': unique_dates})

    # Merge all data
    df_summary = df_summary.merge(
        df_ism_manufacturing.rename(columns={'Index Value': 'ISM Manufacturing PMI'}),
        on='Date', how='left'
    )
    df_summary = df_summary.merge(
        df_ism_services.rename(columns={'Index Value': 'ISM Non-Manufacturing PMI'}),
        on='Date', how='left'
    )
    df_summary = df_summary.merge(
        df_ism_new_orders.rename(columns={'Index Value': 'ISM New Orders Index'}),
        on='Date', how='left'
    )
    df_summary = df_summary.merge(
        df_ism_employment.rename(columns={'Index Value': 'ISM Employment Index'}),
        on='Date', how='left'
    )
    df_summary = df_summary.merge(
        df_ism_prices.rename(columns={'Index Value': 'ISM Prices Paid Index'}),
        on='Date', how='left'
    )
    df_summary = df_summary.merge(
        df_ism_inventories.rename(columns={'Index Value': 'ISM Inventories Index'}),
        on='Date', how='left'
    )
    df_summary = df_summary.merge(
        df_ism_deliveries.rename(columns={'Index Value': 'ISM Supplier Deliveries'}),
        on='Date', how='left'
    )

    # Write summary sheet
    df_summary.to_excel(writer, sheet_name='ISM Data Summary', index=False)
    worksheet_summary = writer.sheets['ISM Data Summary']

    # Create summary line chart - Main PMI indices
    chart_summary = workbook.add_chart({'type': 'line'})
    max_row_summary = len(df_summary)

    # Manufacturing PMI
    chart_summary.add_series({
        'name': 'ISM Manufacturing PMI',
        'categories': ['ISM Data Summary', 1, 0, max_row_summary, 0],
        'values': ['ISM Data Summary', 1, 1, max_row_summary, 1],
        'line': {'width': 2}
    })

    # Non-Manufacturing PMI
    chart_summary.add_series({
        'name': 'ISM Non-Manufacturing PMI',
        'categories': ['ISM Data Summary', 1, 0, max_row_summary, 0],
        'values': ['ISM Data Summary', 1, 2, max_row_summary, 2],
        'line': {'width': 2}
    })

    chart_summary.set_title({'name': 'ISM Manufacturing vs Non-Manufacturing PMI'})
    chart_summary.set_x_axis({'name': 'Date', 'label_position': 'low'})
    chart_summary.set_y_axis({'name': 'Index Value', 'label_position': 'low', 'min': 30, 'max': 70})
    chart_summary.set_size({'width': 1000, 'height': 500})
    chart_summary.set_legend({'position': 'bottom'})

    worksheet_summary.insert_chart('J2', chart_summary)

    # Create manufacturing components comparison chart
    chart_components = workbook.add_chart({'type': 'line'})

    # New Orders
    chart_components.add_series({
        'name': 'ISM New Orders Index',
        'categories': ['ISM Data Summary', 1, 0, max_row_summary, 0],
        'values': ['ISM Data Summary', 1, 3, max_row_summary, 3],
        'line': {'width': 1.5}
    })

    # Employment
    chart_components.add_series({
        'name': 'ISM Employment Index',
        'categories': ['ISM Data Summary', 1, 0, max_row_summary, 0],
        'values': ['ISM Data Summary', 1, 4, max_row_summary, 4],
        'line': {'width': 1.5}
    })

    # Prices
    chart_components.add_series({
        'name': 'ISM Prices Paid Index',
        'categories': ['ISM Data Summary', 1, 0, max_row_summary, 0],
        'values': ['ISM Data Summary', 1, 5, max_row_summary, 5],
        'line': {'width': 1.5}
    })

    # Inventories
    chart_components.add_series({
        'name': 'ISM Inventories Index',
        'categories': ['ISM Data Summary', 1, 0, max_row_summary, 0],
        'values': ['ISM Data Summary', 1, 6, max_row_summary, 6],
        'line': {'width': 1.5}
    })

    chart_components.set_title({'name': 'ISM Manufacturing Components Comparison'})
    chart_components.set_x_axis({'name': 'Date', 'label_position': 'low'})
    chart_components.set_y_axis({'name': 'Index Value', 'label_position': 'low'})
    chart_components.set_size({'width': 1000, 'height': 500})
    chart_components.set_legend({'position': 'bottom'})

    worksheet_summary.insert_chart('J32', chart_components)

print(f"Complete! File saved as: {output_file}")
print("\nData Description:")
print("- ISM Manufacturing PMI: Measures manufacturing activity, >50 indicates expansion, <50 indicates contraction")
print("- ISM Non-Manufacturing PMI: Measures services sector activity")
print("- ISM New Orders Index: Leading indicator of future production activity")
print("- ISM Employment Index: Labor market conditions")
print("- ISM Prices Paid Index: Inflation pressure indicator")
print("- ISM Inventories Index: Inventory management status")
print("- ISM Supplier Deliveries Index: Supply chain pressure indicator")

