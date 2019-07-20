import glob
import os
import re
import shutil

def yes_no(question_to_be_answered):
    while True:
        choice = input(question_to_be_answered).lower()
        if choice[:1] == 'y': 
            return True
        elif choice[:1] == 'n':
            return False
        else:
            print("Please respond with 'Y' for yes or 'N' for no.\n")

print("""\n
                        _ 
                       (_)
  ____   __ _   _   _   _ 
 |_  /  / _` | | | | | | |
  / /  | (_| | | |_| | | |              
 /___|  \__, |  \__,_| |_|
         __/ |            
        |___/    zgui_packer 
              by danielkrupinski 
                 and zxvnme       
""")

generated_file_comment = """
// #===================================================================================#
// +                                                                                   +
// +  _   _   ____  ________ _____                                        (_)          +
// + | \ | | / __ \|__   __||  ____|  DO NOT edit    ____   __ _   _   _   _           +
// + |  \| || |  | |  | |   | |__    this file, it  |_  /  / _` | | | | | | |          +
// + | . ` || |  | |  | |   |  __|   was created by: / /  | (_| | | |_| | | |          +
// + | |\  || |__| |  | |   | |____                 /___|  \__, |  \__,_| |_|          +
// + |_| \_| \____/   |_|   |______|                        __/ |                      +
// +                                                       |___/    zgui_packer        +
// +                                                             by danielkrupinski    + 
// +   (https://github.com/zxvnme/zgui/blob/master/packer.py)       and zxvnme         +      
// #===================================================================================#
"""

ignored_files = ['dllmain.cc']
output_contents = []

proceed = yes_no("  Are you sure you want to pack zgui source to one header library? (Y/N) ")
if proceed:
  output_contents.append(generated_file_comment)
  output_contents.append("#include \"./zgui.hh\"")
  output_contents.append("using namespace zgui::globals;")
  output_contents.append("zgui::functions_t zgui::functions;")

  for filename in glob.iglob('src/**/*.cc', recursive=True):
      if os.path.split(filename)[1] not in ignored_files:
          with open(filename, 'r') as file:
              output_contents.append(re.sub(r'(?s)^//@zgui_packer:ignore.*//@zgui_packer:resume$', '', file.read(), flags=re.M))

  if not os.path.exists('dist'):
      os.mkdir('dist')

  shutil.copy('src/zgui.hh', 'dist/zgui.hh')
  with open('dist/zgui.cc', 'w') as output:
      output.write('\n'.join(output_contents))

  print("\nDone! See /dist folder for output.\n")
else:
  quit()
