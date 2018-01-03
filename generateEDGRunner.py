shell_file = "/home/atkrye/Documents/EDG/runEDGShell.py"
edg_file = "/home/atkrye/Documents/EDG/1BitAdder.output"
target_file = "/home/atkrye/Documents/EDG/1BitAdder.csv"
output_file = "/home/atkrye/Documents/EDG/1BitAdder.py"
inputs = 3
outputs = 2
rows = 8
shell = open(shell_file, "r").read()
shell = shell.replace("#INPUTS", "#INPUTS\ninputs = " + str(inputs))
shell = shell.replace("#OUTPUTS", "#OUTPUTS\noutputs = " + str(outputs))
shell = shell.replace("#ROWS", "#ROWS\nrows = " + str(rows))
shell = shell.replace("#TARGETFILE", "#TARGETFILE\ntarget_file = \"" + target_file + "\"")

edg = open(edg_file, "r").read()
edg_rows = edg.split("~\n")


o = 2
o_string = "#APPENDOUTPUTS\n"
while o < 2 + outputs:
    output = edg_rows[o]
    output_eqn = output.split(":")[1].strip()
    for i in range(inputs):
        output_eqn = output_eqn.replace("i" + str(i), "i[" + str(i) + "]")
    o = o + 1
    o_string = o_string + "    o.append(" + output_eqn + ")\n"
shell = shell.replace("#APPENDOUTPUTS", o_string)
open(output_file, "w").write(shell)
