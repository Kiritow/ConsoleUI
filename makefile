CXXFLAGS = -std=c++14 -Wall -O2

lib:
	$(CXX) $(CXXFLAGS) CUI.cpp -o CUI.o
	$(CXX) $(CXXFLAGS) ColorUI.cpp -o ColorUI.o
test:
	$(CXX) $(CXXFLAGS) example.cpp -o test.exe
	test.exe
clean: 
	rm -f CUI.o ColorUI.o test.exe