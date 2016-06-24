#include <Radix4.h>
#include <DueTimer.h>
#include <TFT.h>  // Arduino LCD library
#include <SPI.h>

// pin definition for the Due
#define cs   10
#define dc   9
#define rst  8

//Size definition
#define FFTSIZE 256
#define HALF_FFTSIZE 128

TFT TFTscreen = TFT(cs, dc, rst);

volatile    int         numero_muestras = -10;
volatile    int         valorV[FFTSIZE] = {2048, 2218, 2384, 2542, 2688, 2820, 2935, 3032, 3110, 3169, 3210, 3233, 3243, 3240, 3228, 3210, 3189, 3168, 3149, 3134, 3125, 3122, 3126, 3137, 3153, 3173, 3196, 3219, 3242, 3261, 3276, 3286, 3289, 3286, 3276, 3261, 3242, 3219, 3196, 3173, 3153, 3137, 3126, 3122, 3125, 3134, 3149, 3168, 3189, 3210, 3228, 3240, 3243, 3233, 3210, 3169, 3110, 3032, 2935, 2820, 2688, 2542, 2384, 2218, 2048, 1878, 1712, 1554, 1408, 1276, 1161, 1064, 986, 927, 886, 863, 853, 856, 868, 886, 907, 928, 947, 962, 971, 974, 970, 959, 943, 923, 900, 877, 854, 835, 820, 810, 807, 810, 820, 835, 854, 877, 900, 923, 943, 959, 970, 974, 971, 962, 947, 928, 907, 886, 868, 856, 853, 863, 886, 927, 986, 1064, 1161, 1276, 1408, 1554, 1712, 1878, 2048, 2218, 2384, 2542, 2688, 2820, 2935, 3032, 3110, 3169, 3210, 3233, 3243, 3240, 3228, 3210, 3189, 3168, 3149, 3134, 3125, 3122, 3126, 3137, 3153, 3173, 3196, 3219, 3242, 3261, 3276, 3286, 3289, 3286, 3276, 3261, 3242, 3219, 3196, 3173, 3153, 3137, 3126, 3122, 3125, 3134, 3149, 3168, 3189, 3210, 3228, 3240, 3243, 3233, 3210, 3169, 3110, 3032, 2935, 2820, 2688, 2542, 2384, 2218, 2048, 1878, 1712, 1554, 1408, 1276, 1161, 1064, 986, 927, 886, 863, 853, 856, 868, 886, 907, 928, 947, 962, 971, 974, 970, 959, 943, 923, 900, 877, 854, 835, 820, 810, 807, 810, 820, 835, 854, 877, 900, 923, 943, 959, 970, 974, 971, 962, 947, 928, 907, 886, 868, 856, 853, 863, 886, 927, 986, 1064, 1161, 1276, 1408, 1554, 1712, 1878};

volatile    int         valorI[FFTSIZE] = {2048, 2107, 2166, 2224, 2279, 2333, 2384, 2432, 2476, 2517, 2553, 2586, 2614, 2638, 2657, 2673, 2684, 2692, 2697, 2698, 2697, 2694, 2690, 2684, 2677, 2669, 2662, 2655, 2649, 2644, 2641, 2638, 2638, 2638, 2641, 2644, 2649, 2655, 2662, 2669, 2677, 2684, 2690, 2694, 2697, 2698, 2697, 2692, 2684, 2673, 2657, 2638, 2614, 2586, 2553, 2517, 2476, 2432, 2384, 2333, 2279, 2224, 2166, 2107, 2048, 1989, 1930, 1872, 1817, 1763, 1712, 1664, 1620, 1579, 1543, 1510, 1482, 1458, 1439, 1423, 1412, 1404, 1399, 1398, 1399, 1402, 1406, 1412, 1419, 1427, 1434, 1441, 1447, 1452, 1455, 1458, 1458, 1458, 1455, 1452, 1447, 1441, 1434, 1427, 1419, 1412, 1406, 1402, 1399, 1398, 1399, 1404, 1412, 1423, 1439, 1458, 1482, 1510, 1543, 1579, 1620, 1664, 1712, 1763, 1817, 1872, 1930, 1989, 2048, 2107, 2166, 2224, 2279, 2333, 2384, 2432, 2476, 2517, 2553, 2586, 2614, 2638, 2657, 2673, 2684, 2692, 2697, 2698, 2697, 2694, 2690, 2684, 2677, 2669, 2662, 2655, 2649, 2644, 2641, 2638, 2638, 2638, 2641, 2644, 2649, 2655, 2662, 2669, 2677, 2684, 2690, 2694, 2697, 2698, 2697, 2692, 2684, 2673, 2657, 2638, 2614, 2586, 2553, 2517, 2476, 2432, 2384, 2333, 2279, 2224, 2166, 2107, 2048, 1989, 1930, 1872, 1817, 1763, 1712, 1664, 1620, 1579, 1543, 1510, 1482, 1458, 1439, 1423, 1412, 1404, 1399, 1398, 1399, 1402, 1406, 1412, 1419, 1427, 1434, 1441, 1447, 1452, 1455, 1458, 1458, 1458, 1455, 1452, 1447, 1441, 1434, 1427, 1419, 1412, 1406, 1402, 1399, 1398, 1399, 1404, 1412, 1423, 1439, 1458, 1482, 1510, 1543, 1579, 1620, 1664, 1712, 1763, 1817, 1872, 1930, 1989};

