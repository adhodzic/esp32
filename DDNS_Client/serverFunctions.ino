void serverFunctions(){
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", String(), false);
  });
  
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest * request) {
    doc["temp"] = dht.readTemperature();
    doc["hum"] = dht.readHumidity();
    doc["light"] = analogRead(LIGHT);
    doc["station1_signal"] = signal_strenght;
    doc["station1_data"] = station1_data;
    doc["switch_state"] = switch_; 
    String output;
    serializeJson(doc, output);
    Serial.println(output);
    request->send(200, "application/json", output);
  });

  server.on("/data_graph", HTTP_GET, [](AsyncWebServerRequest * request) {
    String output = "";
    for(int i = 0; i < 1440;i++){
      if(y_room[i] != 255){
        output += "{\"x\":\""+x[i]+"\", "+"\"y_room\":"+y_room[i]+", \"y_outside\":"+y_outside[i]+"}\n";
      }
    }
    output += "{\"x\":\""+x[1439]+"\", "+"\"y_room\":"+y_room[1439]+", \"y_outside\":"+y_outside[1439]+"}\n";
    output = output.substring(0, output.length() - 1);
    //Serial.println(output);
    //Serial.println("-----------------------------------------------------");
    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", output);
    request->send(response);
  });

  server.on("/data", HTTP_POST, [](AsyncWebServerRequest * request) {
    int params = request->params();
    for (int i = 0; i < params; i++) {
      AsyncWebParameter* p = request->getParam(i);
      if (p->isFile()) { //p->isPost() is also true
        //Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
      } else if (p->isPost()) {
        //Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
      } else {
        //Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
        String dat = p->value().c_str();
        if (dat == "true") {
          switch_ = true;
          //Serial.println(switch_);
        } else if (dat == "false") {
          switch_ = false;
        }
        if (switch_) {
          digitalWrite(33, HIGH);
        } else {
          digitalWrite(33, LOW);
        }
        
        
        //Serial.println(switch_);
      }
    } // for(int i=0;i<params;i++)
    request -> send(200);
  });
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  server.begin();
}
