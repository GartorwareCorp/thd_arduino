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
#define PRIMEROS40_ARM 40

TFT TFTscreen = TFT(cs, dc, rst);


volatile    int         numero_muestras = -10;
//volatile    int         valorV[FFTSIZE] = { 0};
volatile    int         valorV[FFTSIZE] = {2048, 2218, 2384, 2542, 2688, 2820, 2935, 3032, 3110, 3169, 3210, 3233, 3243, 3240, 3228, 3210, 3189, 3168, 3149, 3134, 3125, 3122, 3126, 3137, 3153, 3173, 3196, 3219, 3242, 3261, 3276, 3286, 3289, 3286, 3276, 3261, 3242, 3219, 3196, 3173, 3153, 3137, 3126, 3122, 3125, 3134, 3149, 3168, 3189, 3210, 3228, 3240, 3243, 3233, 3210, 3169, 3110, 3032, 2935, 2820, 2688, 2542, 2384, 2218, 2048, 1878, 1712, 1554, 1408, 1276, 1161, 1064, 986, 927, 886, 863, 853, 856, 868, 886, 907, 928, 947, 962, 971, 974, 970, 959, 943, 923, 900, 877, 854, 835, 820, 810, 807, 810, 820, 835, 854, 877, 900, 923, 943, 959, 970, 974, 971, 962, 947, 928, 907, 886, 868, 856, 853, 863, 886, 927, 986, 1064, 1161, 1276, 1408, 1554, 1712, 1878, 2048, 2218, 2384, 2542, 2688, 2820, 2935, 3032, 3110, 3169, 3210, 3233, 3243, 3240, 3228, 3210, 3189, 3168, 3149, 3134, 3125, 3122, 3126, 3137, 3153, 3173, 3196, 3219, 3242, 3261, 3276, 3286, 3289, 3286, 3276, 3261, 3242, 3219, 3196, 3173, 3153, 3137, 3126, 3122, 3125, 3134, 3149, 3168, 3189, 3210, 3228, 3240, 3243, 3233, 3210, 3169, 3110, 3032, 2935, 2820, 2688, 2542, 2384, 2218, 2048, 1878, 1712, 1554, 1408, 1276, 1161, 1064, 986, 927, 886, 863, 853, 856, 868, 886, 907, 928, 947, 962, 971, 974, 970, 959, 943, 923, 900, 877, 854, 835, 820, 810, 807, 810, 820, 835, 854, 877, 900, 923, 943, 959, 970, 974, 971, 962, 947, 928, 907, 886, 868, 856, 853, 863, 886, 927, 986, 1064, 1161, 1276, 1408, 1554, 1712, 1878};

//volatile    int         valorI[FFTSIZE] = { 0};
volatile    int         valorI[FFTSIZE] = {2048, 2107, 2166, 2224, 2279, 2333, 2384, 2432, 2476, 2517, 2553, 2586, 2614, 2638, 2657, 2673, 2684, 2692, 2697, 2698, 2697, 2694, 2690, 2684, 2677, 2669, 2662, 2655, 2649, 2644, 2641, 2638, 2638, 2638, 2641, 2644, 2649, 2655, 2662, 2669, 2677, 2684, 2690, 2694, 2697, 2698, 2697, 2692, 2684, 2673, 2657, 2638, 2614, 2586, 2553, 2517, 2476, 2432, 2384, 2333, 2279, 2224, 2166, 2107, 2048, 1989, 1930, 1872, 1817, 1763, 1712, 1664, 1620, 1579, 1543, 1510, 1482, 1458, 1439, 1423, 1412, 1404, 1399, 1398, 1399, 1402, 1406, 1412, 1419, 1427, 1434, 1441, 1447, 1452, 1455, 1458, 1458, 1458, 1455, 1452, 1447, 1441, 1434, 1427, 1419, 1412, 1406, 1402, 1399, 1398, 1399, 1404, 1412, 1423, 1439, 1458, 1482, 1510, 1543, 1579, 1620, 1664, 1712, 1763, 1817, 1872, 1930, 1989, 2048, 2107, 2166, 2224, 2279, 2333, 2384, 2432, 2476, 2517, 2553, 2586, 2614, 2638, 2657, 2673, 2684, 2692, 2697, 2698, 2697, 2694, 2690, 2684, 2677, 2669, 2662, 2655, 2649, 2644, 2641, 2638, 2638, 2638, 2641, 2644, 2649, 2655, 2662, 2669, 2677, 2684, 2690, 2694, 2697, 2698, 2697, 2692, 2684, 2673, 2657, 2638, 2614, 2586, 2553, 2517, 2476, 2432, 2384, 2333, 2279, 2224, 2166, 2107, 2048, 1989, 1930, 1872, 1817, 1763, 1712, 1664, 1620, 1579, 1543, 1510, 1482, 1458, 1439, 1423, 1412, 1404, 1399, 1398, 1399, 1402, 1406, 1412, 1419, 1427, 1434, 1441, 1447, 1452, 1455, 1458, 1458, 1458, 1455, 1452, 1447, 1441, 1434, 1427, 1419, 1412, 1406, 1402, 1399, 1398, 1399, 1404, 1412, 1423, 1439, 1458, 1482, 1510, 1543, 1579, 1620, 1664, 1712, 1763, 1817, 1872, 1930, 1989};

