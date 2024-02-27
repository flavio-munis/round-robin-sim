import sys
import json
import pandas as pd
import matplotlib.pyplot as plt

def main(path):

    # Opens output file and parses it
    with open(path, 'r', encoding='utf-8') as fp:
        dataJ = json.load(fp)

    # Deep Parse json and create a DataFrame struct
    dataNormalized = pd.json_normalize(dataJ['events'],
                                       record_path=['processes'],
                                       meta=['time_of_execution'])
    
    # Number os rows in json
    setSize = dataNormalized.shape[0] - 1
    
    # Filters cpu info to plot graph and creates a DataFrame Object
    cpu = getDataStats(dataNormalized, True)
    cpudf = pd.DataFrame(data={'pid': cpu[0],
                                'Start': cpu[1],
                                'Quantum': cpu[2]})


    # Create plot struct
    fig, axis = plt.subplots(2, 1, layout='constrained')

    # Define CPU graph info 
    axis[0].barh(y = cpudf["pid"], 
                 left=cpudf["Start"], 
                 width=cpudf["Quantum"])
    #axis[0].set_xlabel("Quantum", fontsize=15, fontweight="bold")
    axis[0].set_ylabel("PID", fontsize=15, fontweight="bold")
    axis[0].set_title("CPU Usage", fontsize=20, fontweight="bold", pad=10)
    axis[0].set_xlim(0, dataNormalized.loc[setSize].at["time_of_execution"])

    # Filters disk info to plot graph and creates a DataFrame Object
    disk = getDataStats(dataNormalized, False)
    diskdf = pd.DataFrame(data={'pid': disk[0],
                                'Start': disk[1],
                                'IO Time': disk[2]})

    # Sort Data Frame by PID
    diskdf.sort_values(by=['pid'],
                       inplace=True)

    # Define Disk graph info 
    axis[1].barh(y = diskdf["pid"], 
                 left=diskdf["Start"], 
                 width=diskdf["IO Time"])
    #axis[1].set_xlabel("Quantum", fontsize=15, fontweight="bold")
    axis[1].set_ylabel("PID", fontsize=15, fontweight="bold")
    axis[1].set_title("Disk Usage", fontsize=20, fontweight="bold", pad=10)
    axis[1].set_xlim(0, dataNormalized.loc[setSize].at["time_of_execution"])

    # Plot Graphs
    plt.show()


def getDataStats(dFrame, cpu):
    
    pid = []
    start = []
    timeExec = []
    isStatus = False

    for i in range(0, dFrame.shape[0]):
        dFrameLoc = dFrame.loc[i]

        if cpu:
            if dFrameLoc.at["status"] == "running":
                isStatus = True;
        else:
            if dFrameLoc.at["status"] == "running_io":
                isStatus = True;

        if isStatus:
            pidTemp = dFrameLoc.at["pid"]
            startTemp = dFrameLoc.at["time_of_execution"]
            pid.append("Process " + str(pidTemp))
            start.append(startTemp)

            for j in range(i + 1, dFrame.shape[0]):
                if dFrame.loc[j].at["pid"] == pidTemp:
                    finishTemp = dFrame.loc[j].at["time_of_execution"]
                    timeExec.append(finishTemp - startTemp)
                    break;
    
            isStatus = False

    return [pid, start, timeExec]


if __name__ == "__main__":
    if len(sys.argv) == 2:
        main(sys.argv[1])
    else:
        print("Usage: python [program-name] [path-to-json]")
