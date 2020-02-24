//FADE COLORS
//index 0-100
void twocolorfade(uint16_t index, uint8_t color1[3], uint8_t color2[3], uint8_t color[3])
{
  
  uint16_t r_temp, g_temp, b_temp;
  uint8_t index_inverse = 100-index;

  r_temp = ((color1[0] * index)+(color2[0] * index_inverse))/100;
  g_temp = ((color1[1] * index)+(color2[1] * index_inverse))/100;
  b_temp = ((color1[2] * index)+(color2[2] * index_inverse))/100;
  
  color[0]  = (uint8_t)r_temp;
  color[1]  = (uint8_t)g_temp;
  color[2] = (uint8_t)b_temp;
  
}

//fades between a bottom left, top left, bottom right, and top right color
void fourcolorfade(uint16_t indexX, uint16_t indexY, uint8_t colorTR[3], uint8_t colorBR[3], uint8_t colorTL[3], uint8_t colorBL[3], uint8_t color[3])
{

  uint8_t fadecolorL[3];
  uint8_t fadecolorR[3];
  uint8_t fadecolorY[3];
  
  twocolorfade(indexY, colorBL, colorTL, fadecolorL);
  twocolorfade(indexY, colorBR, colorTR, fadecolorR);
  twocolorfade(indexX, fadecolorL, fadecolorR, fadecolorY);
  
  color[0]  = fadecolorY[0];
  color[1]  = fadecolorY[1];
  color[2] = fadecolorY[2];
  
}
