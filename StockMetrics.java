import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import org.json.JSONObject;
import org.json.JSONArray;

public class StockMetrics {
    private static final String API_KEY = "";
    private static final String BASE_URL = "https://www.searchapi.io/api/v1/search?engine=google_finance&q=";
    
    public static void main(String[] args) {
        if (args.length < 3) {
            System.out.println("Usage: java StockMetrics <stockSymbol> <quantity> <averagePrice>");
            return;
        }

        String stockSymbol = args[0].toUpperCase();
        int quantity = Integer.parseInt(args[1]);
        double averagePrice = Double.parseDouble(args[2]);
        final double USD_TO_INR = 83.0;
        double averagePriceINR = averagePrice * USD_TO_INR;

        try {
            HttpClient client = HttpClient.newHttpClient();
            HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create(BASE_URL + stockSymbol + ":NASDAQ&api_key=" + API_KEY))
                .build();
                
            HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());
            JSONObject jsonResponse = new JSONObject(response.body());
            JSONObject summary = jsonResponse.getJSONObject("summary");
            JSONObject priceChange = summary.getJSONObject("price_change");
            
            double currentPrice = summary.getDouble("price");
            double currentPriceINR = currentPrice * USD_TO_INR;

            JSONObject metrics = new JSONObject();
            metrics.put("stockName", stockSymbol);
            metrics.put("quantity", quantity);
            metrics.put("averageCost", averagePriceINR);
            metrics.put("currentPrice", currentPriceINR);
            metrics.put("totalInvested", averagePriceINR * quantity);
            metrics.put("currentValue", currentPriceINR * quantity);
            
            double profitLoss = (currentPriceINR * quantity) - (averagePriceINR * quantity);
            metrics.put("profitLoss", profitLoss);
            metrics.put("profitLossPercentage", ((currentPriceINR - averagePriceINR) / averagePriceINR) * 100);
            metrics.put("dayChange", priceChange.getDouble("amount"));
            metrics.put("dayChangePercentage", priceChange.getDouble("percentage"));
            metrics.put("movement", priceChange.getString("movement"));

            System.out.println(metrics.toString());
            
        } catch (Exception e) {
            JSONObject errorMetrics = new JSONObject();
            errorMetrics.put("error", true);
            System.out.println(errorMetrics.toString());
        }
    }
}
