CXX=llvm-g++
COMPILE=-c -std=c++17 \
-I/usr/local/Cellar/opencv/4.4.0_2/include/opencv4 \
-I/usr/local/Cellar/glm/0.9.9.8/include \
-I/Users/YJ-work/cpp/shadow2d/header
LINK=-L/usr/local/Cellar/opencv/4.4.0_2/lib -lopencv_core -lopencv_highgui -lopencv_imgcodecs \
-lopencv_imgproc
SRC_DIR=/Users/YJ-work/cpp/shadow2d/src

all: main test

main: main.o
	$(CXX) $(LINK) $^ -o $@

main.o: $(SRC_DIR)/main.cpp
	$(CXX) $(COMPILE) $^ -o $@

test: test.o
	$(CXX) $(LINK) $^ -o $@

test.o: $(SRC_DIR)/test.cpp
	$(CXX) $(COMPILE) $^ -o $@

.PHONY: cleanObj cleanImg

cleanObj:
	rm -f *.o

cleanImg:
	rm -f ./result/*