int Vf_r[FFTSIZE]  = { 0};
int If_r[FFTSIZE]  = { 0};
float V_mag[HALF_FFTSIZE] = { 0};     // valor absoluto de I
float V_ang[HALF_FFTSIZE] = { 0};     // Ángulos I
float I_mag[HALF_FFTSIZE] = { 0};     // valor absoluto de I
float I_ang[HALF_FFTSIZE] = { 0};     // Ángulos I
float IrmsFFTTHD[40] = { 0};
float VrmsFFTTHD[40] = { 0};
int mag_FastFFT[HALF_FFTSIZE] = { 0};     // Magnitudes
int dc_offset = 2048;
int SupplyVoltage = 3300;
int ADC_COUNTS = 4096;
float I_RATIO;
float V_RATIO;
float FASE_RATIO_TEMP;
float FASE_RATIO;
int lastFiltered;
int posicion = 0;
int posicion_anterior = 0;

unsigned long time_start;
unsigned int  time_calc;
float realPower, reactivePower, apparentPower, powerFactor, Vrms, Irms;
float realPowerFFT, reactivePowerFFT, apparentPowerFFT, powerFactorFFT, VrmsFFT, IrmsFFT; 
float THDv=0, THDi=0;

#define MULTI_VARS_SIZE 8
char V[MULTI_VARS_SIZE], I[MULTI_VARS_SIZE], P[MULTI_VARS_SIZE], S[MULTI_VARS_SIZE], Q[MULTI_VARS_SIZE], FP[MULTI_VARS_SIZE], V_FFT[MULTI_VARS_SIZE], I_FFT[MULTI_VARS_SIZE], P_FFT[MULTI_VARS_SIZE], S_FFT[MULTI_VARS_SIZE], Q_FFT[MULTI_VARS_SIZE], FP_FFT[MULTI_VARS_SIZE];
#define THD_VARS_SIZE 6


volatile bool up = 0;
volatile bool down = 0;
volatile bool ok = 0;
volatile bool salir = 0;
bool VorI = 0;
long T0 = 0 ;
//Variables screen_variables_armonicos
float rms_anterior[40] = { 0};              //rms para borrar pantalla en screen_variables_armonicos
float THD_anterior=0;                       //THD para borrar pantalla en screen_variables_armonicos

