from main_dashboard import main_dashboard
import pandas as pd

file_path = 'input data small.xlsx'
df = pd.read_excel(file_path)

results = []
for index, row in df.head(100).iterrows():
    # Convert the row to a dictionary
    input_data = row.to_dict()

    # Call main_dashboard with the input_data
    output_data = main_dashboard(input_data)
    
    # Append the result to the results list
    results.append(output_data)

# Optionally, convert the results list to a DataFrame and save it to a new Excel file
output_df = pd.DataFrame(results)
output_df.to_excel('output_file.xlsx', index=False)