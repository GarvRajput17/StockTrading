import java.io.BufferedReader;                     // Import BufferedReader for reading text from input streams
import java.io.InputStreamReader;                  // Import InputStreamReader for converting byte streams to character streams
//import java.io.FileWriter;                       // Uncomment to use FileWriter for writing to files
import java.net.URI;                               // Import URI class for handling Uniform Resource Identifiers
import java.net.URL;                                // Import URL class for handling URLs
import java.net.HttpURLConnection;                  // Import HttpURLConnection for making HTTP requests
import org.json.JSONObject;                         // Import JSONObject for JSON manipulation
import java.util.Scanner;                          // Import Scanner for reading user input

public class alphavantagetest {                   // Define the main class
    private static final String API_KEY = "your_api_key";  // Replace with your Alpha Vantage API key

    public static void main(String[] args) {       // Main method to execute the program
        Scanner scanner = new Scanner(System.in);  // Create a Scanner object for user input

        System.out.println("Select an option:");   // Prompt user to select an option
        System.out.println("1: Fetch Intraday Stock Data"); // Option to fetch intraday stock data
        System.out.println("2: Fetch Daily Stock Data");    // Option to fetch daily stock data
        System.out.println("3: Fetch Currency Exchange Rate"); // Option to fetch currency exchange rates
        System.out.println("4: Fetch SMA (Simple Moving Average)"); // Option to fetch SMA
        System.out.println("5: Fetch RSI (Relative Strength Index)"); // Option to fetch RSI
        int option = scanner.nextInt();             // Read user choice

        switch (option) {                            // Switch statement to handle user choice
            case 1:
                fetchIntradayStockData(scanner);    // Fetch intraday stock data
                break;
            case 2:
                fetchDailyStockData(scanner);         // Fetch daily stock data
                break;
            case 3:
                fetchCurrencyExchangeRate(scanner);   // Fetch currency exchange rate
                break;
            case 4:
                fetchSMA(scanner);                     // Fetch Simple Moving Average
                break;
            case 5:
                fetchRSI(scanner);                     // Fetch Relative Strength Index
                break;
            default:
                System.out.println("Invalid option selected."); // Handle invalid option
        }

        scanner.close();                             // Close the scanner
    }

    // 1. Fetch Intraday Stock Data
    private static void fetchIntradayStockData(Scanner scanner) { // Method to fetch intraday stock data
        System.out.print("Enter stock symbol (e.g., IBM): "); // Prompt for stock symbol
        String stockSymbol = scanner.next();               // Read stock symbol
        String urlString = "https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol=" 
                            + stockSymbol + "&interval=5min&apikey=" + API_KEY; // Create URL for API request
        
        fetchAndDisplayStockData(urlString, "Time Series (5min)"); // Call helper method to fetch and display data
    }

    // 2. Fetch Daily Stock Data
    private static void fetchDailyStockData(Scanner scanner) { // Method to fetch daily stock data
        System.out.print("Enter stock symbol (e.g., IBM): "); // Prompt for stock symbol
        String stockSymbol = scanner.next();               // Read stock symbol
        String urlString = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=" 
                            + stockSymbol + "&apikey=" + API_KEY; // Create URL for API request
        
        fetchAndDisplayStockData(urlString, "Time Series (Daily)"); // Call helper method to fetch and display data
    }

    // 3. Fetch Currency Exchange Rate
    private static void fetchCurrencyExchangeRate(Scanner scanner) { // Method to fetch currency exchange rate
        System.out.print("Enter base currency (e.g., USD): "); // Prompt for base currency
        String fromCurrency = scanner.next();                  // Read base currency
        System.out.print("Enter target currency (e.g., EUR): "); // Prompt for target currency
        String toCurrency = scanner.next();                    // Read target currency
        String urlString = "https://www.alphavantage.co/query?function=CURRENCY_EXCHANGE_RATE&from_currency=" 
                            + fromCurrency + "&to_currency=" + toCurrency + "&apikey=" + API_KEY; // Create URL for API request
        
        try {
            URI uri = new URI(urlString);                     // Create a URI object from the URL string
            URL url = uri.toURL();                            // Convert URI to URL
            
            HttpURLConnection conn = (HttpURLConnection) url.openConnection(); // Open a connection to the URL
            conn.setRequestMethod("GET");                     // Set the request method to GET
            
            BufferedReader reader = new BufferedReader(new InputStreamReader(conn.getInputStream())); // Create a BufferedReader to read the response
            StringBuilder response = new StringBuilder(); // StringBuilder to accumulate the response
            String line;

            while ((line = reader.readLine()) != null) { // Read the response line by line
                response.append(line); // Append each line to the response
            }
            reader.close(); // Close the reader

            JSONObject json = new JSONObject(response.toString()); // Parse the response as JSON
            JSONObject exchangeRate = json.getJSONObject("Realtime Currency Exchange Rate"); // Get the exchange rate object
            String rate = exchangeRate.getString("5. Exchange Rate"); // Extract the exchange rate value

            System.out.println("Exchange Rate: " + rate); // Print the exchange rate
        } catch (Exception e) {
            e.printStackTrace(); // Print the stack trace in case of an error
        }
    }

