CXX      = g++
CXXC      = clang
OBJECTS = Main.o Logger.o Directory.o Controller.o FileFilter.o
BOOST = -lboost_system -lboost_filesystem
LOGGEROBJECTS = LoggerTest.o Logger.o
CXXFLAGS  = -c -std=c++2a
DEBUGFLAGS  = -c -g -std=c++17

SRCDIR := src
OBJDIR := obj
BINDIR := bin

Release: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) 
	
ValueTest: $(VALUEOBJECTS)
	$(CXX) -o $@ $(VALUEOBJECTS) 

LoggerTest: $(LOGGEROBJECTS)
	$(CXX) -o $@ $(LOGGEROBJECTS) 

ClangValueTest: $(VALUEOBJECTS)
	$(CXXC) -o $@ $(VALUEOBJECTS) 

Main.o: Main.cpp
	$(CXX) $(CXXFLAGS) $*.cpp
Controller.o: Controller.cpp Controller.h
	$(CXX) $(CXXFLAGS) $*.cpp
FileFilter.o: FileFilter.h
	$(CXX) $(CXXFLAGS) $*.cpp
Directory.o: Directory.h
	$(CXX) $(CXXFLAGS) $*.cpp
Logger.o: Logger.cpp Logger.h
	$(CXX) $(CXXFLAGS) $*.cpp
CSVReader.o: CSVReader.cpp CSVReader.h
	$(CXX) $(CXXFLAGS) $*.cpp
CSVObject.o: CSVObject.cpp CSVObject.h
	$(CXX) $(CXXFLAGS) $*.cpp

Debug: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(BOOST)

%.o: %.cpp %.h
	$(CXX) $(DEBUGFLAGS) $<

clean:
	rm *.o test




