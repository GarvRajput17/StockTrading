import java.net.URI;                                // Import URI class for handling URIs
import java.net.http.HttpClient;                     // Import HttpClient for sending HTTP requests
import java.net.http.HttpRequest;                    // Import HttpRequest for creating HTTP requests
import java.net.http.HttpResponse;                   // Import HttpResponse for handling HTTP responses
import java.util.Scanner;                            // Import Scanner for user input
import org.json.JSONObject;                          // Import JSONObject for JSON manipulation
import java.awt.Desktop;                            // Import Desktop for opening web pages
import java.io.BufferedReader;                       // Import BufferedReader for reading text from input streams
import java.io.File;                                 // Import File for file input/output
import java.io.FileReader;                           // Import FileReader for reading files
import java.io.FileWriter;                           // Import FileWriter for writing to files
import java.io.IOException;                          // Import IOException for handling input/output exceptions

public class searchapi {                            // Define the main class
    private static final String API_KEY = "";       // Define a constant for the API key (currently empty)
    private static final String BASE_URL = "https://www.searchapi.io/api/v1/search?engine=google_finance&q="; // Base URL for the API

    // Method to display available commands to the user
    private static void displayHelp() {
        System.out.println("\nAvailable Commands:");
        System.out.println("1. Price Details");
        System.out.println("2. Company Information");
        System.out.println("3. Trading Metrics");
        System.out.println("4. Market Performance");
        System.out.println("5. View Stock Graph");
        System.out.println("6. Exit");
        System.out.println("7. Show this help menu");
    }

