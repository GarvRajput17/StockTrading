import java.io.BufferedReader;
import java.io.InputStreamReader;
//import java.io.FileWriter;
import java.net.URI;
import java.net.URL;
import java.net.HttpURLConnection;
import org.json.JSONObject;  // JSON parsing library
import java.util.Scanner;

public class alphavantagetest {
    private static final String API_KEY = "your_api_key";  // Replace with your Alpha Vantage API key

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.println("Select an option:");
        System.out.println("1: Fetch Intraday Stock Data");
        System.out.println("2: Fetch Daily Stock Data");
        System.out.println("3: Fetch Currency Exchange Rate");
        System.out.println("4: Fetch SMA (Simple Moving Average)");
        System.out.println("5: Fetch RSI (Relative Strength Index)");
        int option = scanner.nextInt();
        
        switch (option) {
            case 1:
                fetchIntradayStockData(scanner);
                break;
            case 2:
                fetchDailyStockData(scanner);
                break;
            case 3:
                fetchCurrencyExchangeRate(scanner);
                break;
            case 4:
                fetchSMA(scanner);
                break;
            case 5:
                fetchRSI(scanner);
                break;
            default:
                System.out.println("Invalid option selected.");
        }
        
        scanner.close();
    }

    // 1. Fetch Intraday Stock Data
    private static void fetchIntradayStockData(Scanner scanner) {
        System.out.print("Enter stock symbol (e.g., IBM): ");
        String stockSymbol = scanner.next();
        String urlString = "https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol=" 
                            + stockSymbol + "&interval=5min&apikey=" + API_KEY;
        
        fetchAndDisplayStockData(urlString, "Time Series (5min)");
    }

    // 2. Fetch Daily Stock Data
    private static void fetchDailyStockData(Scanner scanner) {
        System.out.print("Enter stock symbol (e.g., IBM): ");
        String stockSymbol = scanner.next();
        String urlString = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=" 
                            + stockSymbol + "&apikey=" + API_KEY;
        
        fetchAndDisplayStockData(urlString, "Time Series (Daily)");
    }

    // 3. Fetch Currency Exchange Rate
    private static void fetchCurrencyExchangeRate(Scanner scanner) {
        System.out.print("Enter base currency (e.g., USD): ");
        String fromCurrency = scanner.next();
        System.out.print("Enter target currency (e.g., EUR): ");
        String toCurrency = scanner.next();
        String urlString = "https://www.alphavantage.co/query?function=CURRENCY_EXCHANGE_RATE&from_currency=" 
                            + fromCurrency + "&to_currency=" + toCurrency + "&apikey=" + API_KEY;
        
        try {
            URI uri = new URI(urlString);
            URL url = uri.toURL();
            
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setRequestMethod("GET");
            
            BufferedReader reader = new BufferedReader(new InputStreamReader(conn.getInputStream()));
            StringBuilder response = new StringBuilder();
            String line;

            while ((line = reader.readLine()) != null) {
                response.append(line);
            }
            reader.close();

            JSONObject json = new JSONObject(response.toString());
            JSONObject exchangeRate = json.getJSONObject("Realtime Currency Exchange Rate");
            String rate = exchangeRate.getString("5. Exchange Rate");

            System.out.println("Exchange Rate: " + rate);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    // 4. Fetch SMA (Simple Moving Average)
    private static void fetchSMA(Scanner scanner) {
        System.out.print("Enter stock symbol (e.g., IBM): ");
        String stockSymbol = scanner.next();
        String urlString = "https://www.alphavantage.co/query?function=SMA&symbol=" + stockSymbol
                            + "&interval=daily&time_period=10&series_type=close&apikey=" + API_KEY;

        fetchAndDisplayTechnicalIndicator(urlString, "Technical Analysis: SMA", "SMA");
    }

    // 5. Fetch RSI (Relative Strength Index)
    private static void fetchRSI(Scanner scanner) {
        System.out.print("Enter stock symbol (e.g., IBM): ");
        String stockSymbol = scanner.next();
        String urlString = "https://www.alphavantage.co/query?function=RSI&symbol=" + stockSymbol
                            + "&interval=daily&time_period=10&series_type=close&apikey=" + API_KEY;

        fetchAndDisplayTechnicalIndicator(urlString, "Technical Analysis: RSI", "RSI");
    }

    // Helper function to fetch stock data (daily/intraday)
    private static void fetchAndDisplayStockData(String urlString, String timeSeriesKey) {
        try {
            URI uri = new URI(urlString);
            URL url = uri.toURL();
    
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setRequestMethod("GET");
    
            BufferedReader reader = new BufferedReader(new InputStreamReader(conn.getInputStream()));
            StringBuilder response = new StringBuilder();
            String line;
    
            while ((line = reader.readLine()) != null) {
                response.append(line);
            }
            reader.close();
    
            JSONObject json = new JSONObject(response.toString());
            JSONObject timeSeries = json.getJSONObject(timeSeriesKey);
    
            // Get the latest timestamp from the time series data
            String lastTimestamp = timeSeries.keys().next(); // This gets the most recent timestamp
            JSONObject latestData = timeSeries.getJSONObject(lastTimestamp);
            String price = latestData.getString("4. close");
    
            System.out.println("Latest Fetched Timestamp: " + lastTimestamp);
            System.out.println("Latest Close Price: " + price);
    
        } catch (Exception e) {
            System.out.println("Error fetching stock data: " + e.getMessage());
        }
    }
    

    // Helper function to fetch and display technical indicators (SMA, RSI)
    private static void fetchAndDisplayTechnicalIndicator(String urlString, String analysisKey, String indicatorKey) {
        try {
            URI uri = new URI(urlString);
            URL url = uri.toURL();

            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setRequestMethod("GET");

            BufferedReader reader = new BufferedReader(new InputStreamReader(conn.getInputStream()));
            StringBuilder response = new StringBuilder();
            String line;

            while ((line = reader.readLine()) != null) {
                response.append(line);
            }
            reader.close();

            JSONObject json = new JSONObject(response.toString());
            JSONObject technicalAnalysis = json.getJSONObject(analysisKey);
            String lastDate = technicalAnalysis.keys().next(); // Get the most recent date
            String indicatorValue = technicalAnalysis.getJSONObject(lastDate).getString(indicatorKey);

            System.out.println(indicatorKey + " Value for " + lastDate + ": " + indicatorValue);

        } catch (Exception e) {
            System.out.println("Error fetching technical indicator: " + e.getMessage());
        }
    }
}