void setup()
{
  Serial.begin (115200);
  TFTscreen.begin();
  TFTscreen.background(0, 0, 0);
  screenmenu(posicion, posicion_anterior);
  ratio_setup(248.8, 30.5, 1);
  adc_setup ();
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


void calcVI()
{
  //Reset accumulators
  float sqV=0, sumV=0, sqI=0, sumI=0, instP=0, sumP=0, phaseShiftedV=0;


  //-------------------------------------------------------------------------------------------------------------------------
  // 1) Lazo de medida
  //-------------------------------------------------------------------------------------------------------------------------


  for ( uint16_t i = 0; i < FFTSIZE; i++)
  {


    //-----------------------------------------------------------------------------
    // A) Cuadrado de la tensión
    //-----------------------------------------------------------------------------
    sqV = Vf_r[i] * Vf_r[i];                //1) square voltage values
    sumV += sqV;                            //2) sum


    //-----------------------------------------------------------------------------
    // B) Cuadrado de la intensidad
    //-----------------------------------------------------------------------------
    sqI = If_r[i] * If_r[i];                //1) square current values
    sumI += sqI;                            //2) sum

    //-----------------------------------------------------------------------------
    // C) Se calibra la tensión para compensar el retraso de tiempo entre la muestra I y la muestra V.
    //    -Si 20ms sonn 360º ver tiempo entre una muestra y la siguiente y calcular Fase_Cal
    //-----------------------------------------------------------------------------
    phaseShiftedV = lastFiltered + FASE_RATIO_TEMP * (Vf_r[i] - lastFiltered);
    lastFiltered = Vf_r[i];

    //-----------------------------------------------------------------------------
    // D) Instantaneous power calc
    //-----------------------------------------------------------------------------
    instP = phaseShiftedV * If_r[i];          //Instantaneous Power
    sumP += instP;                              //Sum


  }

  //-------------------------------------------------------------------------------------------------------------------------
  // 2) Post loop calculations.
  //-------------------------------------------------------------------------------------------------------------------------
  //Calculation of the root of the mean of the voltage and current squared (rms)
  //Calibration coefficients applied.

  Vrms = V_RATIO * sqrt(sumV / FFTSIZE);                      //Aproximación para entornos de programación
  Irms = I_RATIO * sqrt(sumI / FFTSIZE);


  //Calculation power values
  realPower = V_RATIO * I_RATIO * sumP / FFTSIZE;
  apparentPower = Vrms * Irms;
  powerFactor = realPower / apparentPower;
  reactivePower = sqrt((apparentPower * apparentPower) - (realPower * realPower));


  //--------------------------------------------------------------------------------------
}



void prnt_out(volatile int *array, int dlina){
  Serial.print("\n\t");
  for ( uint32_t i = 0; i < dlina; i++)
  {
    Serial.print(array[i]);
    Serial.print("\t");
    if ((i + 1) % 16 == 0) Serial.print("\n\t");
    }
  Serial.println("\n\t");
}

void prnt_out2( float *array, int dlina){
  Serial.print("\n\t");
  for ( uint32_t i = 0; i < dlina; i++)
  {
    Serial.print(array[i]);
    Serial.print("\t");
    if ((i + 1) % 16 == 0) Serial.print("\n\t");
    }
  Serial.println("\n\t");
}



void adc_setup ()
{
  ADC->ADC_CR &= 0x00;                      //ADC en Stop
  ADC->ADC_MR |= 0x80000000;                //ADC en modo normal, no freerun. Modo secuencial activo.
  ADC->ADC_CHDR = 0x00;
  ADC->ADC_CHER = 0xC0;                     //Habilita canal 7 y 6(¿Se corresponde con pin A0?)
  ADC->ADC_SEQR1 = 0x67000000;              //Convierte los canales 7 y 6
  ADC->ADC_IER |= 0xC0;
}

void muestreo(){
  ADC->ADC_CR |= 0x02;                       //start ADC
  while ((ADC->ADC_ISR & 0xC0) == 0x00);
  if (numero_muestras >= 0 && numero_muestras < FFTSIZE) {
    //valorV[numero_muestras] = ADC->ADC_CDR[7]; //Cuidado, corresponde con el pin A0??
    //valorI[numero_muestras] = ADC->ADC_CDR[6]; //Pin A1*/
  }

  /*
   * Para coger muestras al paso por cero. Es necesario definir paso_cero y ponerla a cero en los mismos casos que numero_muestras. 
   * Numero_muestras tendremos que volver a poner a cero en lugar de a menos 10.
   * if(valorV[numero_muestras]>dc_offset+100 || valorV[numero_muestras]<dc_offset-100 && paso_cero==0){
   * numero_muestras=0;
   * }
   * else
   * {
   * paso_cero=1;
   * numero_muestras++;
   * }
   */


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

void ratio_setup(float VCAL, float ICAL, float Fase_Cal)
{
  FASE_RATIO = ((SupplyVoltage / 1000.0) / (ADC_COUNTS));
  I_RATIO = ICAL * FASE_RATIO;
  V_RATIO = VCAL * FASE_RATIO;     // VCAL*3,3/ADC_COUNTS
  FASE_RATIO_TEMP = Fase_Cal;
}

void calc_FFT(int *f_r, float *magf, float *angf, float RATIO, float RATIO_ang)
{
  int         mag[HALF_FFTSIZE]     = { 0};
  int         ang[HALF_FFTSIZE]     = { 0};
  int         f_i[FFTSIZE]   = { 0};

  //time_start = micros();
  radix.rev_bin( f_r, FFTSIZE);

  radix.fft_radix4_I( f_r, f_i, LOG2_FFT);

  radix.gain_Reset( f_r, LOG2_FFT - 1);         //La amplitud del armónico según la función fft es igual a la amplitud de la señal de entrada por la mitad del número de muestras.
  radix.gain_Reset( f_i, LOG2_FFT - 1);

  radix.get_Magnit3( f_r, f_i, mag, ang);

  radix.get_Ratio (mag, magf, RATIO);
  radix.get_Ratio (ang, angf, RATIO_ang);


  //time_calc  = micros() - time_start;

}

void post_FFT(float *V_mag, float *V_ang, float *I_mag, float *I_ang) {
  float sqVFFT=0, sumVFFT=0, sqIFFT=0, sumIFFT=0, instPFFT=0, sumPFFT=0, desfase = 0;
  
  for (int i = 2; i < 81; i = i + 2) {

    sqVFFT = V_mag[i] * V_mag[i];
    sumVFFT += sqVFFT;
    sqIFFT = I_mag[i] * I_mag[i];
    sumIFFT += sqIFFT;
    desfase = V_ang[i] - I_ang[i];
    instPFFT = V_mag[i] * I_mag[i] * cos(desfase);
    sumPFFT += instPFFT;
  }

  VrmsFFT = sqrt((sumVFFT / 2) + (V_mag[0] * V_mag[0]));
  IrmsFFT = sqrt((sumIFFT / 2) + (I_mag[0] * I_mag[0]));
  realPowerFFT = sumPFFT / 2;
  apparentPowerFFT = VrmsFFT * IrmsFFT;
  reactivePowerFFT = sqrt((apparentPowerFFT * apparentPowerFFT) - (realPowerFFT * realPowerFFT));
  powerFactorFFT = realPowerFFT / apparentPowerFFT;
}

float THD(float *mag, float *RMS_arm) {
  //Reset accumulators
  float sum = 0;
  float cuadrado=0;
  float THDx;
  int indice_armonicos=0;
  RMS_arm[0] = mag[2] / sqrt(2);

  for (int i = 1; i < PRIMEROS40_ARM; i++) {                   //Poner 40 como  define
    indice_armonicos=2*i+2;                                 
    RMS_arm[i] = mag[indice_armonicos]/sqrt(2);
    cuadrado = RMS_arm[i] * RMS_arm[i];
    sum += cuadrado;
  }

  THDx = (sqrt(sum) / RMS_arm [0]) * 100;
  return THDx;

}



void calc_FastFFT(int *f_r, int *mag_Fast)
{
  int         mag[HALF_FFTSIZE]     = { 0};
  int         ang[HALF_FFTSIZE]     = { 0};
  int         f_i[FFTSIZE]   = { 0};

  //time_start = micros();
  radix.rev_bin( f_r, FFTSIZE);

  radix.fft_radix4_I( f_r, f_i, LOG2_FFT);

  radix.gain_Reset( f_r, LOG2_FFT - 1);         //La amplitud del armónico según la función fft es igual a la amplitud de la señal de entrada por la mitad del número de muestras.
  radix.gain_Reset( f_i, LOG2_FFT - 1);

  radix.get_Magnit2( f_r, f_i, mag_Fast);
  //time_calc  = micros() - time_start;


}

void screenmenu (int posicion, int arriba) {
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.setTextSize(2);

  switch (posicion) {
    case 0:
      if (arriba == 1) {
        TFTscreen.text("\n\nOnda temp.\n>Espec. frec.\n", 0, 0);
      } else {
        TFTscreen.text("\n\nOnda temp.\n\n\n\n\n>Armonicos", 0, 0);
      }
      TFTscreen.stroke(255, 255, 255);
      TFTscreen.text("    MENU\n\n>Onda temp.\nEspec. frec.\nMult. temp.\nMult. frec.\nTemp. vs Frec.\nArmonicos", 0, 0);
      break;
    case 1:
      if (arriba == 1) {
        TFTscreen.text("\n\n\nEspec. frec.\n>Mult. temp.\n", 0, 0);
      } else {
        TFTscreen.text("\n\n>Onda temp.\nEspec. frec.\n", 0, 0);
      }
      TFTscreen.stroke(255, 255, 255);
      TFTscreen.text("    MENU\n\nOnda temp.\n>Espec. frec.\nMult. temp.\nMult. frec.\nTemp. vs Frec.\nArmonicos", 0, 0);
      break;
    case 2:
      if (arriba == 1) {
        TFTscreen.text("\n\n\n\nMult. temp.\n>Mult. frec.\n", 0, 0);
      } else {
        TFTscreen.text("\n\n\n>Espec. frec.\nMult. temp.\n", 0, 0);
      }
      TFTscreen.stroke(255, 255, 255);
      TFTscreen.text("    MENU\n\nOnda temp.\nEspec. frec.\n>Mult. temp.\nMult. frec.\nTemp. vs Frec.\nArmonicos", 0, 0);
      break;
    case 3:
      if (arriba == 1) {
        TFTscreen.text("\n\n\n\n\nMult. frec.\n>Temp. vs Frec.\n", 0, 0);
      } else {
        TFTscreen.text("\n\n\n\n>Mult. temp.\nMult. frec.\n", 0, 0);
      }
      TFTscreen.stroke(255, 255, 255);
      TFTscreen.text("    MENU\n\nOnda temp.\nEspec. frec.\nMult. temp.\n>Mult. frec.\nTemp. vs Frec.\nArmonicos", 0, 0);
      break;
    case 4:
      if (arriba == 1) {
        TFTscreen.text("\n\n\n\n\n\nTemp. vs Frec.\n>Armonicos", 0, 0);
      } else {
        TFTscreen.text("\n\n\n\n\n>Mult. frec.\nTemp. vs Frec.\n", 0, 0);
      }
      TFTscreen.stroke(255, 255, 255);
      TFTscreen.text("    MENU\n\nOnda temp.\nEspec. frec.\nMult. temp.\nMult. frec.\n>Temp. vs Frec.\nArmonicos", 0, 0);
      break;
    case 5:
      if (arriba == 1) {
        TFTscreen.text("\n\n>Onda temp.\n\n\n\n\nArmonicos", 0, 0);
      } else {
        TFTscreen.text("\n\n\n\n\n\n>Temp. vs Frec.\nArmonicos", 0, 0);
      }
      TFTscreen.stroke(255, 255, 255);
      TFTscreen.text("    MENU\n\nOnda temp.\nEspec. frec.\nMult. temp.\nMult. frec.\nTemp. vs Frec.\n>Armonicos", 0, 0);
      break;
  }
}

void VorI_menu(bool VorI) {
  TFTscreen.stroke(0, 0, 0);

  switch (VorI) {
    case 0:
      TFTscreen.text("\n\nTension\n\n>Intensidad\n", 0, 0);
      TFTscreen.stroke(255, 255, 255);
      TFTscreen.text("\n\n>Tension\n\nIntensidad\n", 0, 0);
      break;
    case 1:
      TFTscreen.text("\n\n>Tension\n\nIntensidad\n", 0, 0);
      TFTscreen.stroke(255, 255, 255);
      TFTscreen.text("\n\nTension\n\n>Intensidad\n", 0, 0);
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
      TFTscreen.setTextSize(2);
      TFTscreen.text("Forma de onda", 0, 0);
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
            ejes("t", "V", 8, 10);
            break;
          case 1:
            ejes("t", "I", 8, 10);
            break;
        }
        desactive_Boton();
        Timer3.start();
      }

      while (!salir) {
        if (numero_muestras >= FFTSIZE) {
          Timer3.stop();
          active_Boton();
          time_start = micros();
          numero_muestras = -10;
          switch (VorI) {
            case 0:
              onda_sen(valorV, FFTSIZE);
              break;
            case 1:
              onda_sen(valorI, FFTSIZE);
              break;
          }
          time_calc = micros() - time_start;
          /*while(time_calc<=200000){                      //Esperar a paso por cero de la onda. Poner en caso de no verse bien la onda
            time_calc  = micros() - time_start;
            }*/
          if (!salir) {
            desactive_Boton();
            Timer3.start();
          }
        }
      }
      Serial.print("\tOnda: ");
      Serial.println(time_calc);
      TFTscreen.background(0, 0, 0);
      break;
    case 1:                                     //Espectrometro
      TFTscreen.setTextSize(2);
      TFTscreen.text("Espectro Frec.", 0, 0);
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
            ejes("f", "V", 3, 10);
            break;
          case 1:
            ejes("f", "I", 3, 10);
            break;
        }
        desactive_Boton();
        Timer3.start();
      }

      while (!salir) {
        if (numero_muestras >= FFTSIZE) {
          Timer3.stop();
          active_Boton();
          time_start = micros();
          numero_muestras = -10;
          calc_offset();
          switch (VorI) {
            case 0:
              calc_FastFFT(Vf_r, mag_FastFFT);
              break;
            case 1:
              calc_FastFFT(If_r, mag_FastFFT);
              break;
          }
          espectro_frec40(mag_FastFFT);
          time_calc  = micros() - time_start;
          if (!salir) {
            desactive_Boton();
            Timer3.start();
          }
        }
      }
      Serial.print("\tFastFFT: ");
      Serial.println(time_calc);
      TFTscreen.background(0, 0, 0);
      break;
    case 2:
      TFTscreen.setTextSize(2);
      TFTscreen.text("Mult. temporal", 0, 0);
      desactive_Boton();
      Timer3.start();
      while (!salir) {
        if (numero_muestras >= FFTSIZE) {
          Timer3.stop();
          active_Boton();
          time_start = micros();
          numero_muestras = -10;
          calc_offset();
          calcVI();
          screen_multimeter(Vrms, Irms, realPower, reactivePower, apparentPower, powerFactor);
          time_calc  = micros() - time_start;
          if (!salir) {
            desactive_Boton();
            Timer3.start();
          }
        }
      }
      Serial.print("\tMultimetro temporal: ");
      Serial.println(time_calc);
      TFTscreen.background(0, 0, 0);
      break;
    case 3:
      TFTscreen.setTextSize(2);
      TFTscreen.text("Mult. frecuencial", 0, 0);
      desactive_Boton();
      Timer3.start();
      while (!salir) {
        if (numero_muestras >= FFTSIZE) {
          Timer3.stop();
          active_Boton();
          time_start = micros();
          numero_muestras = -10;
          calc_offset();
          calc_FFT(Vf_r, V_mag, V_ang, V_RATIO, FASE_RATIO);
          calc_FFT(If_r, I_mag, I_ang, I_RATIO, FASE_RATIO);
          //prnt_out2(V_ang, HALF_FFTSIZE);
          //prnt_out2(I_ang, HALF_FFTSIZE);
          post_FFT(V_mag, V_ang, I_mag, I_ang);
          screen_multimeter(VrmsFFT, IrmsFFT, realPowerFFT, reactivePowerFFT, apparentPowerFFT, powerFactorFFT);
          time_calc  = micros() - time_start;
          if (!salir) {
            desactive_Boton();
            Timer3.start();
          }
        }
      }
      Serial.print("\tFFT: ");
      Serial.println(time_calc);
      TFTscreen.background(0, 0, 0);
      break;
    case 4:
      screen_temp_vs_FFT();
      desactive_Boton();
      Timer3.start();
      while (!salir) {
        if (numero_muestras >= FFTSIZE) {
          Timer3.stop();
          active_Boton();
          time_start = micros();
          numero_muestras = -10;
          calc_offset();
          calc_FFT(Vf_r, V_mag, V_ang, V_RATIO, FASE_RATIO);
          calc_FFT(If_r, I_mag, I_ang, I_RATIO, FASE_RATIO);
          post_FFT(V_mag, V_ang, I_mag, I_ang);
          calc_offset();
          calcVI();
          screen_variables_temp_vs_FFT();
          time_calc  = micros() - time_start;
          if (!salir) {
            desactive_Boton();
            Timer3.start();
          }
        }
      }
      Serial.print("\tTiempo de calculo temporal vs FFT: ");
      Serial.println(time_calc);
      TFTscreen.background(0, 0, 0);
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
            screen_armonicos("Armonicos   THDv=", "V  =       V  V  =       V");
            break;
          case 1:
            screen_armonicos("Armonicos   THDi=", "I  =       A  I  =       A");
            break;
        }
        desactive_Boton();
        Timer3.start();
      }

      while (!salir) {
        if (numero_muestras >= FFTSIZE) {
          Timer3.stop();
          active_Boton();
          time_start = micros();
          numero_muestras = -10;
          calc_offset(); 
          switch (VorI) {
            case 0:
              calc_FFT(Vf_r, V_mag, V_ang, V_RATIO, FASE_RATIO);
              Serial.println("case V");
              Serial.println(ok);
              THDv=THD(V_mag, VrmsFFTTHD);     //ok pasa a 0 sin motivo cada vez que entra
              Serial.println(ok);
              screen_variables_armonicos(THDv, VrmsFFTTHD);
              break;
            case 1:

              calc_FFT(If_r, I_mag, I_ang, I_RATIO, FASE_RATIO);
              THDi=THD(I_mag, IrmsFFTTHD);
              screen_variables_armonicos(THDi, IrmsFFTTHD);
              break;
          }
          time_calc  = micros() - time_start;
          if (!salir) {
            desactive_Boton();
            Timer3.start();
          }
        }
      }
      Serial.print("\tArmonicos: ");
      Serial.println(time_calc);
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

