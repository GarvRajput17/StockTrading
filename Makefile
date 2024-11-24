CXX = g++-14
CXXFLAGS = -std=c++17 -stdlib=libstdc++ -I. -I/opt/homebrew/Cellar/openssl@3/3.4.0/include -I/opt/homebrew/Cellar/gcc/14.2.0/include/c++/14 -I/opt/homebrew/Cellar/nlohmann-json/3.11.3/include
LDFLAGS = -L/opt/homebrew/Cellar/openssl@3/3.4.0/lib -lssl -lcrypto -lcurl

TARGET = program
SOURCES = main1.cpp user.cpp Stock.cpp authentication/auth.cpp utils/utils.cpp Transaction.cpp
OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)
