import vitaldb
import pandas as pd
import numpy as np
import os

# --- Configuration ---
labels_file = "snuadc_hypotension_labels.csv"
output_dir = 'SNU_data'

# 100 Hz is standard for ART lines.
interval = 1/100 

# Define the case ID to start AFTER. Set to 0 to download all.
START_AFTER_CASE = 2966 

def main():
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # 1. Read the labels and filter for ONLY hypotension cases
    if not os.path.exists(labels_file):
        print(f"Error: '{labels_file}' not found. Please run the scanner script first.")
        return
        
    df_labels = pd.read_csv(labels_file)
    
    #Filter for has_hypotension == 1 AND caseid > START_AFTER_CASE ---
    df_filtered = df_labels[(df_labels['has_hypotension'] == 1) & (df_labels['caseid'] > START_AFTER_CASE)]
    hypo_cases = df_filtered['caseid'].astype(int).tolist()

    print("==================================================")
    print(f"Found {len(hypo_cases)} hypotensive SNUADC cases after case {START_AFTER_CASE} to download.")
    print("==================================================\n")

    # 2. Download and format directly to CSV
    for idx, cid in enumerate(hypo_cases):
        filename = os.path.join(output_dir, f"case_{cid}_snu.csv")
        
        # Check if we already have this file (resume capability)
        if os.path.exists(filename):
            print(f"[{idx+1}/{len(hypo_cases)}] Skipping Case {cid} (already exists).")
            continue

        print(f"[{idx+1}/{len(hypo_cases)}] Downloading Case {cid}...")
        try:
            # Download the raw array
            data = vitaldb.load_case(cid, ['SNUADC/ART'], interval=interval)

            # Flatten the array and create the synchronized time axis
            pressure_array = data.flatten()
            time_axis = np.arange(len(pressure_array)) * interval

            # Build DataFrame
            df = pd.DataFrame({
                'Time': time_axis,
                'Pressure': pressure_array
            })

            # Clean NaNs (crucial to prevent the STL engine from crashing)
            df = df.dropna()

            # Save directly to STL-ready CSV (no headers, no index)
            df.to_csv(filename, index=False, header=False)
            print(f"  -> Saved {len(df)} clean rows to {filename}")

        except Exception as e:
            print(f"  !! Failed to download Case {cid}: {e}")

    print("\nBatch Download Complete!")

if __name__ == "__main__":
    main()