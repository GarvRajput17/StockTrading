#include "Backend_Classes/Class_files/Header Files/Portfolio.hpp"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#define GREEN "\033[32m"
#define RED "\033[31m"
#define BLUE "\033[34m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define RESET "\033[0m"
#define BOLD "\033[1m"

using namespace std::this_thread;
using namespace std::chrono;


Portfolio::Portfolio(int numberOfStocks, double totalInvested, double currentValue,
                    double returns, vector<OwnedStock> holdings, string userID)
    : numberOfStocks(numberOfStocks),
      totalPortfolioInvestment(totalInvested),
      currentValue(currentValue),
      returns(returns),
      holdings(holdings),
      userID(userID) {}

void Portfolio::displayPortfolio() {
    int uniqueStocks = 0;
    double totalInvested = 0.0, totalProfit = 0.0, totalReturns = 0.0, currentPortfolioValue = 0.0;

    cout << BOLD << "\n=== 📊 Portfolio Summary 📊 ===" << RESET << endl;
    cout << CYAN << string(50, '-') << RESET << endl;

    // Animated loading header
    cout << BOLD << "Loading Portfolio Details";
    for(int i = 0; i < 3; i++) {
        this_thread::sleep_for(chrono::milliseconds(300));
        cout << "." << flush;
    }
    cout << RESET << "\n\n";

    // Holdings Header
    cout << BLUE << "Stock Details:" << RESET << "\n";
    cout << string(50, '-') << "\n";

    // Display each stock with a small delay
    for (const auto& stock : holdings) {
        this_thread::sleep_for(chrono::milliseconds(100));
        uniqueStocks++;
        
        double investment = stock.getBuyPrice() * stock.getQuantity();
        double currentStockValue = stock.getPrice() * stock.getQuantity();
        double stockReturns = ((currentStockValue - investment) / investment) * 100;
        
        totalInvested += investment;
        totalProfit += (currentStockValue - investment);
        currentPortfolioValue += currentStockValue;

        cout << CYAN << "Stock Symbol: " << RESET << stock.getstockname() << "\n";
        cout << BLUE << "Quantity: " << RESET << stock.getQuantity() << "\n";
        cout << GREEN << "Buy Price: $" << RESET << fixed << setprecision(2) << stock.getBuyPrice() << "\n";
        cout << YELLOW << "Current Price: $" << RESET << stock.getPrice() << "\n";
        cout << (stockReturns >= 0 ? GREEN : RED) << "Returns: " << stockReturns << "%" << RESET << "\n";
        cout << string(50, '-') << "\n";
    }

    totalReturns = (totalInvested > 0) ? (totalProfit / totalInvested) * 100 : 0;

    // Animated loading for metrics
    cout << "\n" << BOLD << "Calculating Portfolio Metrics";
    for(int i = 0; i < 3; i++) {
        this_thread::sleep_for(chrono::milliseconds(300));
        cout << "." << flush;
    }
    cout << RESET << "\n\n";

    // Portfolio Metrics
    cout << BOLD << "Portfolio Overview:" << RESET << "\n";
    cout << string(50, '-') << "\n";
    cout << CYAN << "Unique Stocks: " << RESET << uniqueStocks << "\n";
    cout << BLUE << "Total Investment: $" << RESET << fixed << setprecision(2) << totalInvested << "\n";
    cout << YELLOW << "Current Portfolio Value: $" << RESET << currentPortfolioValue << "\n";
    cout << "Total Profit/Loss: " << (totalProfit >= 0 ? GREEN : RED) << "$" << totalProfit << RESET << "\n";
    cout << "Overall Returns: " << (totalReturns >= 0 ? GREEN : RED) << totalReturns << "%" << RESET << "\n";
    cout << string(50, '-') << "\n";
}



// Rest of the methods remain the same as they don't involve currency display
void Portfolio::loadPortfolio() {
    ifstream inFile(fileName);
    if (!inFile.is_open()) {
        throw runtime_error("Unable to open " + fileName);
    }

    json data;
    inFile >> data;
    inFile.close();

    if (!data.contains(userID) || !data[userID].contains("portfolio")) {
        return;
    }

    json portfolioData = data[userID]["portfolio"];
    for (const auto& [stockID, stockData] : portfolioData.items()) {
        if (stockID == "OwnedStockId") continue;

        int quantity = stockData["Quantity"].get<int>();
        double averagePrice = stockData["AveragePrice"].get<double>();
        double currentPrice = stockData["CurrentPrice"].get<double>();

        OwnedStock stock(
            stockID,
            stockID,
            currentPrice,
            userID,
            quantity,
            averagePrice
        );

        holdings.push_back(stock);
        holdingsMap[stockID] = stock;
        numberOfStocks++;
        totalPortfolioInvestment += (averagePrice * quantity);
    }
}

// Sorting methods remain unchanged as they don't involve currency display
void Portfolio::savePortfolio() {
    json data;
    ifstream inFile(fileName);
    if (inFile.good()) {
        inFile >> data;
    }
    inFile.close();

    for (const auto& [stockID, stock] : holdingsMap) {
        data[userID]["portfolio"][stockID] = {
            {"Quantity", stock.getQuantity()},
            {"AveragePrice", stock.getBuyPrice()},
            {"CurrentPrice", stock.getPrice()}
        };
    }

    ofstream outFile(fileName);
    outFile << setw(4) << data << endl;
    outFile.close();
}

void Portfolio::sortByProfit() {
    sort(holdings.begin(), holdings.end(),
        [](const OwnedStock& a, const OwnedStock& b) {
            return a.getProfit() > b.getProfit();
        });
}

void Portfolio::sortByLoss() {
    sort(holdings.begin(), holdings.end(),
        [](const OwnedStock& a, const OwnedStock& b) {
            return a.getProfit() < b.getProfit();
        });
}

void Portfolio::sortByAmountInvested() {
    sort(holdings.begin(), holdings.end(),
        [](const OwnedStock& a, const OwnedStock& b) {
            return a.getTotalInvested() > b.getTotalInvested();
        });
}

void Portfolio::sortByReturns() {
    sort(holdings.begin(), holdings.end(),
        [](const OwnedStock& a, const OwnedStock& b) {
            return a.getReturns() > b.getReturns();
        });
}