float V_mag[HALF_FFTSIZE] = { 0};     // valor absoluto de I
float I_mag[HALF_FFTSIZE] = { 0};     // valor absoluto de I
float IrmsFFTTHD[40] = { 0};
float VrmsFFTTHD[40] = { 0};
int posicion = 0;
int posicion_anterior = 0;

float THDv=0, THDi=0;

volatile bool up = 0;
volatile bool down = 0;
volatile bool ok = 0;
volatile bool salir = 0;
bool VorI = 0;
long T0 = 0 ;

void setup()
{
  Serial.begin (115200);
  TFTscreen.begin();
  TFTscreen.background(0, 0, 0);
  screenmenu(posicion, posicion_anterior);
  Timer3.attachInterrupt(muestreo).setFrequency(6400);
  active_Boton();
}

Radix4     radix;

void loop()
{
  if (up == 1) {
    up = 0;
    if (posicion == 0) {
      posicion = 5;
    } else {
      posicion--;
    }
    posicion_anterior = 1;
    screenmenu(posicion, posicion_anterior);
  } else if (down == 1) {
    down = 0;
    if (posicion == 5) {
      posicion = 0;
    } else {
      posicion++;
    }
    posicion_anterior = 0;
    screenmenu(posicion, posicion_anterior);
  } else if (ok == 1) {
    ok = 0;
    functionmenu(posicion);
  }

}

void muestreo(){
  ADC->ADC_CR |= 0x02;                       //start ADC
  while ((ADC->ADC_ISR & 0xC0) == 0x00);
  if (numero_muestras >= 0 && numero_muestras < FFTSIZE) {
    valorV[numero_muestras] = ADC->ADC_CDR[7]; //Cuidado, corresponde con el pin A0??
    valorI[numero_muestras] = ADC->ADC_CDR[6]; //Pin A1*/
  }

  numero_muestras++;
}

void BotonUp()
{
  if ( millis() > T0  + 250)
  {
    up = 1;
    T0 = millis();
  }
}

void BotonDown()
{
  if ( millis() > T0  + 250)
  {
    down = 1;
    T0 = millis();
  }
}

void BotonOk()
{
  if ( millis() > T0  + 250)
  {
    ok = 1;
    T0 = millis();
  }
}

void BotonExit()
{
  if ( millis() > T0  + 250)
  { 
    salir = 1;
    T0 = millis();
  }
}

void THD(float *mag, float *RMS_arm, float &THDx) {
  //Reset accumulators
  float sum = 0;
  float cuadrado=0;
  
  RMS_arm[1] = mag[2] / sqrt(2);

  for (int i = 2; i <= 40; i++) {                   //Poner 40 como  define
    RMS_arm[i] = mag[2*i]/sqrt(2);
    cuadrado = RMS_arm[i] * RMS_arm[i];
    sum += cuadrado;
  }

  THDx = (sqrt(sum) / RMS_arm [1]) * 100;

}

void screenmenu (int posicion, int arriba) {
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.setTextSize(2);

  switch (posicion) {
    case 0:
      if (arriba == 1) {
        TFTscreen.text("A>B", 0, 0);
      } else {
        TFTscreen.text("C>D", 0, 0);
      }
      TFTscreen.stroke(255, 255, 255);
      TFTscreen.text("ABCD", 0, 0);
      break;
    case 1:
      if (arriba == 1) {
        TFTscreen.text("A>B", 0, 0);
      } else {
        TFTscreen.text(">A B", 0, 0);
      }
      TFTscreen.stroke(255, 255, 255);
      TFTscreen.text("A>B", 0, 0);
      break;
    case 2:
      if (arriba == 1) {
        TFTscreen.text("CD", 0, 0);
      } else {
        TFTscreen.text("AP", 0, 0);
      }
      TFTscreen.stroke(255, 255, 255);
      TFTscreen.text("F", 0, 0);
      break;
    case 3:
      if (arriba == 1) {
        TFTscreen.text("G", 0, 0);
      } else {
        TFTscreen.text("H", 0, 0);
      }
      TFTscreen.stroke(255, 255, 255);
      TFTscreen.text("P", 0, 0);
      break;
    case 4:
      if (arriba == 1) {
        TFTscreen.text("K", 0, 0);
      } else {
        TFTscreen.text("M", 0, 0);
      }
      TFTscreen.stroke(255, 255, 255);
      TFTscreen.text("N", 0, 0);
      break;
    case 5:
      if (arriba == 1) {
        TFTscreen.text("U", 0, 0);
      } else {
        TFTscreen.text("I", 0, 0);
      }
      TFTscreen.stroke(255, 255, 255);
      TFTscreen.text("case5", 0, 0);
      break;
  }
}

