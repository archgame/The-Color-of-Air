void temperaturecolor (float temperatureDT, uint8_t temperaturecolorT[3]) {
   // client.publish(SHIFTR_MKR_DT, String(temperatureDT));
  if (temperatureDT < 0) {                                               //test if interior temperature is cooler
//    Serial.print(", cooler inside");
    temperatureDT = abs(temperatureDT);                                   //invert the negative value
    if (temperatureDT > maxtD)                                           
    {
      temperatureDT = maxtD;                                             //keep the distance within the maximum difference range
    }                                                                   
    temperature_percentage = (temperatureDT / maxtD) * 100;              //scale the temperature value based on maximum difference and 100
    twocolorfade(temperature_percentage, colorMin, colorMid, temperaturecolorT);   //lerp colors
  }
  else                                                                  //test if interior temperature is warmer
  {
//    Serial.print(", warmer inside");
    if (temperatureDT > maxtD)                                           
    {
      temperatureDT = maxtD;                                             //keep the distance within the maximum difference range
    }
    temperature_percentage = (temperatureDT / maxtD) * 100;              //scale the temperature value based on maximum difference and 100
    twocolorfade(temperature_percentage, colorMax, colorMid, temperaturecolorT);   //lerp colors
  }
    
}
