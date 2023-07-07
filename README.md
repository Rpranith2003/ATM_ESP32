# ATM System

This Arduino sketch implements an ATM system using an ESP32 microcontroller and integrates it with the Telegram messaging platform. The system allows users to create new accounts, access existing accounts, perform credit and debit transactions, view mini statements, and check available balances.

# Configuration

1. The Wi-Fi credentials were set by modifying the 'ssid' and 'password' variables to match the network settings.
2. The 'BOTtoken' and 'CHAT_ID' variables were updated with the Telegram bot token and chat ID. This enabled the ESP32 to interact with the Telegram bot.
3. The 'GOOGLE_SCRIPT_ID' variable was modified to match the Google Sheets script ID used for data storage.


# Functionality

1. Account Creation: Users can create a new account by providing a username and password. The account details are stored in a Google Sheets spreadsheet using a custom Google Apps Script.
2. Account Access: Users can access their existing accounts by entering their user ID and password. The sketch validates the credentials by comparing them with the stored data in the Google Sheets spreadsheet.
3. Credit Transactions: Users can perform credit transactions by entering the desired amount to be added to their account balance. The sketch updates the account balance in the Google Sheets spreadsheet and sends a confirmation message via Telegram.
4. Debit Transactions: Users can perform debit transactions by entering the desired amount to be deducted from their account balance. The sketch verifies the availability of sufficient funds, updates the account balance in the Google Sheets spreadsheet, and sends a confirmation message via Telegram.
5. Mini Statements: Users can request a mini statement that displays their transaction history. The sketch retrieves the transaction history from the Google Sheets spreadsheet and sends it as a message via Telegram.
6. Balance Inquiry: Users can check their available account balance. The sketch retrieves the balance from the Google Sheets spreadsheet and sends it as a message via Telegram.

# Description
To start, we need to connect the ESP32 to Wi-Fi. I've created a function called `initWiFi()` for this purpose. For any operations involving the spreadsheet (creating, reading, or updating), I've developed three functions. Within these functions, I make API calls and pass the relevant parameters in the API parameter section. Additionally, I've written code in the Google Apps Script to handle the API calls, extracting the parameters and performing the necessary read, write, or update actions based on the command parameter in the API.

After completing the operations in the Google Sheets spreadsheet, we need to notify the user. For this, I've utilised a Telegram bot and the 'bot.sendMessage()' function. 

Here are the steps to set up the system:

1. Create a Google spreadsheet and provide its script ID to the ESP32.
2.  Write the Google Apps Script code in the app script. This code is executed when an API is called on the Google Apps Script servers. In the app script, we extract the parameters from the API call and perform the required operations on the spreadsheet, referring to the API documentation. This is a critical step.
3.  Create a Telegram bot using BOTFATHER and provide its bot token to the ESP32. Also, provide our CHAT_ID to the ESP32, enabling it to send messages to us.
4.  Develop three functions for reading, writing, and updating Google Sheets. The read function retrieves the value of a cell in the specified row or column. The write function adds a new user to the ATM system and returns the UserID of the newly created user. The update function modifies the cell value in the specified row and column, typically used to update the user's balance after each transaction.
5.  With all the requirements fulfilled for the main loop function, we can proceed to write it. Inside the loop, we'll implement a control flow using if-else statements to handle different user inputs.
6.  By following these steps, I've successfully created our ATM system.
