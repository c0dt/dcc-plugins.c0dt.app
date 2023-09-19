import subprocess
import sys
import os
from pathlib import Path
import shutil

def copy_directory(src, dst):
    if not dst.exists():
        shutil.copytree(src, dst)
    else:
        print('already exist, stop')

def replace_string_in_file(filename, old_string, new_string):
    with open(filename, 'r') as file:
        file_contents = file.read()
    file_contents = file_contents.replace(old_string, new_string)
    with open(filename, 'w') as file:
        file.write(file_contents)

def execute_command_in_dir(command, dir):
    result = subprocess.run(command, cwd=dir, shell=True, capture_output=True, text=True)
    print("Return code:", result.returncode)
    print("Have {} bytes in stdout:\n{}".format(len(result.stdout), result.stdout))
    print("Have {} bytes in stderr:\n{}".format(len(result.stderr), result.stderr))

def open_with_default_program(filepath):
    if sys.platform.startswith('win32'):
        os.startfile(filepath)
    elif sys.platform.startswith('darwin'):
        subprocess.run(['open', filepath])
    elif sys.platform.startswith('linux'):
        subprocess.run(['xdg-open', filepath])
    else:
        raise OSError("Platform not supported")

def createMayaNativePlugin(pluginName):
    source_dir = Path(os.getcwd()) / 'maya' / 'native' / 'template'
    target_dir = Path(os.getcwd()) / 'maya' / 'native' / pluginName
    copy_directory(source_dir, target_dir)
    pluginName = pluginName[0].upper() + pluginName[1:]
    source = target_dir / 'c0dtCmdTemplate.cpp'
    destination = target_dir / f'c0dt{pluginName}.cpp'
    source.rename(destination)
    replace_string_in_file(destination, '{{PLUGIN_NAME}}', pluginName)
    destination = target_dir / 'CMakeLists.txt'
    replace_string_in_file(destination, '{{PLUGIN_NAME}}', pluginName)
    execute_command_in_dir('cmake . -B.', target_dir)
    open_with_default_program(target_dir / 'Project.sln')

def main():
    lenOfArgv = len(sys.argv)
    if lenOfArgv != 2:
        print("Incorrect arguments. Usage: python main.py plugin_name")
        return
    pluginName = sys.argv[1]
    createMayaNativePlugin(pluginName)

if __name__ == '__main__':
    main()