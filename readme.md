STL Evaluator
=============

This project uses [StlEval](https://gitlab.com/abakhirkin/StlEval).
Appended with a Python.

Follow build.md to create the build files.  

See Stle-cli/Usage.txt for directions on how to use StlEval

DATA
====
Data was extracted from VitalDB.com
3 cases are kept in the 'data' folder. Additional cases should be added here.
Cases must have (Time, Signal) format.
All cases must start with (0,0) for StlEval to work.

extract.py will:
    1. Extract VitalDB SNU cases with 100hz ABP signals.
        (Note: If extraction does not complete, the program will just start where it left off.)
        (Note: The user can also choose which case the program will start extracting data.) 
    2. Format data to time and blood pressure CSV files.

HYPO PRED EXECUTION
===================
run.py will: 
    1. Find the heartrate.
    2. Scale the STP formula windows and generate the STP files for theta1, theta2 and theta3 (satisfaction) as well as theta1_val, theta2_val and theta3_val (values of satisfaction).
        (Note: where theta1 = diastolic, theta2 = systolic and theta3 = dicrotic notch.)
    3. Run the STP files on StlEval and create the .csv files, 
    4. Compute the MAP.
    5. Compute the AVG MAP for each 20 heartbeat segment.
    6. Compute trigger warnings when MAP AVG trajectory will go below 65mmHg in the next 60 seconds.
    7. Compute the baseline DP and PI values based the first 100 heartbeat average.
        (Note: There is a place for hardcoded baselines for testing purposes that has been commented out.) 
    8. Compute the DP and PI for each heartbeat if possible. 
    9. Compute the delta DP and delta PI from the baseline. 
    10. Create a combined csv file (case_xxxx_snu_combine.csv) which is used to plotting. 
        

PLOTTING
========
plot1.py will show the MAP, Avg MAP, and TTH < min warnings within a plot window.

plot2.py will show the diastolic, systolic and dicrotic notch within a plot window.

plot3.py will show the MAP, Avg MAP, and TTH < min warnings, the Perfusion Inded, Delta PI from baseline, Relative dicrotic Notch Height and Delta DP within a plot window.

    (Note: For all plots, START_TIME and END_TIME can be changed to resize the plot window.)
