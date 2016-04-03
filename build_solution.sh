g++ -O2 -o solution main.cpp \
    c24/build/*.o -I. -std=c++11 -stdlib=libc++ -W -Wall \
    -lglog -lgflags \
    -lsfml-system -lsfml-network -lsfml-window -lsfml-audio -lsfml-graphics;
