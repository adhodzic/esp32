void updateRecord(){
    for(int i=0;i < 1440;i++){
        x[i] = x[i+1];
        y_room[i] = y_room[i+1];
        //y_outside[i] = y_outside[i+1];
    }
    //for(int i=0;i < 1440;i++){
    //    y_outside[i] = y_outside[i+1];
    //}
    addRecord(1439);
    //time(&start1);
}
void addRecord(int i){
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  char timeHour[30];
  strftime(timeHour,30, "%H:%M:%S", &timeinfo);
  long temp = dht.readTemperature();
  while(temp < -10 || temp > 100){
    temp = dht.readTemperature();
  }
  x[i] = timeHour;
  y_room[i] = temp;
  Serial.print("STATION:");
  Serial.println(station1_data);
  y_outside[i] = station1_data;
}
