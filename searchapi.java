import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.util.Scanner;
import org.json.JSONObject;
import java.awt.Desktop; // used to connect the html files to generate a local server
import java.io.File; // file input output

public class searchapi {
    private static final String API_KEY = "";
    private static final String BASE_URL = "https://www.searchapi.io/api/v1/search?engine=google_finance&q=";

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

    public static void main(String[] args) {
        Scanner scanner = null;
        Process serverProcess = null;
        
        try {
            scanner = new Scanner(System.in);
            
            System.out.print("Enter stock symbol: ");
            String stockSymbol = scanner.nextLine().trim().toUpperCase() + ":NASDAQ";
            
            HttpClient client = HttpClient.newHttpClient();
            HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create(BASE_URL + stockSymbol + "&api_key=" + API_KEY))
                .build();
                
            HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());
            JSONObject jsonResponse = new JSONObject(response.body());
            JSONObject summary = jsonResponse.getJSONObject("summary");
            JSONObject priceChange = summary.getJSONObject("price_change");
            JSONObject financials = jsonResponse.getJSONObject("financials");
            //JSONObject annual = financials.getJSONObject("annual");
            //JSONObject revenue = annual.getJSONObject("revenue");
            //JSONObject net_income = annual.getJSONObject("net_income");

            
            System.out.println("\nBasic Stock Information:");
            System.out.println("Company: " + getValueOrNull(summary, "title"));
            System.out.println("Current Price: " + getValueOrNull(summary, "price") + " " + getValueOrNull(summary, "currency"));
            
            displayHelp();  // Show commands once at start
            
            while (true) {
                System.out.print("\nEnter command (7 for help): ");
                String choice = scanner.nextLine();
                
                switch (choice) {
                    case "1":
                        System.out.println("\nPrice Details:");
                        System.out.println("Current Price: " + getValueOrNull(summary, "price") + " " + getValueOrNull(summary, "currency"));
                        System.out.println("Price Change: " + getValueOrNull(priceChange, "amount"));
                        System.out.println("Percentage Change: " + getValueOrNull(priceChange, "percentage") + "%");
                        System.out.println("Movement: " + getValueOrNull(priceChange, "movement"));
                        break;
                        
                    case "2":
                        System.out.println("\nCompany Information:");
                        System.out.println("Company Name: " + getValueOrNull(summary, "title"));
                        System.out.println("Stock Symbol: " + getValueOrNull(summary, "stock"));
                        System.out.println("Exchange: " + getValueOrNull(summary, "exchange"));
                        break;
                        
                    case "3":
                        System.out.println("\nTrading Metrics:");
                        if (jsonResponse.has("graph") && !jsonResponse.getJSONArray("graph").isEmpty()) {
                            JSONObject latestData = jsonResponse.getJSONArray("graph").getJSONObject(0);
                            System.out.println("Latest Volume: " + getValueOrNull(latestData, "volume"));
                            System.out.println("Trading Date: " + getValueOrNull(latestData, "date"));
                        }
                        break;
                        
                    case "4":
                        System.out.println("\nMarket Performance:");
                        System.out.println("Price Movement: " + getValueOrNull(priceChange, "movement"));
                        System.out.println("Change Amount: " + getValueOrNull(priceChange, "amount"));
                        System.out.println("Change Percentage: " + getValueOrNull(priceChange, "percentage") + "%");
                        System.out.println("Last Updated: " + getValueOrNull(summary, "date"));
                       // System.out.println("")
                        break;

                    case "5":
                        System.out.println("\nOpening Stock Graph...");
                        try {
                            ProcessBuilder pb = new ProcessBuilder("python", "-m", "http.server", "8000");
                            pb.directory(new File(System.getProperty("user.dir")));
                            serverProcess = pb.start();
                            
                            Desktop.getDesktop().browse(new URI("http://localhost:8000/av1.html?symbol=" + stockSymbol.replace(":NASDAQ", "")));
                            
                            System.out.println("Graph opened.\nPress enter to continue");
                            scanner.nextLine();
                            
                            if (serverProcess != null) {
                                serverProcess.destroy();
                            }
                        } catch (Exception e) {
                            System.out.println("Error opening graph: " + e.getMessage());
                        }
                        break;
                        
                    case "6":
                        System.out.println("Thank you for using the stock information system!");
                        return;

                    case "7":
                        displayHelp();
                        break;
                    
                    

                    
                        
                    default:
                        System.out.println("Invalid command.\n Enter 7 for help.");
                }
            }
            
        } catch (Exception e) {
            System.out.println("Error details: " + e.getMessage());
            e.printStackTrace();
        } finally {
            if (scanner != null) {
                scanner.close();
            }
            if (serverProcess != null) {
                serverProcess.destroy();
            }
        }
    }
    
    private static String getValueOrNull(JSONObject obj, String key) {
        return obj.has(key) ? obj.get(key).toString() : "null";
    }
}
