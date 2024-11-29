#CXX = g++-14
#CXXFLAGS = -std=c++17 -stdlib=libstdc++ -I. -I/opt/homebrew/Cellar/openssl@3/3.4.0/include -I/opt/homebrew/Cellar/gcc/14.2.0/include/c++/14 -I/opt/homebrew/Cellar/nlohmann-json/3.11.3/include
#LDFLAGS = -L/opt/homebrew/Cellar/openssl@3/3.4.0/lib -lssl -lcrypto -lcurl

#TARGET = program
#SOURCES = main1.cpp user.cpp Stock.cpp authentication/auth.cpp utils/utils.cpp Transaction.cpp portfolio.cpp
#OBJECTS = $(SOURCES:.cpp=.o)

#$(TARGET): $(OBJECTS)
#	$(CXX) $(OBJECTS) $(LDFLAGS) -o $(TARGET)

#%.o: %.cpp
#	$(CXX) $(CXXFLAGS) -c $< -o $@

#clean:
#	rm -f $(TARGET) $(OBJECTS)

CXX = g++-14
CXXFLAGS = -std=c++17 -stdlib=libstdc++ -I. -I/opt/homebrew/Cellar/openssl@3/3.4.0/include -I/opt/homebrew/Cellar/gcc/14.2.0/include/c++/14 -I/opt/homebrew/Cellar/nlohmann-json/3.11.3/include -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/darwin -fPIC

LDFLAGS = -L/opt/homebrew/Cellar/openssl@3/3.4.0/lib -lssl -lcrypto -lcurl -shared

JAVAC = javac
JAVA = java

# C++ sources with updated paths
CPP_SOURCES = JNI/Helper.cpp \
              Backend_Classes/Class_files/user.cpp \
              Backend_Classes/Class_files/Stock.cpp \
              authentication/auth.cpp \
              utils/utils.cpp \
              Backend_Classes/Class_files/Transaction.cpp \
              Backend_Classes/Class_files/portfolio.cpp

CPP_OBJECTS = $(CPP_SOURCES:.cpp=.o)

# Java sources with updated paths
JAVA_SOURCES = Main.java JNI/Helper.java
JAVA_CLASSES = $(JAVA_SOURCES:.java=.class)

# Target shared library
LIB_TARGET = libstocktrading.dylib

# Final executable
all: $(LIB_TARGET) $(JAVA_CLASSES)
	@echo "Build complete. Run with: make run"

# Compile Java files and generate JNI header
$(JAVA_CLASSES): $(JAVA_SOURCES)
	$(JAVAC) $(JAVA_SOURCES)
	$(JAVAC) -h . JNI/Helper.java

# Create shared library
$(LIB_TARGET): $(CPP_OBJECTS)
	$(CXX) $(CPP_OBJECTS) $(LDFLAGS) -o $@

# Compile C++ files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the program
run: all
	export DYLD_LIBRARY_PATH=.:$$DYLD_LIBRARY_PATH && $(JAVA) Main

clean:
	rm -f $(CPP_OBJECTS) $(JAVA_CLASSES) $(LIB_TARGET) *.class

.PHONY: all clean run

