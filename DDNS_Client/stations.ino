void updateStation(){
  byte transmitt_byte[5] = {0,0,0,0,0};
  if(switch_){
    transmitt_byte[2] = 1;
  }else{
    transmitt_byte[2] = 2;
  }
  ELECHOUSE_cc1101.SetTx();
  ELECHOUSE_cc1101.SendData(transmitt_byte, 5, (byte)2);
}
