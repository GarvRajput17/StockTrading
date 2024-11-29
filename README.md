# Stock Go!

## Overview

Stock Go is a C++ and Java based CLI-Based application for buying and selling stocks. Integrated with the Google Web Scraper API's, it ensures the persistency of the Realtime stock data is available to the users.

## Features

- User registration and authentication through Firebase 
- Wallet management (add/remove funds) 
- Stock buying and selling
- Portfolio management
- Watchlist functionality
- Transaction history tracking
- Company fundamentals analysis
- Latest news and trends around the stock Market
- Experts predictions based on the Stcok Performance and the company fundamentals.

## Requirements

- C++ Compiler (e.g., g++)
- Java Development Kit (JDK 8 or higher)
- openssl Library
- curl Library (c++)
- [nlohmann/json](https://github.com/nlohmann/json) library for JSON manipulation
- [JNI](https://docs.oracle.com/javase/8/docs/technotes/guides/jni/) for interfacing Java and C++

## Installation

### Clone the Repository

```bash
git clone https://github.com/username/stock-trading-system.git](https://github.com/GarvRajput17/StockTrading/garv)
```

### Set Up Java Environment

1. Ensure you have the JDK installed and configured in your system's PATH.
2. Ensure that JNI is set up and include Path for the IDE is updated for the project to build.

### C++ Libraries

1. Ensure that openssl / curl / nlohmann-json have been installed and include path have been setup correctly.

### Firebase Initialisation

1. Setting up firebase account is cruicial for the porject to run.
2. Install the firebase cli tools using the command:

   For Mac Users:

   ```bash
   brew install firebase cli
   ```

   For Windows Users:

   ```bash
   npm install -g firebase-tools
   ```

3. Login into your firebase account and initialise it using the command:
   
   ```bash
   firebase init
   ```
   
4. Ensure to configure the firebase.json / firestore database indexes/ database.rules.json/ firestore.indexes.json/ fierestore.rules
5. Then create a firebase_config.h file in the Firebase_files folder and configure it as given below:
   
    #ifndef FIREBASE_CONFIG_H
    #define FIREBASE_CONFIG_H
    #define FIREBASE_API_KEY ""
    #define FIREBASE_PROJECT_ID ""
    /*
    const char* FIREBASE_CONFIG = R"({
        "projectId": "",
        "appId": "",
        "databaseURL": "",
        "storageBucket": "",
        "apiKey": "",
        "authDomain": "",
        "messagingSenderId": "",
        "measurementId": ""
    })";
    
    */
    #endif

   Ensure to fill the given details from tyour firebase console.


### Start the Application

```bash
java -cp .:lib/json-20240303.jar Main
```

### Access the Application

1. Create a MAKEFILE for configuring the java file (Main.java)
2. Build the program using the command "make".
3. Run the pogram using the command "make run".

## Usage

### User Registration

- Choose the registration option and provide a username and password.

### User Login

- Log in using your credentials after registering.

### Adding Money to Wallet

- Select the option to add funds and choose a payment method.

### Buying Stocks

- Enter the stock symbol and quantity you wish to purchase.

### Selling Stocks

- Provide the stock symbol and quantity to sell from your portfolio.

### Viewing Portfolio

- Check your current stock holdings and their total value.

### Transaction History

- View a list of all transactions made, including date, amount, and type.

## Testing
Testing was done for checking the persistence by logging in the same user on two different terminals at the same time and check if the data is accessed only once at a time. Database was tested heavily by performing CRUD operations on a same set of data points from multiple terminals at the same time. 
