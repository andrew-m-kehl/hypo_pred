import pandas as pd
import os
import subprocess
import numpy as np
from scipy.signal import find_peaks

pd.set_option('display.max_rows', None)
pd.set_option('display.max_columns', None)

base_path = os.path.dirname(os.path.abspath(__file__))

# --- Configuration ---
#case_name = "case_10_snu"  #HT DN
#case_name = "case_409_snu"  #HT DN
#case_name = "case_416_snu"
#case_name = "case_6102_snu"  #HT No DN
case_name = "case_6280_snu"  #No HT



#case_name = "case_105_snu"
#case_name = "case_142_snu"
#case_name = "case_153_snu"



#Real Hypotension
#case_name = "case_2195_snu"
#case_name = "case_5293_snu"




data_path = os.path.join(base_path, "data")
file_path = os.path.join(data_path, f"{case_name}.csv")
print ("Opening: ", file_path)
df_signal = pd.read_csv(file_path, header=None)



def generate_dynamic_stls(df):
    print("Analyzing patient heart rate to generate dynamic STL formulas...")
    
    # 1. Smooth the signal lightly to find true systolic peaks
    smoothed_pressure = df[1].rolling(window=5, min_periods=1).mean()
    peaks, _ = find_peaks(smoothed_pressure, distance=50, prominence=10)
    
    # 2. Calculate the base window
    if len(peaks) < 2:
        print("Warning: Could not detect enough heartbeats. Defaulting to 0.75s baseline.")
        main_window = 0.7
    else:
        peak_times = df[0].iloc[peaks].values
        beat_intervals = np.diff(peak_times)
        median_interval = np.median(beat_intervals)
        heartrate = round (60/median_interval,1)
        
        print("Heartrate: ",heartrate, "bpm")
        print("Beat interval: 1 beat every", round(median_interval,3), "seconds")
        
    
    #Until (# Main STL window, 90% of the actual beat interval)
    w_main = round(median_interval * 0.9, 3)
    #On (theta1 and theta 2, 40% of beat interval)
    w_large = round(median_interval * 0.40, 3) 
    #On (theta3, 10% of beat interval)
    w_small = round(median_interval * 0.10, 3)
    
    print(f"Main Window: {w_main}s")
    print(f"Large Window: +/- {w_large}s")
    print(f"Small Window: +/- {w_small}s")

    # 4. Generate the exact STL syntax with the dynamic variables
    theta1 = f"(>= (On (-{w_large} {w_large}) (Min x0)) x0)"
    theta1_str = f"(Until (0 {w_main}) 0 (Get x0) (>= (On (-{w_large} {w_large}) (Min x0)) x0))"
    theta2 = f"(<= (On (-{w_large} {w_large}) (Max x0)) x0)"
    theta2_str = f"(Until (0 {w_main}) 0 (Get x0) (<= (On (-{w_large} {w_large}) (Max x0)) x0))"
    theta3 = f"""(and 
        (>= (On (-{w_small} {w_small}) (Min x0)) x0)
        (not 
        (>= (On (-{w_large} {w_large}) (Min x0)) x0)))"""
    theta3_str = f"""(Until (0 {w_main}) 0 (Get x0) 
        (and 
            (>= (On (-{w_small} {w_small}) (Min x0)) x0)
        (not 
            (>= (On (-{w_large} {w_large}) (Min x0)) x0))))"""

    # 5. Write the files to your directory
    with open("theta1.stl", 'w') as f: f.write(theta1)
    with open("theta1_val.stl", 'w') as f: f.write(theta1_str)
    with open("theta2.stl", 'w') as f: f.write(theta2)
    with open("theta2_val.stl", 'w') as f: f.write(theta2_str)
    with open("theta3.stl", 'w') as f: f.write(theta3)
    with open("theta3_val.stl", 'w') as f: f.write(theta3_str)
    
    print("Successfully generated theta1.stl, theta1_val.stl, theta2.stl, theta2_val.stl, theta3.stl and theta3_val.stl")

# ==========================================
# EXECUTION

generate_dynamic_stls(df_signal)

stle_bin = "./build/bin/stle"
thetas = ["theta1", "theta2", "theta3", "theta1_val","theta2_val", "theta3_val"]

for name in thetas:
        stl_file = f"{name}.stl"
        output_file = f"{name}.csv"
        
        if not os.path.exists(stl_file):
            print(f"Skipping {stl_file}: File not found.")
            continue
        
        print(f"Running analysis for {name}...")
        command = f"{stle_bin} -isf fso {file_path} -ff {stl_file} -os 1 -osf c > {output_file}"
        
        try:
            subprocess.run(command, shell=True, check=True)
            print(f"Done! Created {output_file}")
        except subprocess.CalledProcessError as e:
            print(f"Error executing {name}: {e}")


print("Computing PI and Dicpleth")

