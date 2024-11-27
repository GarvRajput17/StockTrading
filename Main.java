public class Main {
    public static void main(String[] args) {
        Helper stockTradingHelper = new Helper();
        stockTradingHelper.run();
    }
}

// Main.java initiates the program.
// Helper.java declares the native methods.
// Helper.cpp implements those native methods using JNI
// The c++ functions are then called. 

//The key components:
    //Java side declares methods with 'native' keyword
    //JNI generates Helper.h with function signatures
    //Helper.cpp implements these functions
    //C++ business logic executes when called