CXXFLAGS = -Wall -O2 -std=c++14 

lib:
	mkdir obj
	$(CXX) $(CXXFLAGS) -c ConsoleUI/CUI.cpp -o obj/CUI.o
	$(CXX) $(CXXFLAGS) -c ConsoleUI/ColorUI.cpp -o obj/ColorUI.o
test: lib
	$(CXX) $(CXXFLAGS) -c example.cpp -o obj/example.o
	mkdir bin
	$(CXX) $(CXXFLAGS) -o bin/test.exe obj/CUI.o obj/ColorUI.o obj/example.o -s
clean: 
	rm -rf obj
	rm -rf bin