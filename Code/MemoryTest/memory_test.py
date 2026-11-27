# Libraries needed
import time
import random
import os

# Print out what is being tested
print("STARTING TEST")
print("1.Temperature")
print("2.Temperature")
print("3.Humidity")
print("4.Sea altitude")
print("5.Ground altitude")
#print("6.Radiation")

seconds = 75 # how long is the satellite in the air
step = 0.5 # how many times per second to send info

data = ""
packets = 75 / step # calculate packet count
while packets > 0:
    # Generate random values for each thing we are measuring
    temperature = random.uniform(-40, 85)
    pressure = random.uniform(300, 1100)
    humidity = random.uniform(0, 100)
    seaAltitude = random.uniform(-500, 9000)
    groundAltitude = random.uniform(-500, 9000)
    #radiation = random.uniform(0, 100)

    # append to the data string
    data += f"{temperature:.2f} {pressure:.2f} {humidity:.2f} {seaAltitude:.2f} {groundAltitude:.2f} \n"
    
    packets -= 1

# dump it all into a file
with open("./Code/MemoryTest/data_v1.txt", "w") as f:
    f.write(data)
