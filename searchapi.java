import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.util.Scanner;
import org.json.JSONArray;
import org.json.JSONObject;
import java.awt.Desktop; // used to connect the html files to generate a local server
import java.io.BufferedReader;
import java.io.File; // file input output
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;


public class searchapi {
    private static final String RESET = "\u001B[0m";
    private static final String GREEN = "\u001B[32m";
    private static final String RED = "\u001B[31m";
    private static final String BLUE = "\u001B[34m";
    private static final String YELLOW = "\u001B[33m";
    private static final String CYAN = "\u001B[36m";
    private static final String BOLD = "\u001B[1m";
    
    private static final String API_KEY = "";
    private static final String BASE_URL = "https://www.searchapi.io/api/v1/search?engine=google_finance&q=";

    private static void displayHelp() {
        System.out.println(BOLD + "\n📊 Available Commands 📊" + RESET);
        System.out.println(CYAN + "┌─────────────────────────────┐");
        System.out.println("│ 1. 💰 Price Details          │");
        System.out.println("│ 2. 🏢 Company Information    │");
        System.out.println("│ 3. 📈 Trading Metrics        │");
        System.out.println("│ 4. 📊 Market Performance     │");
        System.out.println("│ 5. 📉 View Stock Graph       │");
        System.out.println("│ 6. 🚪 Exit                   │");
        System.out.println("│ 7. ❓ Show this help menu    │");
        System.out.println("└─────────────────────────────┘" + RESET);
    }

    public static void main(String[] args) {
        Scanner scanner = null;
        Process serverProcess = null;
        try {
            scanner = new Scanner(System.in);
            System.out.print(BOLD + "🔍 Enter stock symbol: " + RESET);
            String stockSymbol = scanner.nextLine().trim().toUpperCase() + ":NASDAQ";

            System.out.println(CYAN + "\n⌛ Fetching stock data..." + RESET);
            HttpClient client = HttpClient.newHttpClient();
            HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create(BASE_URL + stockSymbol + "&api_key=" + API_KEY))
                .build();

            HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());
            String jsonResponseStr = response.body();
            saveToJson(stockSymbol, jsonResponseStr);

            JSONObject jsonResponse = new JSONObject(response.body());
            JSONObject summary = jsonResponse.getJSONObject("summary");
            JSONObject priceChange = summary.getJSONObject("price_change");

            System.out.println(BOLD + "\n=== 📈 Basic Stock Information 📈 ===" + RESET);
            System.out.println(BLUE + "🏢 Company: " + RESET + getValueOrNull(summary, "title"));
            System.out.println(GREEN + "💰 Current Price: " + RESET + getValueOrNull(summary, "price") + " " + getValueOrNull(summary, "currency"));

            displayHelp();