    public static void main(String[] args) {         // Main method to execute the program
        Scanner scanner = null;                      // Declare Scanner for user input
        Process serverProcess = null;                 // Declare a Process for the local server
        
        try {
            scanner = new Scanner(System.in);         // Initialize the Scanner
            
            System.out.print("Enter stock symbol: "); // Prompt for stock symbol
            String stockSymbol = scanner.nextLine().trim().toUpperCase() + ":NASDAQ"; // Format stock symbol
            
            HttpClient client = HttpClient.newHttpClient(); // Create a new HttpClient instance
            HttpRequest request = HttpRequest.newBuilder() // Build the HTTP request
                .uri(URI.create(BASE_URL + stockSymbol + "&api_key=" + API_KEY)) // Set the URI with stock symbol and API key
                .build();
                
            HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString()); // Send the request and get the response
            String jsonResponseStr = response.body(); // Get the response body as a string
            saveToJson(stockSymbol, jsonResponseStr); // Save the response to a JSON file
            JSONObject jsonResponse = new JSONObject(response.body()); // Parse the response as JSON
            JSONObject summary = jsonResponse.getJSONObject("summary"); // Get the summary from the JSON response
            JSONObject priceChange = summary.getJSONObject("price_change"); // Get the price change data
            
            System.out.println("\nBasic Stock Information:"); // Print basic stock information
            System.out.println("Company: " + getValueOrNull(summary, "title")); // Print company name
            System.out.println("Current Price: " + getValueOrNull(summary, "price") + " " + getValueOrNull(summary, "currency")); // Print current price
            
            displayHelp();  // Show commands once at start
            
            while (true) {                              // Loop for user commands
                System.out.print("\nEnter command (7 for help): "); // Prompt for command
                String choice = scanner.nextLine();    // Read user command
                
                switch (choice) {                       // Handle user command
                    case "1":
                        System.out.println("\nPrice Details:"); // Print price details
                        System.out.println("Current Price: " + getValueOrNull(summary, "price") + " " + getValueOrNull(summary, "currency")); // Current price
                        System.out.println("Price Change: " + getValueOrNull(priceChange, "amount")); // Price change amount
                        System.out.println("Percentage Change: " + getValueOrNull(priceChange, "percentage") + "%"); // Percentage change
                        System.out.println("Movement: " + getValueOrNull(priceChange, "movement")); // Price movement
                        break;
                        
                    case "2":
                        System.out.println("\nCompany Information:"); // Print company information
                        System.out.println("Company Name: " + getValueOrNull(summary, "title")); // Company name
                        System.out.println("Stock Symbol: " + getValueOrNull(summary, "stock")); // Stock symbol
                        System.out.println("Exchange: " + getValueOrNull(summary, "exchange")); // Exchange information
                        break;
                        
                    case "3":
                        System.out.println("\nTrading Metrics:"); // Print trading metrics
                        if (jsonResponse.has("graph") && !jsonResponse.getJSONArray("graph").isEmpty()) {
                            JSONObject latestData = jsonResponse.getJSONArray("graph").getJSONObject(0); // Get latest trading data
                            System.out.println("Latest Volume: " + getValueOrNull(latestData, "volume")); // Latest trading volume
                            System.out.println("Trading Date: " + getValueOrNull(latestData, "date")); // Trading date
                        }
                        break;
                        
                    case "4":
                        System.out.println("\nMarket Performance:"); // Print market performance
                        System.out.println("Price Movement: " + getValueOrNull(priceChange, "movement")); // Price movement
                        System.out.println("Change Amount: " + getValueOrNull(priceChange, "amount")); // Change amount
                        System.out.println("Change Percentage: " + getValueOrNull(priceChange, "percentage") + "%"); // Change percentage
                        System.out.println("Last Updated: " + getValueOrNull(summary, "date")); // Last updated date
                        break;

                    case "5":
                        System.out.println("\nOpening Stock Graph..."); // Open stock graph
                        try {
                            ProcessBuilder pb = new ProcessBuilder("python", "-m", "http.server", "8000"); // Start local server
                            pb.directory(new File(System.getProperty("user.dir"))); // Set directory for server
                            serverProcess = pb.start(); // Start the server
                            
                            Desktop.getDesktop().browse(new URI("http://localhost:8000/av1.html?symbol=" + stockSymbol.replace(":NASDAQ", ""))); // Open graph in browser
                            
                            System.out.println("Graph opened.\nPress enter to continue"); // Wait for user input
                            scanner.nextLine();
                            
                            if (serverProcess != null) {
                                serverProcess.destroy(); // Stop the server
                            }
                        } catch (Exception e) {
                            System.out.println("Error opening graph: " + e.getMessage()); // Handle errors
                        }
                        break;
                        
                    case "6":
                        System.out.println("Thank you for using the stock information system!"); // Exit message
                        return;

                    case "7":
                        displayHelp(); // Show help menu
                        break;
                    
                    default:
                        System.out.println("Invalid command.\n Enter 7 for help."); // Handle invalid commands
                }
            }
            
        } 
        catch (Exception e) {
            System.out.println("Error details: " + e.getMessage()); // Handle exceptions
            e.printStackTrace();
        } 
        finally {
            if (scanner != null) {
                scanner.close(); // Close scanner
            }
            if (serverProcess != null) {
                serverProcess.destroy(); // Destroy server process
            }
        }
    }
    
    private static String getValueOrNull(JSONObject obj, String key) { // Helper method to get value from JSON
        return obj.has(key) ? obj.get(key).toString() : "null"; // Return value or "null"
    }

    private static void saveToJson(String stockSymbol, String jsonResponse) { // Method to save stock data to JSON
        JSONObject stockdata = new JSONObject(); // Create JSON object for stock data
        File jsonfile = new File("stockdetails.json"); // Define JSON file
        
        // Read existing data if file exists
        if (jsonfile.exists()) {
            try {
                FileReader reader = new FileReader(jsonfile); // Create FileReader
                BufferedReader bufferedReader = new BufferedReader(reader); // Create BufferedReader
                StringBuilder content = new StringBuilder(); // StringBuilder for content
                String line;
                
                while ((line = bufferedReader.readLine()) != null) {
                    content.append(line); // Append lines to content
                }
                bufferedReader.close(); // Close BufferedReader
                reader.close(); // Close FileReader
                
                stockdata = new JSONObject(content.toString()); // Parse existing content to JSON
            } catch (IOException e) {
                System.out.println("Error reading file: " + e.getMessage()); // Handle read errors
            }
        }
        
        // Add new stock data
        JSONObject newStockData = new JSONObject(jsonResponse); // Create JSON object from response
        stockdata.put(stockSymbol, newStockData); // Add new stock data to existing data
        
        // Write updated data back to file
        try {
            FileWriter writer = new FileWriter(jsonfile); // Create FileWriter
            writer.write(stockdata.toString(4)); // Write JSON data with pretty print
            writer.close(); // Close FileWriter
            System.out.println("Stock data saved successfully"); // Success message 
        } catch (IOException e) {
            System.out.println("Error writing to file: " + e.getMessage()); // Handle write errors
        }
    }
}
