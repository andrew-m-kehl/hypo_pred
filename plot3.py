import pandas as pd
import matplotlib.pyplot as plt
import os

# 1. Get the current directory of the script
base_path = os.path.dirname(os.path.abspath(__file__))

# 2. Point to the 'data' subfolder
data_path = os.path.join(base_path, "data")

# --- Configuration ---
case_name = "case_10_snu"  #HT DN
#case_name = "case_6102_snu"  #HT No DN
#case_name = "case_6280_snu"  #No HT


file_signal = os.path.join(data_path, f"{case_name}.csv")
file_combine = os.path.join(data_path, f"{case_name}_combine.csv")

file_t1 = "theta1.csv"
file_t2 = "theta2.csv"
file_t3 = "theta3.csv"
file_a = "theta2_val.csv"
file_b = "theta3_val.csv"
file_c = "theta1_val.csv"

# Zoom window (Seconds)
START_TIME = 0
END_TIME = 20000
# ==========================================
# LOADERS
# ==========================================
def load_logic_data(filename):
    if not os.path.exists(filename):
        return None, None
    df = pd.read_csv(filename, header=None, names=['Time', 'Val'])
    df['Logic'] = (df['Val'] == 0).astype(int)
    return df['Time'], df['Logic']

def load_value_data(filename):
    if not os.path.exists(filename):
        return None
    df = pd.read_csv(filename, header=None, names=['Time', 'Val'])
    return df

def load_combined_data(filename):
    """Loads the unified 11-column CSV"""
    if not os.path.exists(filename):
        print(f"Error: {filename} not found.")
        return None
    
    df = pd.read_csv(filename, header=None)
    
    if len(df.columns) >= 11:
        df = df.iloc[:, :11]
        df.columns = ['Time', 'Min', 'Max', 'Notch', 'MAP', 'DI_Base', 'PI_Base', 'DI_Delta', 'PI_Delta', 'AVG', 'TTH_Warning']
        return df
    else:
        print(f"Warning: {filename} does not have enough columns (Found {len(df.columns)}, expected 11).")
        return None

