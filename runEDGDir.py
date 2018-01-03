import glob
import sys
this_shell_file = "runEDGShell.py"
this_inputs = int(sys.argv[1])
this_outputs = int(sys.argv[2])
this_rows = int(sys.argv[3])
this_target_file = sys.argv[4]
this_edg_dir = sys.argv[5]
shell = open(this_shell_file, "r").read()
shell = shell.replace("#INPUTS", "#INPUTS\ninputs = " + str(this_inputs))
shell = shell.replace("#OUTPUTS", "#OUTPUTS\noutputs = " + str(this_outputs))
shell = shell.replace("#ROWS", "#ROWS\nrows = " + str(this_rows))
shell = shell.replace("#TARGETFILE", "#TARGETFILE\ntarget_file = \"" + this_target_file + "\"")
files = glob.glob(this_edg_dir + "/*.output")
print(this_edg_dir + "/*.output contains:")
print(files)
total_errors = 0
failures = []
for edg_f in files:
    print("Checking " + edg_f)
    edg = open(edg_f, "r").read()
    edg_rows = edg.split("~\n")


    o = 2
    o_string = "#APPENDOUTPUTS\n"
    while o < 2 + this_outputs:
        output = edg_rows[o]
        output_eqn = output.split(":")[1].strip()
        for i in range(this_inputs):
            output_eqn = output_eqn.replace("i" + str(i), "i[" + str(i) + "]")
        o = o + 1
        o_string = o_string + "    o.append(" + output_eqn + ")\n"
    shell_copy = shell.replace("#APPENDOUTPUTS", o_string)
    exec(shell_copy)
    print("Errors:" + str(errors))
    total_errors = total_errors + errors
    if errors > 0:
        failures.append(edg_f)

print("Total errors: " + str(total_errors))
if total_errors > 0:
    print("Erroneous files: ")
    print(failures)
    
    
