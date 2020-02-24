void humidcolor (float humidDT, uint8_t windcolorT[3], uint8_t humidcolorT[3]) {
/*    if (humidDT <= (.05*maxhD)) {                                                 //test if interior humidity is dryer
    Serial.print(", humidity dryer");
    humidDT = .05;
    int j;
    for (j = 0; j < 3; j++) {
      humidcolorT[j] = (uint8_t)(humidDT * humidcolorT[j]);
    }
  }
  else                                                                  //test if interior humidity is wetter
  {
    if (humidDT > maxhD) {
      Serial.print(", humidity max");
      humidDT = 1;
      int k;
      for (k = 0; k < 3; k++) {
      humidcolorT[k] = (uint8_t)(humidDT * humidcolorT[k]);
    }
    }
    Serial.print(", humidity wetter");                                  
    humid_percentage = humidDT / maxhD;
    int l;
    for (l = 0; l < 3; l++){
    humidcolorT[l] = (uint8_t)(humid_percentage * humidcolorT[l]);
    }
  }
}*/
if (humidDT < 50)
  {
    humidDT = 50;
  }


else 
{
  if (humidDT > 100){                                           

      humidDT = 100;                                             //keep the distance within the maximum difference range
  }
}
    humid_percentage = (humidDT / 100);              //scale the temperature value based on maximum difference and 100
     int k;
      for (k = 0; k < 3; k++) {
    humidcolorT[k]=(uint8_t)(humid_percentage * windcolorT[k]);   //lerp color
      }
}
