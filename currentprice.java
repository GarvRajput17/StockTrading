import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import org.json.JSONObject;

public class currentprice {
    private static final String API_KEY = "";
    private static final String BASE_URL = "https://www.searchapi.io/api/v1/search?engine=google_finance&q=";

    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Please provide stock symbol as argument");
            return;
        }

        String stockSymbol = args[0].toUpperCase() + ":NASDAQ";
        
        try {
            HttpClient client = HttpClient.newHttpClient();
            HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create(BASE_URL + stockSymbol + "&api_key=" + API_KEY))
                .build();
                
            HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());
            JSONObject jsonResponse = new JSONObject(response.body());
            
            double price = jsonResponse.getJSONObject("summary").getDouble("price");
            System.out.println("Current Price of Stock Trading is: " +  price);
            
        } catch (Exception e) {
            System.out.println("Error: " + e.getMessage());
        }
    }
}