void calc_offset() {
  for ( uint16_t i = 0; i < FFTSIZE; i++ )
  {

    Vf_r[i] = (valorV[i] - dc_offset);
    If_r[i] = (valorI[i] - dc_offset);
  }

}

void onda_sen(volatile int *onda, int longitud) {


  int xPos = 34;
  int muestra;
  int muestra_anterior = 59;


  for (int i = 0; i < longitud; i = i + 2) {

    int sensor = onda[i];

    muestra = map(sensor, 0, 4096, 0, (TFTscreen.height() - 10));

    TFTscreen.stroke(0, 0, 0);                            //Color negro
    TFTscreen.line(xPos, 0, xPos, 118);                   //borrar posicion que vamos a pintar
    TFTscreen.line(xPos + 1, 0, xPos + 1, 118);               //borrar posicion que vamos a pintar
    TFTscreen.line(xPos + 2, 0, xPos + 2, 118);
    TFTscreen.stroke(255, 255, 255);                      //Blanco *


    TFTscreen.line(xPos - 2, muestra_anterior, xPos, muestra); //Representar linea desde posición anterior hasta posicion actual
    muestra_anterior = muestra;

    xPos = xPos + 2;

  }

}

void espectro_frec(int *frec, int longitud) {


  int xPos = 33;
  int muestra;

  for (int i = 0; i < longitud; i++) {

    int amplitud = frec[i];

    muestra = map(amplitud, 0, 1500, 0, (TFTscreen.height() - 10));


    TFTscreen.line(xPos, 118, xPos, 118 - muestra);           //Representar amplitud de la frec

    // if the graph has reached the screen edge
    // erase the screen and start again
    if (xPos == 160) {
      xPos = 33;                                          //Hemos representado 128 muestras, pasamos a inicio de la pantalla
      TFTscreen.noStroke();
      TFTscreen.fill(0, 0, 0);                          //Color negro
      TFTscreen.rect(xPos - 1, 0, 160, 118);                //Borrar pantalla completa. (Ver si tomando muestras reales es mejor borrar posicion que vamos a pintar, al igual que hacíamos en v1)
      TFTscreen.stroke(255, 255, 255);                      //Blanco
    } else {
      // increment the horizontal position:
      xPos++;
    }
  }

}

