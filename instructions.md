### check dotenv : `apt-cache search dotenv-cpp` install `sudo apt install libdotenv-cpp-dev`

Finding any package/lib/header installed 

Find system include directories using gcc : gcc -v -E -x c++ -

serch any lib in the package manager's database : apt list --installed | grep dotenv

manual serch: sudo find / -name dotenv

Finding status of port:  sudo lsof -i :7000
Kill process : sudo kill -9  pid
Check listning port: sudo netstat -tuln | grep LISTEN
