void updateStation(){
  byte transmitt_byte[7] = {0,0,0,0,0,0,0};
  if(switch_){
    transmitt_byte[2] = 1;
  }else{
    transmitt_byte[2] = 2;
  }
  ELECHOUSE_cc1101.SetTx();
  ELECHOUSE_cc1101.SendData(transmitt_byte, 7, (byte)2);
  delay(100);

  String r_s = obj2["r"];
  String g_s = obj2["g"];
  String b_s = obj2["b"];
  int r_i = r_s.toInt();
  int g_i = g_s.toInt();
  int b_i = b_s.toInt();
  
  transmitt_byte[2] = (byte)switch_;
  transmitt_byte[3] = (byte)r_i;
  transmitt_byte[4] = (byte)g_i;
  transmitt_byte[5] = (byte)b_i;
  
  ELECHOUSE_cc1101.SendData(transmitt_byte, 7, (byte)4);

  
}
