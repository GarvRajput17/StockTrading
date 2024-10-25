#include <iostream>
#include <string>
#include <jni.h>
#include<bits/stdc++.h>

using namespace std;

class StockInfo {
private:
    string stockSymbol;
    string stockPrice;
    string stockGain;
    JavaVM *jvm;
    JNIEnv *env;

public:
    StockInfo() {
        // Initialize JVM
        JavaVMInitArgs vm_args;
        JavaVMOption options[1];
        options[0].optionString = const_cast<char*>("-Djava.class.path=.:lib/json-20240303.jar");
        
        vm_args.version = JNI_VERSION_1_8;
        vm_args.nOptions = 1;
        vm_args.options = options;
        vm_args.ignoreUnrecognized = false;
        
        JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
    }

    ~StockInfo() {
        jvm->DestroyJavaVM();
    }

    void setStockSymbol(const string& symbol) {
        stockSymbol = symbol;
    }

    string getStockSymbol() const {
        return stockSymbol;
    }

    void fetchStockData() {
        // Find the Java class
        jclass stockClass = env->FindClass("AlphaVantageAPI");
        if (stockClass == nullptr) {
            cout << "Failed to find Java class" << endl;
            return;
        }

        // Get the method ID for main
        jmethodID mainMethod = env->GetStaticMethodID(stockClass, "main", "([Ljava/lang/String;)V");
        if (mainMethod == nullptr) {
            cout << "Failed to find main method" << endl;
            return;
        }

        // Create the string array argument
        jobjectArray args = env->NewObjectArray(1, 
            env->FindClass("java/lang/String"), 
            env->NewStringUTF(stockSymbol.c_str()));

        // Call the Java method
        env->CallStaticVoidMethod(stockClass, mainMethod, args);

        // Read the output file
        ifstream infile("output.txt");
        if (infile.is_open()) {
            getline(infile, stockPrice);
            getline(infile, stockGain);
            infile.close();
        }
    }

    void displayStockData() const {
        cout << "Stock Symbol: " << stockSymbol << endl;
        cout << "Current Price: " << stockPrice << endl;
        cout << "Today's Gain: " << stockGain << endl;
    }
};

int main() {
    StockInfo stock;
    string symbol;

    cout << "Enter the stock symbol: ";
    cin >> symbol;

    stock.setStockSymbol(symbol);
    stock.fetchStockData();
    stock.displayStockData();

    return 0;
}
