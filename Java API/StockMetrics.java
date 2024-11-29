import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import org.json.JSONObject;

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

            JSONObject metrics = new JSONObject();
            metrics.put("stockName", stockSymbol);
            metrics.put("quantity", quantity);
            metrics.put("averageCost", averagePrice);
            metrics.put("currentPrice", currentPrice);
            metrics.put("totalInvested", averagePrice * quantity);
            metrics.put("currentValue", currentPrice * quantity);
            double profitLoss = (currentPrice * quantity) - (averagePrice * quantity);
            metrics.put("profitLoss", profitLoss);
            metrics.put("profitLossPercentage", ((currentPrice - averagePrice) / averagePrice) * 100);
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
