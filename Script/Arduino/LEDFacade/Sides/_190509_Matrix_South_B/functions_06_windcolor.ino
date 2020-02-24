void windcolor (float windDT, uint8_t colorTa[3], uint8_t windcolorT[3]) {
/*    if (windDT <= (.05*maxwD)) {                                                 //test if interior humidity is dryer
//    Serial.print(", humidity dryer");
    windDT = .05;
    int j;
    for (j = 0; j < 3; j++) {
      windcolorT[j] = (uint8_t)(windDT * windcolorT[j]);
    }
  }
  else                                                                  //test if interior humidity is wetter
  {
    if (windDT > maxwD) {
//      Serial.print(", humidity max");
      windDT = 1;
      int k;
      for (k = 0; k < 3; k++) {
      windcolorT[k] = (uint8_t)(windDT * windcolorT[k]);
    }
    }
//    Serial.print(", humidity wetter");                                  
    wind_percentage = windDT / maxwD;
    int l;
    for (l = 0; l < 3; l++){
    windcolorT[l] = (uint8_t)(wind_percentage * windcolorT[l]);
    }
  }
}*/

if (windDT > maxwD)                                           
    {
      windDT = maxwD;                                             //keep the distance within the maximum difference range
    }
    wind_percentage = (windDT / maxwD) *100;              //scale the temperature value based on maximum difference and 100
    twocolorfade(wind_percentage, colorWindMax, colorTa, windcolorT);   //lerp colors
  }
