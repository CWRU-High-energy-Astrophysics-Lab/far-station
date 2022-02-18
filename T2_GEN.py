import os
import random
import time
##fake t2
path = "T2_list.out"
if os.path.exists(path):
    os.remove(path)
f = open(path, "w")

while True:
    currentsec = time.time_ns().__str__()[0:10]
    f.write("Sec,nt2,scaler: " + currentsec + " ## " + "Scaler\n")
    i = 0
    while currentsec == time.time_ns().__str__()[0:10]:
        if random.randint(0, 1000000) < 17:
            f.write(i.__str__() + " 1:" + time.time_ns().__str__()[10:] + "\n")
            i += 1
    f.write("---\n")
