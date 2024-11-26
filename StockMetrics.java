import java.net.URI;                              // Import the URI class for handling URIs
import java.net.http.HttpClient;                   // Import HttpClient for sending HTTP requests
import java.net.http.HttpRequest;                   // Import HttpRequest for creating HTTP requests
import java.net.http.HttpResponse;                  // Import HttpResponse for handling HTTP responses
import org.json.JSONObject;                         // Import JSONObject for JSON manipulation
import org.json.JSONArray;                          // Import JSONArray for JSON array manipulation

public class StockMetrics {                        // Define the StockMetrics class
    private static final String API_KEY = "";      // Define a constant for the API key (currently empty)
    private static final String BASE_URL = "https://www.searchapi.io/api/v1/search?engine=google_finance&q="; // Define the base URL for the API
    
    public static void main(String[] args) {       // Main method to execute the program
        if (args.length < 3) {                     // Check if there are fewer than 3 command-line arguments
            System.out.println("Usage: java StockMetrics <stockSymbol> <quantity> <averagePrice>"); // Print usage instructions
            return;                                  // Exit the program
        }

        String stockSymbol = args[0].toUpperCase(); // Convert the first argument to uppercase for stock symbol
        int quantity = Integer.parseInt(args[1]);   // Parse the second argument as an integer for quantity
        double averagePrice = Double.parseDouble(args[2]); // Parse the third argument as a double for average price
        final double USD_TO_INR = 83.0;              // Define the conversion rate from USD to INR
        double averagePriceINR = averagePrice * USD_TO_INR; // Calculate average price in INR

        try {
            HttpClient client = HttpClient.newHttpClient(); // Create a new HttpClient instance
            HttpRequest request = HttpRequest.newBuilder()  // Build the HTTP request
                .uri(URI.create(BASE_URL + stockSymbol + ":NASDAQ&api_key=" + API_KEY)) // Set the URI with stock symbol and API key
                .build();                                  // Build the request
                
            HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString()); // Send the request and get the response
            JSONObject jsonResponse = new JSONObject(response.body()); // Parse the response body as JSON
            JSONObject summary = jsonResponse.getJSONObject("summary"); // Get the summary object from the JSON response
            JSONObject priceChange = summary.getJSONObject("price_change"); // Get the price change object from the summary
            
            double currentPrice = summary.getDouble("price"); // Get the current price from the summary
            double currentPriceINR = currentPrice * USD_TO_INR; // Calculate the current price in INR

            JSONObject metrics = new JSONObject(); // Create a new JSON object to hold metrics
            metrics.put("stockName", stockSymbol); // Add stock name to the metrics
            metrics.put("quantity", quantity);      // Add quantity to the metrics
            metrics.put("averageCost", averagePriceINR); // Add average cost in INR to the metrics
            metrics.put("currentPrice", currentPriceINR); // Add current price in INR to the metrics
            metrics.put("totalInvested", averagePriceINR * quantity); // Calculate and add total invested amount
            metrics.put("currentValue", currentPriceINR * quantity); // Calculate and add current value

            double profitLoss = (currentPriceINR * quantity) - (averagePriceINR * quantity); // Calculate profit or loss
            metrics.put("profitLoss", profitLoss); // Add profit/loss to the metrics
            metrics.put("profitLossPercentage", ((currentPriceINR - averagePriceINR) / averagePriceINR) * 100); // Calculate and add profit/loss percentage
            metrics.put("dayChange", priceChange.getDouble("amount")); // Add daily change amount to the metrics
            metrics.put("dayChangePercentage", priceChange.getDouble("percentage")); // Add daily change percentage to the metrics
            metrics.put("movement", priceChange.getString("movement")); // Add movement type (e.g., up/down) to the metrics

            System.out.println(metrics.toString()); // Print the metrics JSON object
            
        } catch (Exception e) {                      // Catch any exceptions that occur during execution
            JSONObject errorMetrics = new JSONObject(); // Create a new JSON object for error metrics
            errorMetrics.put("error", true);        // Indicate that an error occurred
            System.out.println(errorMetrics.toString()); // Print the error metrics JSON object
        }
    }
}