void screen_multimeter (float V_cal, float I_cal, float P_cal, float Q_cal, float S_cal, float FP_cal) {

  TFTscreen.text("\n\nVrms=\nIrms=\nP=\nQ=\nS=\nFP=", 0, 0);
  TFTscreen.text("\n\n            V\n            A\n          W\n          Var\n          VA\n", 0, 0);


  String sensorVal = String(V_cal);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(V, 70, 32);
  sensorVal.toCharArray(V, MULTI_VARS_SIZE);
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.text(V, 70, 32);


  sensorVal = String(I_cal);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(I, 70, 48);
  sensorVal.toCharArray(I, MULTI_VARS_SIZE);
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.text(I, 70, 48);


  sensorVal = String(P_cal);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(P, 30, 64);
  sensorVal.toCharArray(P, MULTI_VARS_SIZE);
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.text(P, 30, 64);


  sensorVal = String(Q_cal);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(Q, 30, 80);
  sensorVal.toCharArray(Q, MULTI_VARS_SIZE);
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.text(Q, 30, 80);


  sensorVal = String(S_cal);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(S, 30, 96);
  sensorVal.toCharArray(S, MULTI_VARS_SIZE);
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.text(S, 30, 96);


  sensorVal = String(FP_cal);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(FP, 50, 112);
  sensorVal.toCharArray(FP, MULTI_VARS_SIZE);
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.text(FP, 50, 112);

}