    // 4. Fetch SMA (Simple Moving Average)
    private static void fetchSMA(Scanner scanner) { // Method to fetch Simple Moving Average
        System.out.print("Enter stock symbol (e.g., IBM): "); // Prompt for stock symbol
        String stockSymbol = scanner.next(); // Read stock symbol
        String urlString = "https://www.alphavantage.co/query?function=SMA&symbol=" + stockSymbol
                            + "&interval=daily&time_period=10&series_type=close&apikey=" + API_KEY; // Create URL for API request

        fetchAndDisplayTechnicalIndicator(urlString, "Technical Analysis: SMA", "SMA"); // Call helper method to fetch and display SMA
    }

    // 5. Fetch RSI (Relative Strength Index)
    private static void fetchRSI(Scanner scanner) { // Method to fetch Relative Strength Index
        System.out.print("Enter stock symbol (e.g., IBM): "); // Prompt for stock symbol
        String stockSymbol = scanner.next(); // Read stock symbol
        String urlString = "https://www.alphavantage.co/query?function=RSI&symbol=" + stockSymbol
                            + "&interval=daily&time_period=10&series_type=close&apikey=" + API_KEY; // Create URL for API request

        fetchAndDisplayTechnicalIndicator(urlString, "Technical Analysis: RSI", "RSI"); // Call helper method to fetch and display RSI
    }

    // Helper function to fetch stock data (daily/intraday)
    private static void fetchAndDisplayStockData(String urlString, String timeSeriesKey) { // Method to fetch and display stock data
        try {
            URI uri = new URI(urlString); // Create a URI object from the URL string
            URL url = uri.toURL(); // Convert URI to URL

            HttpURLConnection conn = (HttpURLConnection) url.openConnection(); // Open a connection to the URL
            conn.setRequestMethod("GET"); // Set the request method to GET

            BufferedReader reader = new BufferedReader(new InputStreamReader(conn.getInputStream())); // Create a BufferedReader to read the response
            StringBuilder response = new StringBuilder(); // StringBuilder to accumulate the response
            String line;

            while ((line = reader.readLine()) != null) { // Read the response line by line
                response.append(line); // Append each line to the response
            }
            reader.close(); // Close the reader

            JSONObject json = new JSONObject(response.toString()); // Parse the response as JSON
            JSONObject timeSeries = json.getJSONObject(timeSeriesKey); // Get the time series data

            // Get the latest timestamp from the time series data
            String lastTimestamp = timeSeries.keys().next(); // This gets the most recent timestamp
            JSONObject latestData = timeSeries.getJSONObject(lastTimestamp); // Get the latest data
            String price = latestData.getString("4. close"); // Extract the closing price

            System.out.println("Latest Fetched Timestamp: " + lastTimestamp); // Print the latest timestamp
            System.out.println("Latest Close Price: " + price); // Print the latest closing price

        } catch (Exception e) {
            System.out.println("Error fetching stock data: " + e.getMessage()); // Print error message in case of an exception
        }
    }

    // Helper function to fetch and display technical indicators (SMA, RSI)
    private static void fetchAndDisplayTechnicalIndicator(String urlString, String analysisKey, String indicatorKey) { // Method to fetch and display technical indicators
        try {
            URI uri = new URI(urlString); // Create a URI object from the URL string
            URL url = uri.toURL(); // Convert URI to URL

            HttpURLConnection conn = (HttpURLConnection) url.openConnection(); // Open a connection to the URL
            conn.setRequestMethod("GET"); // Set the request method to GET

            BufferedReader reader = new BufferedReader(new InputStreamReader(conn.getInputStream())); // Create a BufferedReader to read the response
            StringBuilder response = new StringBuilder(); // StringBuilder to accumulate the response
            String line;

            while ((line = reader.readLine()) != null) { // Read the response line by line
                response.append(line); // Append each line to the response
            }
            reader.close(); // Close the reader

            JSONObject json = new JSONObject(response.toString()); // Parse the response as JSON
            JSONObject technicalAnalysis = json.getJSONObject(analysisKey); // Get the technical analysis data
            String lastDate = technicalAnalysis.keys().next(); // Get the most recent date
            String indicatorValue = technicalAnalysis.getJSONObject(lastDate).getString(indicatorKey); // Extract the indicator value

            System.out.println(indicatorKey + " Value for " + lastDate + ": " + indicatorValue); // Print the indicator value

        } catch (Exception e) {
            System.out.println("Error fetching technical indicator: " + e.getMessage()); // Print error message in case of an exception
        }
    }
}