void VorI_menu(bool VorI) {
  TFTscreen.stroke(0, 0, 0);

  switch (VorI) {
    case 0:
      TFTscreen.text("L", 0, 0);
      TFTscreen.stroke(255, 255, 255);
      TFTscreen.text(">L", 0, 0);
      break;
    case 1:
      TFTscreen.text("D", 0, 0);
      TFTscreen.stroke(255, 255, 255);
      TFTscreen.text(">D", 0, 0);
      break;
  }
}

void functionmenu (int posicion) {
  salir = 0;
  VorI = 0;
  bool salir2 = 0;
  TFTscreen.background(0, 0, 0);
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.setTextSize(1);
  switch (posicion) {
    case 0:                                         // Forma de onda. Dominio tiempo.
      break;
    case 1:                                     //Espectrometro
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      TFTscreen.setTextSize(2);
      TFTscreen.text("Armonicos", 0, 0);
      VorI_menu(VorI);
      while (!salir && !ok) {
        if (up || down) {
          VorI = !VorI;
          VorI_menu(VorI);
          up = 0;
          down = 0;
        }
      }
      if (salir) {
        salir2 = 1;
      } else {
        TFTscreen.background(0, 0, 0);
        TFTscreen.setTextSize(1);
        switch (VorI) {
          case 0:
            //screen_armonicos("Armonicos   THDv=", "V  =       V  V  =       V");
            break;
          case 1:
            //screen_armonicos("Armonicos   THDi=", "I  =       A  I  =       A");
            break;
        }
        desactive_Boton();
        Timer3.start();
      }

      while (!salir) {
        if (numero_muestras >= FFTSIZE) {
          Timer3.stop();
          active_Boton();
          //time_start = micros();
          numero_muestras = -10;
          //calc_offset(); 
          switch (VorI) {
            case 0:
              //calc_FFT(Vf_r, V_mag, V_ang, V_RATIO, FASE_RATIO);
              //Serial.println("case V");
              //Serial.println(ok);
              THD(V_mag, VrmsFFTTHD, THDv);     //ok pasa a 0 sin motivo cada vez que entra
              //Serial.println(ok);
              //screen_variables_armonicos(THDv, VrmsFFTTHD);
              break;
            case 1:
              //Serial.println("case I: VorI:");
              //Serial.println(VorI);
              //calc_FFT(If_r, I_mag, I_ang, I_RATIO, FASE_RATIO);
              THD(I_mag, IrmsFFTTHD, THDi);
              //Serial.println(VorI);
              //screen_variables_armonicos(THDi, IrmsFFTTHD);
              break;
          }
          //time_calc  = micros() - time_start;
          if (!salir) {
            desactive_Boton();
            Timer3.start();
          }
        }
      }
      //Serial.print("\tArmonicos: ");
      ////Serial.println(time_calc);
      TFTscreen.background(0, 0, 0);
      break;

  }
  Timer3.stop();
  active_Boton();

  numero_muestras = -10;
  salir = 0;
  up = 0;
  down = 0;
  if (posicion == 2 || posicion == 3 || posicion == 4 || salir2) {
    screenmenu(posicion, posicion_anterior);
    ok = 0;
  }

}

void active_Boton(){
  attachInterrupt( digitalPinToInterrupt(2), BotonDown, RISING);
  attachInterrupt( digitalPinToInterrupt(3), BotonOk, RISING);
  attachInterrupt( digitalPinToInterrupt(4), BotonUp, RISING);
  attachInterrupt( digitalPinToInterrupt(5), BotonExit, RISING);
}

void desactive_Boton(){
  detachInterrupt( digitalPinToInterrupt(2));
  detachInterrupt( digitalPinToInterrupt(3));
  detachInterrupt( digitalPinToInterrupt(4));
  detachInterrupt( digitalPinToInterrupt(5));
}
