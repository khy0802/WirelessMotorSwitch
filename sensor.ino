

int active_sensor(){
  int err;
  float h, t;
  long m = millis();
  Serial.println("Reading...");
  if((err=dht11.read(h, t))==0) 
  {
    temp = t;
    humi = h;
  }
  dht.readHumidity();
  Serial.println("Complete");
  long a = millis();
  return a - m;
}
