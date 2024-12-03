package JNI;
import java.util.*;
//import java.io.*;
public class Helper {
    static {
        System.loadLibrary("stocktrading"); // Native library name
    }

    // Native method declarations for the options code calls
    private native void displayMenuBeforeLogin();
    private native void displayMenuAfterLogin();
    private native void displayStockOperationsMenu();
    private native void displayPortfolioMenu();
    private native String getUserID();
    private native String getPassword();
    
    // User related native methods 
    private native boolean registerUser(String userID, String password);
    private native boolean login(String userID, String password);
    private native void logout();
    private native boolean deleteAccount();
    private native void addMoneyToWallet(double amount);
    private native double checkWalletBalance();
    
    // Stock related native methods
    private native void displayStockDetails();
    private native void buyStock(String stockName, int quantity);
    private native void sellStock(String stockName, int quantity);
    private native void displayPortfolio();
    private native void displayReturns();
    private native void showTransactionHistory();
    private native void addToWatchlist(String stockName);
    private native void viewWatchlist();
    private native void handleStockChoice(String choice);

    // Portfolio related native methods
    private native void sortPortfolio(int sortOption);
    private native void sortByProfit();
    private native void sortByLoss();
    private native void sortByInvestment();
    private native void sortByReturns();

    public void run() {
        String choice;
        Scanner scanner = new Scanner(System.in);
        boolean isLoggedIn = false;

        while(true) {
            if(!isLoggedIn) {
                displayMenuBeforeLogin();
                choice = scanner.nextLine();

                switch(choice) {
                    case "3":
                        System.out.println("Exiting program.");
                        return;
                    case "1":
                        String newUserID = getUserID();
                        String newPassword = getPassword();
                        isLoggedIn = registerUser(newUserID, newPassword);
                        break;
                    case "2":
                        String userID = getUserID();
                        String password = getPassword();
                        isLoggedIn = login(userID, password);
                        break;
                    default:
                        System.out.println("Invalid choice. Select 1-3.");
                }
            } else {
                displayMenuAfterLogin();
                choice = scanner.nextLine();

                switch(choice) {
                    case "11":
                        System.out.println("Exiting program.");
                        return;
                    case "1":
                        addMoneyToWallet(0);
                        break;
                    case "2":
                        System.out.printf("Current Wallet Balance: $%.2f%n", checkWalletBalance());
                        break;
                    case "3":
                        handleStockOperations(scanner);
                        break;
                    case "4":
                        handlePortfolioOperations(scanner);
                        break;
                    case "5":
                        displayReturns();
                        break;
                    case "6":
                        showTransactionHistory();
                        break;
                    case "7":
                        System.out.print("Enter stock symbol to add to watchlist: ");
                        addToWatchlist(scanner.nextLine());
                        break;
                    case "8":
                        viewWatchlist();
                        break;
                    case "9":
                        logout();
                        isLoggedIn = false;
                        break;
                    case "10":
                        if(deleteAccount()) {
                            isLoggedIn = false;
                        }
                        break;
                    default:
                        System.out.println("Invalid choice. Please select a valid option.");
                }
            }
        }
    }

    //private native void handleStockChoice(String choice);

private void handleStockOperations(Scanner scanner) {
    displayStockDetails();
    displayStockOperationsMenu();
    String choice = scanner.nextLine();

    switch(choice) {
        case "1":
            System.out.print("Enter stock symbol (e.g., AAPL): ");
            String buyStockName = scanner.nextLine();
            System.out.print("Enter quantity to buy: ");
            int buyQuantity = Integer.parseInt(scanner.nextLine());
            buyStock(buyStockName, buyQuantity);
            break;
        case "2":
            System.out.print("Enter stock symbol to sell: ");
            String sellStockName = scanner.nextLine();
            System.out.print("Enter quantity to sell: ");
            int sellQuantity = Integer.parseInt(scanner.nextLine());
            sellStock(sellStockName, sellQuantity);
            break;
        case "3":
            handleStockChoice(choice);
            break;
        case "4":
            return;
    }
}


    private void handlePortfolioOperations(Scanner scanner) {
        String choice;
        do {
            displayPortfolioMenu();
            choice = scanner.nextLine();
            switch(choice) {
                case "1":
                    displayPortfolio();
                    break;
                case "2":
                    sortByProfit();
                    break;
                case "3":
                    sortByLoss();
                    break;
                case "4":
                    sortByInvestment();
                    break;
                case "5":
                    sortByReturns();
                    break;
                case "6":
                    handleStockOperations(scanner);
                    break;
            }
        } while(!choice.equals("7"));
    }
}