void screen_temp_vs_FFT () {

  TFTscreen.setTextSize(1);
  TFTscreen.text("\n   Temporal\n\nVrms=\n\nIrms=\n\nP=\n\nQ=\n\nS=\n\nFP=", 0, 0);
  TFTscreen.text("\n\n\n            V\n\n            A\n\n            W\n\n          Var\n\n          VA\n", 0, 0);
  TFTscreen.line(78, 0, 78, 128);
  TFTscreen.text("FFT", 110, 8);
  TFTscreen.text("Vrms=", 80, 24);
  TFTscreen.text("Irms=", 80, 40);
  TFTscreen.text("P=", 80, 56);
  TFTscreen.text("Q=", 80, 72);
  TFTscreen.text("S=", 80, 88);
  TFTscreen.text("FP=", 80, 104);

  TFTscreen.text("V", 152, 24);
  TFTscreen.text("A", 152, 40);
  TFTscreen.text("W", 152, 56);
  TFTscreen.text("Var", 144, 72);
  TFTscreen.text("VA", 144, 88);

}

void screen_variables_temp_vs_FFT() {

  // realPower, reactivePower, apparentPower, powerFactor, Vrms, Irms;

  String sensorVal = String(Vrms);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(V, 32, 24);
  sensorVal.toCharArray(V, MULTI_VARS_SIZE);
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.text(V, 32, 24);

  sensorVal = String(Irms);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(I, 32, 40);
  sensorVal.toCharArray(I, MULTI_VARS_SIZE);
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.text(I, 32, 40);

  sensorVal = String(realPower);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(P, 16, 56);
  sensorVal.toCharArray(P, MULTI_VARS_SIZE);
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.text(P, 16, 56);

  sensorVal = String(reactivePower);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(Q, 16, 72);
  sensorVal.toCharArray(Q, MULTI_VARS_SIZE);
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.text(Q, 16, 72);

  sensorVal = String(apparentPower);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(S, 16, 88);
  sensorVal.toCharArray(S, MULTI_VARS_SIZE);
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.text(S, 16, 88);

  sensorVal = String(powerFactor);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(FP, 24, 104);
  sensorVal.toCharArray(FP, MULTI_VARS_SIZE);
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.text(FP, 24, 104);

  // realPowerFFT, reactivePowerFFT, apparentPowerFFT, powerFactorFFT, VrmsFFT, IrmsFFT;

  sensorVal = String(VrmsFFT);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(V_FFT, 112, 24);
  sensorVal.toCharArray(V_FFT, MULTI_VARS_SIZE);
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.text(V_FFT, 112, 24);

  sensorVal = String(IrmsFFT);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(I_FFT, 112, 40);
  sensorVal.toCharArray(I_FFT, MULTI_VARS_SIZE);
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.text(I_FFT, 112, 40);

  sensorVal = String(realPowerFFT);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(P_FFT, 96, 56);
  sensorVal.toCharArray(P_FFT, MULTI_VARS_SIZE);
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.text(P_FFT, 96, 56);

  sensorVal = String(reactivePowerFFT);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(Q_FFT, 96, 72);
  sensorVal.toCharArray(Q_FFT, MULTI_VARS_SIZE);
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.text(Q_FFT, 96, 72);

  sensorVal = String(apparentPowerFFT);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(S_FFT, 96, 88);
  sensorVal.toCharArray(S_FFT, MULTI_VARS_SIZE);
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.text(S_FFT, 96, 88);

  sensorVal = String(powerFactorFFT);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(FP_FFT, 104, 104);
  sensorVal.toCharArray(FP_FFT, MULTI_VARS_SIZE);
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.text(FP_FFT, 104, 104);

}

