g++ main.cpp -lncurses  -o output `pkg-config --cflags --libs opencv4`
./output
rm output
# for more recourses watch build.sh