            while (true) {
                System.out.print(YELLOW + "\n📝 Enter command (7 for help): " + RESET);
                String choice = scanner.nextLine();

                switch (choice) {
                    case "1":
                        System.out.println(BOLD + "\n=== 💰 Price Details 💰 ===" + RESET);
                        System.out.println(CYAN + "┌────────────────────────────────────────┐");
                        System.out.println("│ Current Price: " + formatValue(getValueOrNull(summary, "price")) + " " + getValueOrNull(summary, "currency"));
                        System.out.println("│ Price Change: " + formatValue(getValueOrNull(priceChange, "amount")));
                        System.out.println("│ Change %    : " + formatValue(getValueOrNull(priceChange, "percentage")) + "%");
                        System.out.println("│ Movement    : " + formatValue(getValueOrNull(priceChange, "movement")));
                        System.out.println("└────────────────────────────────────────┘" + RESET);
                        break;

                    case "2":
                        System.out.println(BOLD + "\n=== 🏢 Company Information 🏢 ===" + RESET);
                        System.out.println(BLUE + "┌────────────────────────────────────────┐");
                        System.out.println("│ Company Name: " + formatValue(getValueOrNull(summary, "title")));
                        System.out.println("│ Stock Symbol: " + formatValue(getValueOrNull(summary, "stock")));
                        System.out.println("│ Exchange    : " + formatValue(getValueOrNull(summary, "exchange")));
                        System.out.println("└────────────────────────────────────────┘" + RESET);
                        break;

                    case "3":
                        System.out.println(BOLD + "\n=== 📈 Trading Metrics 📈 ===" + RESET);
                        if (jsonResponse.has("graph") && !jsonResponse.getJSONArray("graph").isEmpty()) {
                            JSONObject latestData = jsonResponse.getJSONArray("graph").getJSONObject(0);
                            System.out.println(GREEN + "┌────────────────────────────────────────┐");
                            System.out.println("│ Latest Volume: " + formatValue(getValueOrNull(latestData, "volume")));
                            System.out.println("│ Trading Date : " + formatValue(getValueOrNull(latestData, "date")));
                            System.out.println("└────────────────────────────────────────┘" + RESET);
                        }
                        break;

                    case "4":
                        System.out.println(BOLD + "\n=== 📊 Market Performance 📊 ===" + RESET);
                        System.out.println(YELLOW + "┌────────────────────────────────────────┐");
                        System.out.println("│ Price Movement : " + formatValue(getValueOrNull(priceChange, "movement")));
                        System.out.println("│ Change Amount  : " + formatValue(getValueOrNull(priceChange, "amount")));
                        System.out.println("│ Change %       : " + formatValue(getValueOrNull(priceChange, "percentage")) + "%");
                        System.out.println("│ Last Updated   : " + formatValue(getValueOrNull(summary, "date")));
                        System.out.println("└────────────────────────────────────────┘" + RESET);
                        break;

                    
                    case "5":
                        System.out.println(CYAN + "\n📊 Opening Stock Graph..." + RESET);
                        try {
                            ProcessBuilder pb = new ProcessBuilder("python", "-m", "http.server", "8000");
                            pb.redirectError(ProcessBuilder.Redirect.DISCARD);
                            serverProcess = pb.start();
                            // Extract the symbol without :NASDAQ suffix for the graph URL
                            String graphSymbol = stockSymbol.split(":")[0];
                            Desktop.getDesktop().browse(new URI("http://localhost:8000/Graph%20Widgets/av2.html?symbol=" + graphSymbol));

                            System.out.println(GREEN + "✨ Graph opened successfully!" + RESET);
                            System.out.println(YELLOW + "Press enter to continue" + RESET);
                            scanner.nextLine();
                            if (serverProcess != null) {
                                serverProcess.destroy();
                            }
                        } catch (Exception e) {
                            System.out.println(RED + "❌ Error opening graph: " + e.getMessage() + RESET);
                        }
                        break;
                

                    case "6":
                        System.out.println(GREEN + "\n✨ Thank you for using the stock information system! ✨" + RESET);
                        return;

                    case "7":
                        displayHelp();
                        break;

                    default:
                        System.out.println(RED + "❌ Invalid command. Enter 7 for help." + RESET);
                }
            }
        } catch (Exception e) {
            System.out.println(RED + "❌ Error details: " + e.getMessage() + RESET);
            e.printStackTrace();
        } finally {
            if (scanner != null) scanner.close();
            if (serverProcess != null) serverProcess.destroy();
        }
    }

    private static String getValueOrNull(JSONObject obj, String key) {
        return obj.has(key) ? obj.get(key).toString() : "N/A";
    }

    private static String formatValue(String value) {
        return String.format("%-15s", value);
    }

    //private static void saveToJson;
    private static void saveToJson(String stockSymbol, String jsonResponse) {
        JSONObject stockdata = new JSONObject();
        File jsonfile = new File("stockdetails.json");
        
        // Read existing data if file exists
        if (jsonfile.exists()) {
            try {
                FileReader reader = new FileReader(jsonfile);
                BufferedReader bufferedReader = new BufferedReader(reader);
                StringBuilder content = new StringBuilder();
                String line;
                while ((line = bufferedReader.readLine()) != null) {
                    content.append(line);
                }
                bufferedReader.close();
                reader.close();
                stockdata = new JSONObject(content.toString());
            } catch (IOException e) {
                System.out.println("Error reading file: " + e.getMessage());
            }
        }
    
        // Parse the full response
        JSONObject fullData = new JSONObject(jsonResponse);
        
        // Create filtered data object with only required information
        JSONObject filteredData = new JSONObject();
        
        // Add price information
        // Inside saveToJson method, modify the price information section:
if (fullData.has("summary")) {
    JSONObject summary = fullData.getJSONObject("summary");
    JSONObject priceInfo = new JSONObject();
    priceInfo.put("current_price", summary.optString("price"));
    priceInfo.put("currency", summary.optString("currency"));
    priceInfo.put("52_week_high", summary.optString("52_week_high"));
    priceInfo.put("52_week_low", summary.optString("52_week_low"));
    if (summary.has("price_change")) {
        priceInfo.put("price_change", summary.getJSONObject("price_change"));
    }
    filteredData.put("price_info", priceInfo);
}

// Modify the financials section to include additional metrics:

        
        // Add news
        if (fullData.has("news")) {
            filteredData.put("news", fullData.getJSONArray("news"));
        }
        
        // Add similar stocks
        if (fullData.has("similar")) {
            filteredData.put("similar_stocks", fullData.getJSONArray("similar"));
        }
        
        // Add financial information (2023 and 2024 only)
        // Replace the financials section in saveToJson with this:
if (fullData.has("financials")) {
    JSONObject financials = fullData.getJSONObject("financials");
    JSONObject filteredFinancials = new JSONObject();
    
    if (financials.has("annual")) {
        JSONArray annualData = financials.getJSONArray("annual");
        
        // Process only the most recent years (2023-2024)
        for (int i = 0; i < annualData.length(); i++) {
            JSONObject yearData = annualData.getJSONObject(i);
            String year = yearData.optString("year");
            
            if (year.equals("2023") || year.equals("2024")) {
                JSONObject yearMetrics = new JSONObject();
                String[] metrics = {
                    "price_to_book",
                    "operating_expense",
                    "total_assets",
                    "total_liabilities",
                    "free_cash_flow",
                    "cash_from_operations",
                    "shares_outstanding",
                    "revenue",
                    "net_profit_margin",
                    "earnings_per_share",
                    "net_change_in_cash",
                    "total_equity",
                    "return_on_assets_percentage",
                    "cash_and_short_investments",
                    "cash_from_financing",
                    "cash_from_investing",
                    "net_income"
                };
                
                for (String metric : metrics) {
                    if (yearData.has(metric)) {
                        yearMetrics.put(metric, yearData.get(metric));
                    }
                }
                
                filteredFinancials.put(year, yearMetrics);
            }
        }
    }
    
    if (filteredFinancials.length() > 0) {
        filteredData.put("financials", filteredFinancials);
    }
}

        
        // Add filtered data to main object
        stockdata.put(stockSymbol, filteredData);
        
        // Write filtered data to file
        try {
            FileWriter writer = new FileWriter(jsonfile);
            writer.write(stockdata.toString(4));
            writer.close();
            System.out.println("Filtered stock data saved successfully");
        } catch (IOException e) {
            System.out.println("Error writing to file: " + e.getMessage());
        }
    }
    

}


