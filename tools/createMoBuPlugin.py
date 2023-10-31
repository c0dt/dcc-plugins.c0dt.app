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

def createMoBuNativePlugin(type, pluginName):
    source_dir = Path(os.getcwd()) / 'Samples' / f'{type}s' / f'{type}_template'
    target_dir = Path(os.getcwd()) / 'Develop' / f'{type}s' / f'{pluginName}'
    copy_directory(source_dir, target_dir)
    files = [f for f in os.listdir(target_dir) if os.path.isfile(os.path.join(target_dir, f))]
    for file in files:
        file_path = os.path.join(target_dir, file)
        dst_path = os.path.join(target_dir, file.replace(f'or{type}_template', pluginName))
        replace_string_in_file(file_path, f'or{type}_template', pluginName)
        replace_string_in_file(file_path, f'OR{type[0].upper() + type[1:]}_Template',  pluginName[0].upper() + pluginName[1:])
        replace_string_in_file(file_path, f'or{type}_template'.upper(), pluginName.upper())
        replace_string_in_file(file_path, f'or{type}template'.upper(), pluginName.upper())
        os.rename(file_path, dst_path)
    # pluginName = pluginName[0].upper() + pluginName[1:]
    # source = target_dir / 'c0dtCmdTemplate.cpp'
    # destination = target_dir / f'c0dt{pluginName}.cpp'
    # source.rename(destination)
    # replace_string_in_file(destination, '{{PLUGIN_NAME}}', pluginName)
    # destination = target_dir / 'CMakeLists.txt'
    # replace_string_in_file(destination, '{{PLUGIN_NAME}}', pluginName)
    # execute_command_in_dir('cmake . -B.', target_dir)
    # open_with_default_program(target_dir / 'Project.sln')

def main():
    lenOfArgv = len(sys.argv)
    if lenOfArgv != 3:
        print("Incorrect arguments. Usage: python Tools/createMoBuPlugin.py plugin_type plugin_name")
        return
    pluginType = sys.argv[1]
    pluginName = sys.argv[2]
    createMoBuNativePlugin(pluginType, pluginName)

if __name__ == '__main__':
    main()