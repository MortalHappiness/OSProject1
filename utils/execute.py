# Execute the program from input folder and output corresponding files
# to output folder

import os
import sys
import time

# ========================================

def help_message():
    print("Usage: python execute.py <program_path> <input_folder> <output_folder>")

def main(script, program_path, input_folder, output_folder):
    for input_file in os.listdir(input_folder):
        print("="*40)
        cmd = "sudo dmesg --clear"
        print(cmd)
        os.system(cmd)

        basename, extension = os.path.splitext(input_file)
        cmd = "sudo "  + program_path + \
              " < " + os.path.join(input_folder, input_file) + \
              " > " + os.path.join(output_folder, basename + "_stdout.txt")
        print(cmd)
        os.system(cmd)

        time.sleep(2) # sleep to wait for dmesg log
        cmd = "dmesg --notime | grep Project1 > " + \
              os.path.join(output_folder, basename + "_dmesg.txt")
        print(cmd)
        os.system(cmd)

# ========================================

if __name__ == '__main__':
    main(*sys.argv)