df_t1val = pd.read_csv("theta1_val.csv", header=None, names=['Time', 'Min'])
df_t2val = pd.read_csv("theta2_val.csv", header=None, names=['Time', 'Max'])
df_t3val = pd.read_csv("theta3_val.csv", header=None, names=['Time', 'Notch'])

df_t1val = df_t1val[df_t1val['Min'] > 0].reset_index(drop=True)
df_t2val = df_t2val[df_t2val['Max'] > 0].reset_index(drop=True)
df_t3val = df_t3val[df_t3val['Notch'] > 0].reset_index(drop=True)

sizec = len(df_t1val)
sizea = len(df_t2val)
sizeb = len(df_t3val)

df_comb = pd.DataFrame(np.nan, index=range(sizec), columns=range(11))

j = 0
k = 0
l = 0
avg_sum = 0
avg_previous = 0
#pi_bl = 1.7
#dp_bl = .54

baseline_dp_list = []
baseline_pi_list = []
pi_bl = None
dp_bl = None
CALIBRATION_BEATS = 60

for i in range(sizec):
    theta1_time = df_t1val.iloc[i, 0]
    theta1_value = df_t1val.iloc[i, 1]

    next_theta1_time = df_t1val.iloc[i+1, 0] if i + 1 < sizec else float('inf')

    while j < sizea and df_t2val.iloc[j, 0] <= theta1_time:
        j += 1

    while k < sizeb and df_t3val.iloc[k, 0] <= theta1_time:
        k += 1

    df_comb.iloc[i, 0] = theta1_time
    df_comb.iloc[i, 1] = theta1_value

    if j < sizea and df_t2val.iloc[j, 0] < next_theta1_time:
        theta2_time = df_t2val.iloc[j, 0]
        theta2_value = df_t2val.iloc[j, 1]
        
        a = theta2_value - theta1_value
        map_val = theta1_value + (a / 3)
        pi = a / theta1_value if theta1_value != 0 else 0

        avg_sum += map_val
        l += 1
        if l == 20:
            avg = avg_sum / l
            df_comb.iloc[i - int(l/2), 9] = round(avg, 2)
            
            avg_sum = 0
            if avg < 65:
                x=0
            elif avg_previous > avg:
                delta = avg_previous-avg
                n = (avg - 65)/delta
                tth = (theta1_time - last_time)*n
                minutes = tth/60
                if minutes <1:
                    df_comb.loc[i - int(l/2),10] = 1
            l = 0
            avg_previous = avg
            last_time = theta1_time

        df_comb.iloc[i, 2] = theta2_value
        df_comb.iloc[i, 4] = round(map_val, 2)
        df_comb.iloc[i, 6] = round(pi, 3)
       

        if k < sizeb and theta2_time < df_t3val.iloc[k, 0] < next_theta1_time:
            theta3_time = df_t3val.iloc[k, 0]
            theta3_value = df_t3val.iloc[k, 1]
            
            b = theta3_value - theta1_value
            dp = b / a if a != 0 else 0
            
            df_comb.iloc[i, 3] = theta3_value
            df_comb.iloc[i, 5] = round(dp, 3)
            # -----------------------------------------
            # 1. BASELINE CALIBRATION PHASE
            # -----------------------------------------
            if pi_bl is None and dp_bl is None:
                # Ensure MAP > 50 so we don't calibrate on flatlines or sensor noise
                if map_val > 50 and dp > 0: 
                    baseline_pi_list.append(pi)
                    baseline_dp_list.append(dp)
                    
                    # Once we hit 60 clean beats, lock in the baselines!
                    if len(baseline_pi_list) == CALIBRATION_BEATS:
                        pi_bl = np.median(baseline_pi_list)
                        dp_bl = np.median(baseline_dp_list)
                        print(f"Resting Baselines at beat {i} -> PI: {round(pi_bl, 3)} | DP: {round(dp_bl, 3)}")
                        # pi_bl = 1.2
                        # dp_bl = 0.1
                        # print(f"Hardcoded Baselines at beat {i} -> PI: {pi_bl} | DP: {dp_bl}")
                        print ("Computing Delta DP and PI")
            
            # -----------------------------------------
            # 2. DELTA CALCULATIONS
            # -----------------------------------------
            
            if pi_bl is not None and dp_bl is not None:
                # Calculate the relative percentages once baselines are established
                df_comb.iloc[i, 7] = round(100 * (dp - dp_bl) / dp_bl, 3)
                df_comb.iloc[i, 8] = round(100 * (pi - pi_bl) / pi_bl, 3)
            else:
                # Output 0.0 while the system is still calibrating the first 60 beats
                df_comb.iloc[i, 7] = 0.0
                df_comb.iloc[i, 8] = 0.0
            
            

output_filename = f"{case_name}_combine.csv"
output_file_path = os.path.join(data_path, output_filename)

df_comb = df_comb.dropna(subset=[0])

df_comb.to_csv(output_file_path, header=False, index=False)
print(f"Done! Created {output_file_path}")