void espectro_frec40(int *frec) {                   //REVISARRRRRRRRRRR


  int xPos = 34;
  int muestra;

  for (int i = 2; i < 81; i = i + 2) {                                  //ver si se puede cambiar el 81 por 40

    int amplitud = frec[i];

    muestra = map(amplitud, 0, 2048, 0, (TFTscreen.height() - 10));     //Cambiar segun circuito acondicionamiento

    TFTscreen.stroke(0, 0, 0);                            //Color negro
    TFTscreen.line(xPos, 0, xPos, 118);                   //borrar posicion que vamos a pintar
    TFTscreen.line(xPos + 1, 0, xPos + 1, 118);               //borrar posicion que vamos a pintar
    TFTscreen.stroke(255, 255, 255);                      //Blanco */

    TFTscreen.line(xPos, 118, xPos, (118 - muestra));           //Representar amplitud de la frec
    TFTscreen.line(xPos + 1, 118, xPos + 1, (118 - muestra));       //Representar amplitud de la frec

    xPos = xPos + 3;


  }

}


void ejes(char *abscisas, char *ordenadas, int xDiv, int yDiv) {
  TFTscreen.text(ordenadas, 0, 0);
  TFTscreen.text(abscisas, 155, 120);
  TFTscreen.line(32, 0, 32, 128);
  TFTscreen.line(0, 118, 160, 118);

  for (int x = 32; x < 160; x = x + xDiv) {
    TFTscreen.line(x, 118, x, 122);
  }

  for (int y = 118; y > 0; y = y - yDiv) {
    TFTscreen.line(28, y, 32, y);
  }

}

