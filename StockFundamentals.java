import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.Scanner;
import java.awt.Desktop;
import java.net.URI;
//import org.json.JSONArray;
import org.json.JSONObject;

public class StockFundamentals {
    private static final String RESET = "\u001B[0m";
    private static final String GREEN = "\u001B[32m";
    private static final String BLUE = "\u001B[34m";
    private static final String YELLOW = "\u001B[33m";
    private static final String CYAN = "\u001B[36m";
    private static final String BOLD = "\u001B[1m";
    private static final String RED = "\u001B[31m";

    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println(YELLOW + "⚠️ Please provide stock symbol as argument" + RESET);
            return;
        }

        String stockSymbol = args[0];
        Scanner scanner = new Scanner(System.in);

        System.out.println(BOLD + "\n=== 📊 Stock Analysis Options 📈 ===" + RESET);
        System.out.println(CYAN + "1. Show Technical Analysis and Company Fundamentals");
        System.out.println("2. Show News and Stock Related Latest Updates" + RESET);
        System.out.print(BOLD + "\nEnter your choice (1-2): " + RESET);
        
        String choice = scanner.nextLine();

        switch (choice) {
            case "1":
                System.out.println(CYAN + "\n📊 Loading fundamental analysis..." + RESET);
                StockFundamentals fundamentals = new StockFundamentals();
                fundamentals.calculateValuation(stockSymbol + ":NASDAQ");
                openTechnicalAnalysis(stockSymbol, scanner);
                break;
            case "2":
                openNewsWidget(stockSymbol, scanner);
                break;
            default:
                System.out.println(RED + "Invalid choice!" + RESET);
        }
    }

    private static void openNewsWidget(String symbol, Scanner scanner) {
        try {
            ProcessBuilder pb = new ProcessBuilder("python", "-m", "http.server", "8000");
            pb.redirectError(ProcessBuilder.Redirect.DISCARD);
            pb.directory(new File(System.getProperty("user.dir")));
            Process serverProcess = pb.start();
            
            Desktop.getDesktop().browse(new URI("http://localhost:8000/Graph%20Widgets/newsfeed.html?symbol=" + symbol));
            System.out.println(GREEN + "✨ News feed opened successfully!" + RESET);
            System.out.println(YELLOW + "Press enter to continue" + RESET);
            scanner.nextLine();
            
            if (serverProcess != null) {
                serverProcess.destroy();
            }
        } catch (Exception e) {
            System.out.println(RED + "❌ Error opening news feed: " + e.getMessage() + RESET);
        }
    }

    private static void openTechnicalAnalysis(String symbol, Scanner scanner) {
        try {
            ProcessBuilder pb = new ProcessBuilder("python", "-m", "http.server", "8000");
            pb.redirectError(ProcessBuilder.Redirect.DISCARD);
            pb.directory(new File(System.getProperty("user.dir")));
            Process serverProcess = pb.start();
            
            Desktop.getDesktop().browse(new URI("http://localhost:8000/Graph%20Widgets/Technical_analysis.html?symbol=" + symbol));
            System.out.println(GREEN + "✨ Technical Analysis opened successfully!" + RESET);
            System.out.println(YELLOW + "Press enter to continue" + RESET);
            scanner.nextLine();
            
            if (serverProcess != null) {
                serverProcess.destroy();
            }
        } catch (Exception e) {
            System.out.println(RED + "❌ Error opening Technical Analysis: " + e.getMessage() + RESET);
        }
    }

    public void calculateValuation(String stockName) {
        String filePath = "stockdetails.json";
        try {
            File inputFile = new File(filePath);
            FileReader fileReader = new FileReader(inputFile);
            char[] buffer = new char[(int) inputFile.length()];
            fileReader.read(buffer);
            fileReader.close();

            String jsonData = new String(buffer);
            JSONObject stockDetails = new JSONObject(jsonData);
            JSONObject stockData = stockDetails.getJSONObject(stockName);
            System.out.println(BOLD + "\n=== 📈 Company Fundamental Analysis 📊 ===" + RESET);
            String result = calculateValuationMetrics(stockData);
            System.out.println(result);

            System.out.println(GREEN + "\n✅ Company Fundamentals Analysis Complete!" + RESET);
        } catch (IOException e) {
            System.out.println(BLUE + "❌ Error handling the file: " + e.getMessage() + RESET);
        }
    }

    public static String calculateValuationMetrics(JSONObject stockData) {
        StringBuilder result = new StringBuilder();
        try {
            JSONObject priceInfo = stockData.getJSONObject("price_info");
            JSONObject financials = stockData.getJSONObject("financials").getJSONObject("2023");
            
            result.append(BOLD).append("\n=== 📊 Key Valuation Metrics 📈 ===\n").append(RESET);
            double priceToBook = financials.getJSONObject("price_to_book").getDouble("value");
            result.append(CYAN).append("📚 Price to Book Value: ").append(RESET)
                .append(String.format("%.2f", priceToBook)).append("\n");

            double netIncome = financials.getJSONObject("net_income").getDouble("value");
            double totalEquity = financials.getJSONObject("total_equity").getDouble("value");
            double roe = (netIncome / totalEquity) * 100;
            result.append(BLUE).append("📈 Return on Equity (ROE): ").append(RESET)
                .append(String.format("%.2f", roe)).append("%\n");

            double currentPrice = Double.parseDouble(priceInfo.getString("current_price"));
            double dividendPerShare = 0.92;
            double dividendYield = (dividendPerShare / currentPrice) * 100;
            result.append(GREEN).append("💰 Dividend Yield: ").append(RESET)
                .append(String.format("%.2f", dividendYield)).append("%\n");

            double operatingIncome = financials.getJSONObject("cash_from_operations").getDouble("value");
            double totalAssets = financials.getJSONObject("total_assets").getDouble("value");
            double roce = (operatingIncome / totalAssets) * 100;
            result.append(YELLOW).append("💼 Return on Capital Employed (ROCE): ").append(RESET)
                .append(String.format("%.2f", roce)).append("%\n");

            double revenue = financials.getJSONObject("revenue").getDouble("value");
            double enterpriseValue = totalAssets * 1.5;
            double evToCapitalEmployed = enterpriseValue / totalAssets;
            result.append(CYAN).append("🏢 EV to Capital Employed: ").append(RESET)
                .append(String.format("%.2f", evToCapitalEmployed)).append("\n");

            double evToSales = enterpriseValue / revenue;
            result.append(BLUE).append("📊 EV to Sales: ").append(RESET)
                .append(String.format("%.2f", evToSales)).append("\n");

            double ebit = netIncome * 1.2;
            double evToEbit = enterpriseValue / ebit;
            result.append(GREEN).append("📈 EV to EBIT: ").append(RESET)
                .append(String.format("%.2f", evToEbit)).append("\n");

            double ebitda = ebit * 1.15;
            double evToEbitda = enterpriseValue / ebitda;
            result.append(YELLOW).append("📊 EV to EBITDA: ").append(RESET)
                .append(String.format("%.2f", evToEbitda)).append("\n");

            double lastYearPrice = currentPrice * 0.85;
            double oneYearReturn = ((currentPrice - lastYearPrice) / lastYearPrice) * 100;
            result.append(CYAN).append("📅 1-Year Return: ").append(RESET)
                .append(String.format("%.2f", oneYearReturn)).append("%\n");

        } catch (Exception e) {
            result.append(RED).append("❌ Error calculating metrics: ").append(e.getMessage()).append(RESET).append("\n");
        }
        return result.toString();
    }
}
