// Header Files have been included
#include <iostream>
using namespace std;
#include <vector>
#include <string>
#include "WiFi.h"
#include <HTTPClient.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
// Bot token and chat_id for telegram bot integration with ESP32
#define BOTtoken "5797324424:AAF9P39imuTlBLvRlo3uYUqLkgp027BJbDk"
#define CHAT_ID "*********"
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
int botRequestDelay = 500;
unsigned long lastTimeBotRan = 0;
// WiFi Credentials for ESP32
const char* ssid = "****;
const char* password = "****";
String userID;
String GOOGLE_SCRIPT_ID = "AKfycbxn_Fo_S9bxccs0Y93IsVXvJ_k1m8TgOLGX8cibL5ic1jfPbw1TYa8Mendfe-6x0AX";
//Function to initialize and connect to the wifi specified above
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.print("Connected Successfully :");
}
// here Gs means Google Sheets
// This is a function to read the data in a particular cell of the google sheet
// Row number and column number of the cell whose data is to be read is passed as parameter of
the function
  // This function will return the concerned data in String format.
  String
  readGs(int row, char col) {
  if (WiFi.status() == WL_CONNECTED) {
    String
      urlFinal = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?row=" + row + "&co
                 l = "+col+" & command = read ";
      HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpcode = http.GET();
    String data = http.getString();
    if (httpcode != 200) {
      return "Error";
    } else {
      return data;
    }
    http.end();
  } else {
    return "Error";
  }
}
// This is a function to write the data of newly created user in the google sheet
// Username and Password of newly created user is passed as parameter of the function
// This function will return the UserID of newly created user in String format.
String writeGs(String newUsername, String newPassword) {
  if (WiFi.status() == WL_CONNECTED) {
    String
      urlFinal = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?username=" + new Username + "&password=" + newPassword + "&command=write";
    HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpcode = http.GET();
    String data = http.getString();
    if (httpcode != 200) {
      Serial.println("Error");
      return "Error";
    } else {
      Serial.println("Success");
      return data;
    }
    http.end();
  }
}
// This is a function to update the balance of user who made a transaction in the google sheet
// row and column number of cell to be updated is passed as parameter of the function
String updateGs(int row, char col, String value) {
  if (WiFi.status() == WL_CONNECTED) {
    String
      urlFinal = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?row=" + row + "&co
                 l = "+col+" & command = update& value = "+value;
      HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpcode = http.GET();
    if (httpcode != 200) {
      Serial.println("Error");
    } else {
      String oldHistory = http.getString();
      Serial.println("Success");
      return oldHistory;
    }
    http.end();
  }
}
// This function is used to update the transaction history of a user after doing a transaction.
void updateHistory(int row, char col, String value) {
  if (WiFi.status() == WL_CONNECTED) {
    String
      urlFinal = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?row=" + row + "&co
                 l = "+col+" & command = updateHistory& value = "+value;
      HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpcode = http.GET();

    if (httpcode != 200) {
      Serial.println("Error");
    } else {
      Serial.println("Success");
    }
    http.end();
  }
}
vector<String> handleNewMessages(int numNewMessages) {
  // char* inputs[numNewMessages];
  vector<String> inputs;
  for (int i = 0; i < numNewMessages; i++) {
    String text = bot.messages[i].text;
    // inputs[i]=text;
    inputs.push_back(text);
  }
  return inputs;
}
// This function is used to take one by one input from user using telegram bot.
vector<String> take_input(int n) {
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  while (numNewMessages != n) {
    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  }
  // char* inputs[numNewMessages];
  vector<String> inputs;
  Serial.println("got response");
  inputs = handleNewMessages(numNewMessages);
  numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  return inputs;
}
void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  Serial.begin(9600);
  initWiFi();
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
}
void loop() {
  // Bot starts here
  vector<String> inputs;
  String welcome = "Welcome To iATM 💰.\n";
  welcome += "Send 1 to create a new account. �\n";
  welcome += "Send 2 to access an existing account. 💳\n";
  bot.sendMessage(CHAT_ID, welcome, "");
  inputs = take_input(1);
  if (inputs[0] != "1" && inputs[0] != "2" && inputs[0] != "3") {
    // User has pressed some invalid Key error handling.
    bot.sendMessage(CHAT_ID, "Invalid Input �\nRedirecting.......", "");
  } else {
    if (inputs[0] == "1") {
      // Creating new account
      String newUsername, newPassword;
      bot.sendMessage(CHAT_ID, "Enter Username 👤", "");
      inputs = take_input(1);
      newUsername = inputs[0];
      bot.sendMessage(CHAT_ID, "Enter new password 🔑", "");
      inputs = take_input(1);
      newPassword = inputs[0];
      userID = writeGs(newUsername, newPassword);
      if (userID != "Error") {
 bot.sendMessage(CHAT_ID,"Your Account has been created successfully
�\n\n","");
 bot.sendMessage(CHAT_ID,"Kindly keep your userID safe.\n","");
 bot.sendMessage(CHAT_ID,"Your UserID is :"+userID,"");
      } else {
 bot.sendMessage(CHAT_ID,"Your Account cannot be created due to some technical
failure .�\n\n","");
      }
    } else if (inputs[0] == "2") {
      // debit or credit transaction
      String UserID, password;
      bot.sendMessage(CHAT_ID, "Please Enter your UserID 👤 :", "");
      inputs = take_input(1);
      UserID = inputs[0];
      bot.sendMessage(CHAT_ID, "Please Enter your Password 🔑:", "");
      inputs = take_input(1);
      password = inputs[0];
      String realPassword;
      realPassword = readGs(UserID.toInt() + 1, 'C');
      if (realPassword != password) {
        bot.sendMessage(CHAT_ID, "Wrong Password �", "");
      } else {
        bot.sendMessage(CHAT_ID, "Password Correct �", "");
        while (true) {
          String action;
          String amount;
          int amountToInt;
          int presentAmt;
 bot.sendMessage(CHAT_ID,"For Credit send 1\nFor Debit send 2\nTo See Mini
Statement send 3\nTo see your available balance send 4\nTo Log Out send 0","");
 inputs=take_input(1);
 action=inputs[0];
 if(action=="0"){
            break;
 }
 String history="";
 presentAmt=readGs(UserID.toInt()+1,'D').toInt();
 if(action!="3" && action!="4"){
            bot.sendMessage(CHAT_ID, "Enter Amount :💰", "");
            inputs = take_input(1);
            amount = inputs[0];
            history = amount;
            amountToInt = amount.toInt();
 }
 if(action=="1"){
            presentAmt = presentAmt + amountToInt;
            amount = String(presentAmt);
            bot.sendMessage(CHAT_ID, "Transaction Successfull �", "");
            String oldHistory = updateGs(UserID.toInt() + 1, 'D', amount);
            oldHistory += ",+";
            oldHistory += history;
            updateHistory(UserID.toInt() + 1, 'E', oldHistory);
            bot.sendMessage(CHAT_ID, "Updated Balance :", "");
            bot.sendMessage(CHAT_ID, amount, "");
 }else if(action=="2"){
            if (presentAmt < amountToInt) {
              bot.sendMessage(CHAT_ID, "Insufficient Balance ‼️ ‼️", "");
              bot.sendMessage(CHAT_ID, "Updated Balance :", "");
              bot.sendMessage(CHAT_ID, String(presentAmt), "");
            } else {
              presentAmt = presentAmt - amountToInt;
              bot.sendMessage(CHAT_ID, "Transaction Successfull �", "");
              String oldHistory = updateGs(UserID.toInt() + 1, 'D', String(presentAmt));
              oldHistory += ",-";
              oldHistory += history;
              updateHistory(UserID.toInt() + 1, 'E', oldHistory);
              bot.sendMessage(CHAT_ID, "Updated Balance :", "");
              bot.sendMessage(CHAT_ID, String(presentAmt), "");
            }
 }else if(action=="3"){
            String miniStatement = readGs(UserID.toInt() + 1, 'E');
            String miniMsg = "";
            for (int i = 0; i < miniStatement.length(); i++) {
              if (miniStatement.charAt(i) == ',') {
                miniMsg = miniMsg + " Rs ";
                miniMsg = miniMsg + "\n";
              } else if (miniStatement.charAt(i) == ' ') {
                miniMsg = miniMsg + "Credited ";
              } else if (miniStatement.charAt(i) == '-') {
                miniMsg = miniMsg + "Debited ";
              } else {
                miniMsg = miniMsg + miniStatement.charAt(i);
              }
            }
            bot.sendMessage(CHAT_ID, miniMsg, "");
 }else if(action=="4"){
            String userAvailableBalance = readGs(UserID.toInt() + 1, 'D');
            userAvailableBalance = userAvailableBalance + " Rs.";
 bot.sendMessage(CHAT_ID,"Your available balance is :
"+userAvailableBalance,"");
 }else{
            bot.sendMessage(CHAT_ID, "Invalid Action 🚫", "");
 }
        }
      }
    } 
  }
}
