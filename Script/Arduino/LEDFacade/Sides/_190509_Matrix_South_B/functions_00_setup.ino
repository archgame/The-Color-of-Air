//WIFI CONNECT
void connect(const char* subscription_01, const char* subscription_02, const char* subscription_03, const char* subscription_04, const char* subscription_05, const char* subscription_06, const char* subscription_07,
                const char* subscription_08, const char* subscription_09, const char* subscription_10, const char* subscription_11, const char* subscription_12, const char* subscription_13, const char* subscription_14) {
  Serial.print("\nchecking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
  Serial.print(".");
  delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("South_B_MKR1000_LED", "2ffedeff", "4ca8d8c1141ec18c")) { // <-- CHANGE NI to your initials
  Serial.print(".");
  delay(1000);
  }

  Serial.print("\nconnected!");

  client.subscribe(subscription_01);
  client.subscribe(subscription_02);
  client.subscribe(subscription_03);
  client.subscribe(subscription_04);
  client.subscribe(subscription_05);
  client.subscribe(subscription_06);
  client.subscribe(subscription_07);
  client.subscribe(subscription_08);
  client.subscribe(subscription_09);
  client.subscribe(subscription_10);
  client.subscribe(subscription_11);
  client.subscribe(subscription_12);
  client.subscribe(subscription_13);
  client.subscribe(subscription_14);
}
