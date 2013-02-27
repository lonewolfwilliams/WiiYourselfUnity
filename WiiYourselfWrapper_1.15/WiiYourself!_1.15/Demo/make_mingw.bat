g++ -Wall -g0 -O2 -c Demo.cpp -o Demo.o
g++ -o Demo.exe Demo.o -s -L. -lwiimote -lwinmm -lhid -lsetupapi

@rem g++ -o Demo.exe Demo.o wiimote.dll -lwinmm ; to create Demo.exe that uses wiimote.dll