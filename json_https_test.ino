#include "WiFi.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"
#include "secrets.h"

void setup()
{
  Serial.begin(115200);
  
  delay(5000);

  Serial.println("Connecting to Wi-Fi");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(2000);
    Serial.println(".");
  }

  Serial.println("\nFetching JSON...");

  int httpResponseCode;
  HTTPClient https;
  String response = String();

  https.begin(GITHUB_JSON_URL, HTTPS_PORT, GITHUB_JSON_PATH, rootCaCert);
  httpResponseCode = https.GET();

  Serial.print("HTTP RESPONSE: ");
  Serial.println(httpResponseCode);
  if (httpResponseCode > 0)
  {
    response = https.getString();
    // Serial.println("PAYLOAD:");
    // Serial.println(response);
  }
  else
  {
    Serial.println("Error " + String(httpResponseCode) + String(" : ") + https.errorToString(httpResponseCode));
    https.end();
    return;
  }
  https.end();

  JsonDocument recvDoc;

  DeserializationError error = deserializeJson(recvDoc, response);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  int count = 0;
  String repoName;
  
  Serial.println("Got repo list:");

  for (JsonObject repo : recvDoc.as<JsonArray>())
  {
    repoName = repo["name"].as<String>();
      Serial.printf("\t%d. %s\n", ++count, repoName.c_str());
  }
}

void loop()
{
}