# Libraries needed
import time
import random
import os

# Print out what is being tested
print("STARTING TEST")
print("TESTING WITH VARIABLES:")
print("1.AIR TEMPERATURE")
print("2.AIR PRESSURE")
print("3.GAMME RADIATION\n")

SECONDS = 75 # how long will the test run
STEP = 0.5 # in seconds

# Countdown function, which simply sleeps for STEP seconds and then dumps random values into the files, which mimics the data the CanSat would actually collect
def countdown(seconds):
    while seconds > 0:
        time.sleep(STEP)
        seconds -= STEP
        print(f"{seconds} LEFT")

        with open("temperature.txt", "+a") as f:
            f.write(f"{random.randrange(-20, 50)}K\n")
        
        with open("pressure.txt", "+a") as f:
            f.write(f"{random.randrange(70000, 150000)}Pa\n")
        
        with open("radiation.txt", "+a") as f:
            f.write(f"{random.random()}Sv\n")

    print("\n")

countdown(SECONDS) # Call the function

# Get the individual file sized and then add them up together
temp_size = os.path.getsize("temperature.txt")
pressure_size = os.path.getsize("pressure.txt")
radiation_size = os.path.getsize("radiation.txt")
TOTAL_SIZE = temp_size + pressure_size + radiation_size

print(f"TOTAL FILE SIZE: {TOTAL_SIZE}") # Print out the total size of the files

# Dump the results into a new file
with open("sizes.txt", "w") as f:
    f.write(f"Temperature size: {temp_size}b;\nPressure size: {pressure_size}b;\nRadiation size: {radiation_size}b;\nTOTAL SIZE: {TOTAL_SIZE}b");