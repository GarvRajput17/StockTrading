CXX = g++-14
CXXFLAGS = -I/opt/homebrew/Cellar/openssl@3/3.4.0/include -I/opt/homebrew/Cellar/gcc/14.2.0/include/c++/14
LDFLAGS = -L/opt/homebrew/Cellar/openssl@3/3.4.0/lib -lssl -lcrypto -lcurl

TARGET = program
SOURCE = main1.cpp

$(TARGET): $(SOURCE)
	$(CXX) $(SOURCE) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)
