def AND(a, b):
    if (a == 1.0 and b == 1.0):
        return 1.0
    return 0.0

def NAND(a, b):
    if (a == 0.0 or b == 0.0):
        return 1.0
    return 0.0

def OR(a, b):
    if (a == 1.0 or b == 1.0):
        return 1.0
    return 0.0

def NOR(a, b):
    if (a == 0.0 and b == 0.0):
        return 1.0
    return 0.0

def ANDI(a, b):
    if (a == 1.0 and b == 0.0):
        return 1.0
    return 0.0

def XOR(a, b):
    if ((a == 1.0 and b == 0.0) or (a == 0.0 and b == 1.0)):
        return 1.0
    return 0.0

def OUT(a):
    return a

#INPUTS
#OUTPUTS
#ROWS
#TARGETFILE

dataString = open(target_file, "r").read()

dataRows = dataString.split("\n")

data = []
for i in range(inputs + outputs):
    data.append([])

for i in range(rows):
    row = dataRows[i].split(",")
    
    for j in range(inputs + outputs):
        data[j].append(float(row[j].strip()))


errors = 0
for r in range(rows):
    i = []
    eO = []
    for j in range(inputs):
        i.append(data[j][r])
    for j in range(outputs):
        eO.append(data[j + inputs][r])
    o = []
    
    #APPENDOUTPUTS
    for j in range(outputs):
        if eO[j] != o[j]:
            errors = errors + 1

