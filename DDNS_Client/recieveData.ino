void recieveData(){
  if (ELECHOUSE_cc1101.CheckRxFifo(100)){
    //CRC Check. If "setCrc(false)" crc returns always OK!
    if (ELECHOUSE_cc1101.CheckCRC()){ 
      //Get received Data and calculate length
      int len = ELECHOUSE_cc1101.ReceiveData(buffer);
      buffer[len] = '\0';
      signal_strenght = ELECHOUSE_cc1101.getRssi();
      //Print received in char format.
      station1_data = (char *) buffer;
      Serial.println((char *) buffer);
      time(&now1);
    }
  }
}
