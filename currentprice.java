import java.net.URI;                               // Import URI class for handling URIs
import java.net.http.HttpClient;                    // Import HttpClient for sending HTTP requests
import java.net.http.HttpRequest;                   // Import HttpRequest for creating HTTP requests
import java.net.http.HttpResponse;                  // Import HttpResponse for handling HTTP responses
import org.json.JSONObject;                         // Import JSONObject for JSON manipulation

public class currentprice {                        // Define the main class
    private static final String API_KEY = "";      // Define a constant for the API key (currently empty)
    private static final String BASE_URL = "https://www.searchapi.io/api/v1/search?engine=google_finance&q="; // Define the base URL for the API

    public static void main(String[] args) {       // Main method to execute the program
        if (args.length < 1) {                     // Check if a stock symbol argument is provided
            System.out.println("Please provide stock symbol as argument"); // Print error message if no argument is provided
            return;                                  // Exit the program
        }

        String stockSymbol = args[0].toUpperCase() + ":NASDAQ"; // Convert the stock symbol to uppercase and append the exchange type
        
        try {
            HttpClient client = HttpClient.newHttpClient(); // Create a new HttpClient instance
            HttpRequest request = HttpRequest.newBuilder()  // Build the HTTP request
                .uri(URI.create(BASE_URL + stockSymbol + "&api_key=" + API_KEY)) // Set the URI with stock symbol and API key
                .build();                                  // Build the request
                
            HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString()); // Send the request and get the response
            JSONObject jsonResponse = new JSONObject(response.body()); // Parse the response body as JSON
            
            double price = jsonResponse.getJSONObject("summary").getDouble("price"); // Extract the current price from the JSON response
            System.out.println("Current Price of Stock Trading is: " +  price); // Print the current price
            
        } catch (Exception e) {                          // Catch any exceptions that occur during execution
            System.out.println("Error: " + e.getMessage()); // Print the error message
        }
    }
}
