zgui_packer_version='1.0.0'

echo -e '
                        _ 
                       (_)
  ____   __ _   _   _   _ 
 |_  /  / _` | | | | | | |
  / /  | (_| | | |_| | | |              
 /___|  \__, |  \__,_| |_|
         __/ |            
        |___/   packer v'$zgui_packer_version'       
'

echo -e 'Welcome to zgui_packer'

PS3='=> '

read -p "Are you sure you want to pack zgui source to one header library? (Y/N) " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]
then
mkdir "./dist"
cp "./src/zgui.cc" "./dist/zgui.cc"
cp "./src/zgui.hh" "./dist/zgui.hh"
fi