#include <iostream>                               // Include standard input-output stream library
#include <fstream>                                // Include file stream library for file operations
#include <vector>                                 // Include vector library for dynamic arrays
#include <unordered_map>                          // Include unordered_map for hash table implementation
#include <algorithm>                              // Include algorithm library for sorting
#include "Stock.hpp"                              // Include the header file for the OwnedStock class
#include <nlohmann/json.hpp>                     // Include the JSON library for parsing JSON data

using json = nlohmann::json;                     // Create an alias for nlohmann::json

class Portfolio {                                // Define the Portfolio class
private:
    int numberOfStocks;                          // Number of unique stocks in the portfolio
    double totalPortfolioInvestment;             // Total amount invested in the portfolio
    double currentValue;                         // Current value of the portfolio
    double returns;                              // Overall returns of the portfolio
    vector<OwnedStock> holdings;                 // Vector to hold OwnedStock objects
    unordered_map<string, OwnedStock> holdingsMap; // Map to associate stock IDs with OwnedStock objects
    const string fileName = "test.json";        // File name for saving/loading portfolio data
    string userID;                               // User ID associated with the portfolio

public:
    // Constructor to initialize the Portfolio object
    Portfolio(int numberOfStocks, double totalInvested, double currentValue, 
             double returns, vector<OwnedStock> holdings, string userID)
        : numberOfStocks(numberOfStocks),
          totalPortfolioInvestment(totalInvested),
          currentValue(currentValue),
          returns(returns),
          holdings(holdings),
          userID(userID) {}

    // Function to display the portfolio summary
    void displayPortfolio() {
        int uniqueStocks = 0;                     // Counter for unique stocks
        double totalInvested = 0.0;               // Total amount invested in stocks
        double totalProfit = 0.0;                  // Total profit/loss from stocks
        double totalReturns = 0.0;                 // Overall returns percentage
        double currentPortfolioValue = 0.0;        // Current value of the portfolio

        cout << "\n=== Portfolio Summary ===\n";      // Portfolio summary header
        cout << "----------------------------------------\n";

        // Iterate through each stock in holdings
        for (const auto& stock : holdings) {
            uniqueStocks++;                         // Increment unique stock counter
            double investment = stock.getBuyPrice() * stock.getQuantity(); // Calculate investment for the stock
            double currentStockValue = stock.getPrice() * stock.getQuantity(); // Calculate current value of the stock
            
            totalInvested += investment;            // Accumulate total invested amount
            totalProfit += (currentStockValue - investment); // Calculate total profit
            currentPortfolioValue += currentStockValue; // Accumulate current portfolio value

            stock.printStockInfo();                 // Print stock information
        }

        // Calculate overall returns percentage
        totalReturns = (totalInvested > 0) ? (totalProfit / totalInvested) * 100 : 0;

        // Display overall portfolio metrics
        cout << "\n=== Overall Portfolio Metrics ===\n";
        cout << "Number of Unique Stocks: " << uniqueStocks << "\n";
        cout << "Total Investment: ₹" << fixed << setprecision(2) << totalInvested << "\n";
        cout << "Current Portfolio Value: ₹" << currentPortfolioValue << "\n";
        cout << "Total Profit/Loss: ₹" << totalProfit << "\n";
        cout << "Overall Returns: " << totalReturns << "%\n";
        cout << "----------------------------------------\n";
    }

    // Function to load portfolio data from a JSON file
    void loadPortfolio() {
        ifstream inFile(fileName);                  // Open the file for reading
        if (!inFile.is_open()) {                    // Check if the file opened successfully
            throw runtime_error("Unable to open " + fileName); // Throw an error if not
        }

        json data;                                   // Create a JSON object to hold data
        inFile >> data;                              // Read data from the file
        inFile.close();                              // Close the file

        // Check if userID and portfolio data exist
        if (!data.contains(userID) || !data[userID].contains("portfolio")) {
            return;                                  // Exit if no portfolio data found
        }

        json portfolioData = data[userID]["portfolio"]; // Get the portfolio data
        
        // Iterate through each stock in the portfolio
        for (const auto& [stockID, stockData] : portfolioData.items()) {
            if (stockID == "OwnedStockId") continue; // Skip if stockID is "OwnedStockId"

            // Extract stock data from JSON
            int quantity = stockData["Quantity"].get<int >(); // Get the quantity of the stock
            double averagePrice = stockData["AveragePrice"].get<double>(); // Get the average price of the stock
            double currentPrice = stockData["CurrentPrice"].get<double>(); // Get the current price of the stock

            OwnedStock stock(
                stockID,                    // stockID
                stockID,                    // using stockID as name temporarily
                currentPrice,               // currentPrice
                userID,                     // userID
                quantity,                   // quantity
                averagePrice                // buyPrice
            );

            holdings.push_back(stock);              // Add the stock to holdings
            holdingsMap[stockID] = stock;           // Map the stockID to the OwnedStock object
            
            numberOfStocks++;                       // Increment the number of stocks
            totalPortfolioInvestment += (averagePrice * quantity); // Update total investment
        }
    }

    // Function to save portfolio data to a JSON file
    void savePortfolio() {
        json data;                                 // Create a JSON object to hold data
        ifstream inFile(fileName);                 // Open the file for reading
        if (inFile.good()) {                       // Check if the file is good
            inFile >> data;                        // Read existing data
        }
        inFile.close();                            // Close the file

        // Iterate through holdings and save to JSON
        for (const auto& [stockID, stock] : holdingsMap) {
            data[userID]["portfolio"][stockID] = {
                {"Quantity", stock.getQuantity()}, // Save quantity
                {"AveragePrice", stock.getBuyPrice()}, // Save average price
                {"CurrentPrice", stock.getPrice()} // Save current price
            };
        }

        ofstream outFile(fileName);                // Open the file for writing
        outFile << setw(4) << data << endl;       // Write the JSON data to the file
        outFile.close();                           // Close the file
    }

    // Function to sort holdings by profit
    void sortByProfit() {
        sort(holdings.begin(), holdings.end(), 
             [](const OwnedStock& a, const OwnedStock& b) {
                 return a.getProfit() > b.getProfit(); // Sort in descending order of profit
             });
    }

    // Function to sort holdings by loss
    void sortByLoss() {
        sort(holdings.begin(), holdings.end(), 
             [](const OwnedStock& a, const OwnedStock& b) {
                 return a.getProfit() < b.getProfit(); // Sort in ascending order of profit
             });
    }

    // Function to sort holdings by amount invested
    void sortByAmountInvested() {
        sort(holdings.begin(), holdings.end(), 
             [](const OwnedStock& a, const OwnedStock& b) {
                 return a.getTotalInvested() > b.getTotalInvested(); // Sort in descending order of total invested
             });
    }

    // Function to sort holdings by returns
    void sortByReturns() {
        sort(holdings.begin(), holdings.end(), 
             [](const OwnedStock& a, const OwnedStock& b) {
                 return a.getReturns() > b.getReturns(); // Sort in descending order of returns
             });
    }
};
