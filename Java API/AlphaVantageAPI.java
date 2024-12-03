import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.FileWriter;
import java.net.URI;
import java.net.URL;
import java.net.HttpURLConnection;
import org.json.JSONObject;  // This should work now with the downloaded jar

public class AlphaVantageAPI {
    private static final String API_KEY = "";  // Replace with your Alpha Vantage API key

    public static void main(String[] args) {
        if (args.length == 0) {
            System.out.println("No stock symbol provided.");
            return;
        }

        String stockSymbol = args[0];
        String urlString = "https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol=" 
                            + stockSymbol + "&interval=5min&apikey=" + API_KEY;

        try {
            // Convert string to URI first, then to URL to avoid the deprecation warning
            URI uri = new URI(urlString);
            URL url = uri.toURL();  // Convert URI to URL

            // Create a request to Alpha Vantage API
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setRequestMethod("GET");
            BufferedReader reader = new BufferedReader(new InputStreamReader(conn.getInputStream()));
            String line;
            StringBuilder response = new StringBuilder();

            while ((line = reader.readLine()) != null) {
                response.append(line);
            }
            reader.close();

            // Parse the JSON response
            JSONObject json = new JSONObject(response.toString());
            JSONObject metaData = json.getJSONObject("Meta Data");
            String lastRefreshed = metaData.getString("3. Last Refreshed");

            JSONObject timeSeries = json.getJSONObject("Time Series (5min)");
            JSONObject latestData = timeSeries.getJSONObject(lastRefreshed);
            String price = latestData.getString("4. close");

            // For simplicity, let's assume today's gain is just the difference between open and close prices
            String openPrice = latestData.getString("1. open");
            double gain = Double.parseDouble(price) - Double.parseDouble(openPrice);

            // Write the output to a file (output.txt)
            FileWriter writer = new FileWriter("output.txt");
            writer.write(price + "\n");
            writer.write(String.format("%.2f", gain) + "\n");
            writer.close();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