void screen_armonicos (char *titulo, char *variable) {
  String num_armonico;
  int num = 0;
  char num_1[MULTI_VARS_SIZE], num_2[MULTI_VARS_SIZE];
  TFTscreen.line(78, 16, 78, 128);
  TFTscreen.text(titulo, 16, 0);
  for (int y = 12; y < 128; y = y + 12) {
    TFTscreen.text(variable, 0, y);
    num++;
    num_armonico = String(num);
    num_armonico.toCharArray(num_1, MULTI_VARS_SIZE);
    num++;
    num_armonico = String(num);
    num_armonico.toCharArray(num_2, MULTI_VARS_SIZE);
    TFTscreen.text(num_1, 6, y);
    TFTscreen.text(num_2, 90, y);
  }
}

void screen_variables_armonicos(float THD_calc, float *rms) {
  char arm_pares[THD_VARS_SIZE], arm_impares[THD_VARS_SIZE], THD_var[THD_VARS_SIZE];
  String num_armonico;                                        //string temporal, luego se convierte a char array correspondientes THD_var,arm_pares y arm_impares
  int armonico = 0;
  num_armonico = String(THD_anterior);                        //Float a string THD anterior
  num_armonico.toCharArray(THD_var, THD_VARS_SIZE);
  TFTscreen.stroke(0, 0, 0);                                  //Texto color negro
  TFTscreen.text(THD_var, 120, 0);                            //Borrar THD anterior
  THD_anterior = THD_calc;                                    //Guardar THD para luego borrar
  num_armonico = String(THD_calc);                            //Float a string THD actual
  num_armonico.toCharArray(THD_var, THD_VARS_SIZE);
  TFTscreen.stroke(255, 255, 255);                            //Texto blanco
  TFTscreen.text(THD_var, 120, 0);                            //Escribe variable actual
  for (int i = 0; i < PRIMEROS40_ARM/2; i++)                   
  {
    int y=12*i+12;                                            //128Pixeles en eje y, escribimos cada 12 una linea con armonico par e impar
    TFTscreen.stroke(0, 0, 0);                                //Texto color negro
    num_armonico = String(rms_anterior[armonico]);            //Float rms par anterior a string
    num_armonico.toCharArray(arm_pares, THD_VARS_SIZE);       //String a chararray
    TFTscreen.text(arm_pares, 28, y);                         //Escribir en negro rms anterior, es decir, borrar armonicos pares
    num_armonico = String(rms[armonico]);                     //Float rms par actual a string
    num_armonico.toCharArray(arm_pares, THD_VARS_SIZE);
    rms_anterior[armonico] = rms[armonico];                   //Guardar rms actual para luego poder borrarlo
    armonico++;
    num_armonico = String(rms_anterior[armonico]);            //Float rms impar anterior a string
    num_armonico.toCharArray(arm_impares, THD_VARS_SIZE);
    TFTscreen.text(arm_impares, 112, y);                        //Escribir en negro rms anterior, es decir, borrar armonicos pares
    num_armonico = String(rms[armonico]);                 //Float rms impar actual a string
    num_armonico.toCharArray(arm_impares, THD_VARS_SIZE);
    rms_anterior[armonico] = rms[armonico];               //Guardar rms actual para luego poder borrarlo
    TFTscreen.stroke(255, 255, 255);                      //Texto color negro
    TFTscreen.text(arm_pares, 28, y);                         //rms par en parte izquierda pantalla
    TFTscreen.text(arm_impares, 112, y);                        //rms impar en parte derecha pantalla
    armonico++;
  }
}