# ==========================================
# MAIN PLOTTER
# ==========================================
def main():
    if not os.path.exists(file_signal):
        print(f"Error: {file_signal} not found.")
        return

    print("Loading datasets...")
    df_sig = pd.read_csv(file_signal, header=None, names=['Time', 'Pressure'])
    
    # Smooth signal to flatten micro-bounces
    df_sig['Pressure'] = df_sig['Pressure'].rolling(window=5, min_periods=1).mean()
    
    df_a = load_value_data(file_a)
    df_b = load_value_data(file_b)
    df_c = load_value_data(file_c)
    
    # Load the unified file
    df_combo = load_combined_data(file_combine)

    # # Create 1 plot
    # fig, axes = plt.subplots(1, 1, figsize=(14, 6), sharex=True, 
    #     gridspec_kw={'height_ratios': [3]})
    # (ax_sig) = axes

    # # Create 2 subplots
    # fig, axes = plt.subplots(2, 1, figsize=(10, 8), sharex=True, 
    #     gridspec_kw={'height_ratios': [3, 1]})
    # (ax_sig, ax_thetas) = axes
        # Create 4 subplots
    
    # # Create 3 subplots
    # fig, axes = plt.subplots(3, 1, figsize=(14, 18), sharex=True, 
    #     gridspec_kw={'height_ratios': [4, 1.2, 1.2]})
    # (ax_sig, ax_pi_base, ax_pi_delta) = axes

    #    # Create 3 subplots
    # fig, axes = plt.subplots(3, 1, figsize=(14, 18), sharex=True, 
    #     gridspec_kw={'height_ratios': [4, 1.2, 1.2]})
    # (ax_sig, ax_di_base, ax_di_delta) = axes

    # # Create 4 subplots
    # fig, axes = plt.subplots(4, 1, figsize=(14, 18), sharex=True, 
    #     gridspec_kw={'height_ratios': [4, 1, 1.2, 1.2]})
    # (ax_sig, ax_thetas, ax_di_delta, ax_pi_delta) = axes

    # Create 5 subplots
    fig, axes = plt.subplots(5, 1, figsize=(14, 18), sharex=True, 
        gridspec_kw={'height_ratios': [4, 1.2, 1.2, 1.2, 1.2]})
    (ax_sig, ax_di_base, ax_di_delta, ax_pi_base, ax_pi_delta) = axes

    # # Create 6 subplots
    # fig, axes = plt.subplots(6, 1, figsize=(14, 18), sharex=True, 
    #     gridspec_kw={'height_ratios': [4, 1, 1.2, 1.2, 1.2, 1.2]})
    # (ax_sig, ax_thetas, ax_di_base, ax_di_delta, ax_pi_base, ax_pi_delta) = axes

    # ---------------------------------------------------------
    # Subplot 1: Arterial Signal + Overlays (aa, bb, c, MAP, AVG, TTH Warning)
    # ---------------------------------------------------------
    ax_sig.plot(df_sig['Time'], df_sig['Pressure'], color='gray', linewidth=1.0, alpha=1.0, label="Arterial Pressure")
    
    start_t = df_sig['Time'].iloc[0] if not df_sig.empty else 0.0
    zero_row = pd.DataFrame({'Time': [start_t], 'Val': [0.0]})

    # if df_c is not None and not df_c.empty:
    #     df_c_filt = df_c[df_c['Val'] > 0]
    #     if not df_c_filt.empty:
    #         df_c_step = pd.concat([zero_row, df_c_filt], ignore_index=True)
    #         ax_sig.step(df_c_step['Time'], df_c_step['Val'], color='blue', linewidth=1.5, where='post', label="c (Diastolic)", zorder=3)

    # if df_a is not None and not df_a.empty:
    #     df_a_filt = df_a[df_a['Val'] > 0]
    #     if not df_a_filt.empty:
    #         df_a_step = pd.concat([zero_row, df_a_filt], ignore_index=True)
    #         ax_sig.step(df_a_step['Time'], df_a_step['Val'], color='orange', linewidth=1.5, where='post', label="a (Systolic)", zorder=3)

    # if df_b is not None and not df_b.empty:
    #     df_b_filt = df_b[df_b['Val'] > 0]
    #     if not df_b_filt.empty:
    #         df_b_step = pd.concat([zero_row, df_b_filt], ignore_index=True)
    #         ax_sig.step(df_b_step['Time'], df_b_step['Val'], color='green', linewidth=1.5, where='post', label="b (Notch)", zorder=3)

   
    # -----------MAP-------------
    if df_combo is not None:
        df_map = df_combo[df_combo['MAP'] > 0]
        if not df_map.empty:
            ax_sig.plot(df_map['Time'], df_map['MAP'], label="MAP", color='purple', linewidth=0.75, linestyle='-', zorder=4)

    # -----------AVG-------------       
        df_avg = df_combo[df_combo['AVG'] > 0]
        if not df_avg.empty:
            ax_sig.plot(df_avg['Time'], df_avg['AVG'], label="AVG", color='blue', linewidth=0.75, linestyle='-', zorder=4)

    # -----------TTH-------------
    if 'TTH_Warning' in df_combo.columns:
            # Extract just the timestamps where a warning was triggered
            warn_times = df_combo.loc[df_combo['TTH_Warning'] == 1, 'Time']
            
            if not warn_times.empty:
                # Plot vertical lines from 20 to 120 (matching the y-axis limits)
                ax_sig.vlines(warn_times, ymin=20, ymax=180, colors='red', linewidth=0.9, alpha=0.8, zorder=2, label="TTH Warning")

    ax_sig.set_ylabel("Pressure\n(mmHg)")
    ax_sig.set_ylim(20, 180) 
    ax_sig.grid(True, linestyle='--', alpha=0.5)
    ax_sig.legend(loc='upper right', fontsize='x-small', ncol=6)
    ax_sig.axhline(y=65, color='red', linestyle=':', alpha=0.5, label="65mmHg (Hypotension)")

    # # ---------------------------------------------------------
    # # Subplot 2: Combined Logic Lanes (Theta 1, 2, 3)
    # # ---------------------------------------------------------
    # logic_configs = [
    #     (file_t1, 'blue', "Theta1 (Min)"),
    #     (file_t2, 'orange', "Theta2 (Max)"),
    #     (file_t3, 'green', "Theta3 (Notch)")
    # ]

    # for filename, color, label in logic_configs:
    #     t, val = load_logic_data(filename)
    #     if t is not None:
    #         ax_thetas.vlines(t, 0, val, color=color, linewidth=2.0, label=label, alpha=0.6)
        
    # ax_thetas.text(0.01, 0.85, "STL Satisfaction", transform=ax_thetas.transAxes, color='black', fontweight='bold', va='top')
    # ax_thetas.set_yticks([0, 1])
    # ax_thetas.set_yticklabels(['F', 'T'])
    # ax_thetas.set_ylim(-0.1, 1.2) 
    # ax_thetas.grid(True, linestyle='--', alpha=0.3)
    # ax_thetas.legend(loc='upper right', fontsize='x-small', ncol=3)

    # ---------------------------------------------------------
    # Subplot 3 & 4: DP (Base and Delta)
    # ---------------------------------------------------------
    if df_combo is not None:
        # Base DP
        ax_di_base.scatter(df_combo['Time'], df_combo['DI_Base'], color='magenta', s=20, alpha=0.9, zorder=3)
        ax_di_base.text(0.01, 0.85, "DP Index", transform=ax_di_base.transAxes, color='magenta', fontweight='bold', va='top')
        ax_di_base.set_ylabel("DP Value")
        ax_di_base.grid(True, linestyle='--', alpha=0.3)
        ax_di_base.set_ylim(0, 0.75) 
        #baseline
        ax_di_base.axhline(y=0.33, color='blue', linestyle=':', alpha=0.3, label="DP baseline")
        
        # Delta DP
        ax_di_delta.scatter(df_combo['Time'], df_combo['DI_Delta'], color='purple', s=20, alpha=0.9, zorder=3)
        ax_di_delta.text(0.01, 0.85, "Delta DP", transform=ax_di_delta.transAxes, color='purple', fontweight='bold', va='top')
        ax_di_delta.set_ylabel("Change")
        ax_di_delta.set_ylim(-100, 100)
        ax_di_delta.grid(True, linestyle='--', alpha=0.3)
        ax_di_delta.axhline(y=-19, color='red', linestyle=':', alpha=0.3, label="Delta DP Trigger")

    # ---------------------------------------------------------
    # Subplot 5 & 6: PI (Base and Delta)
    # ---------------------------------------------------------
    if df_combo is not None:
        # Base PI
        ax_pi_base.scatter(df_combo['Time'], df_combo['PI_Base'], color='teal', s=20, alpha=0.9, zorder=3)
        ax_pi_base.text(0.01, 0.85, "PI Index", transform=ax_pi_base.transAxes, color='teal', fontweight='bold', va='top')
        ax_pi_base.set_ylabel("PI Value")
        ax_pi_base.grid(True, linestyle='--', alpha=0.3)
        ax_pi_base.set_ylim(0.0, 2.0) 
        #baseline
        ax_pi_base.axhline(y=.58, color='blue', linestyle=':', alpha=0.3, label="PI baseline")
        # Delta PI
        ax_pi_delta.scatter(df_combo['Time'], df_combo['PI_Delta'], color='blue', s=20, alpha=0.9, zorder=3)
        ax_pi_delta.text(0.01, 0.85, "Delta PI Index", transform=ax_pi_delta.transAxes, color='blue', fontweight='bold', va='top')
        ax_pi_delta.set_ylabel("Change")
        ax_pi_delta.grid(True, linestyle='--', alpha=0.3)
        ax_pi_delta.set_ylim(-100, 100)
        ax_pi_delta.set_xlabel("Time (seconds)")
        ax_pi_delta.axhline(y=51, color='red', linestyle=':', alpha=0.3, label="Delta PI Trigger")

    # Apply Zoom
    plt.xlim(START_TIME, END_TIME)
    plt.tight_layout()
    plt.subplots_adjust(hspace=.1)
    
    print("Displaying plot...")
    plt.show()

if __name__ == "__main__":
    main()