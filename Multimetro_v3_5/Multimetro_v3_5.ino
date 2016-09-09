/*-----------------------------------------------------------------------
Sistema de medida de armonicos con arduino.
Francisco Javier García Torrejón.
Doble grado ingeniería eléctrica y electrónica industrial.
------------------------------------------------------------------------*/
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
#define RESET_MUESTRAS -10                //Se reinicia a -10 para descartar 10 primeras muestras, ya que arduino no suele tomarlas bien
#define MULTI_VARS_SIZE 8
#define THD_VARS_SIZE 6
#define POS_EJE_X 113
#define POS_EJE_Y 32

TFT TFTscreen = TFT(cs, dc, rst);


volatile    int         numero_muestras = RESET_MUESTRAS;
//volatile    int         valorV[FFTSIZE] = { 0};
volatile    int         valorV[FFTSIZE] = {2048,2139,2229,2318,2405,2489,2571,2649,2724,2796,2864,2929,2991,3049,3105,3159,3210,3260,3308,3354,3399,3442,3483,3522,3559,3592,3623,3649,3672,3690,3703,3711,3713,3711,3703,3690,3672,3649,3623,3592,3559,3522,3483,3442,3399,3354,3308,3260,3210,3159,3105,3049,2991,2929,2864,2796,2724,2649,2571,2489,2405,2318,2229,2139,2048,1957,1867,1778,1691,1607,1525,1447,1372,1300,1232,1167,1105,1047,991,937,886,836,788,742,697,654,613,574,537,504,473,447,424,406,393,385,383,385,393,406,424,447,473,504,537,574,613,654,697,742,788,836,886,937,991,1047,1105,1167,1232,1300,1372,1447,1525,1607,1691,1778,1867,1957,2048,2139,2229,2318,2405,2489,2571,2649,2724,2796,2864,2929,2991,3049,3105,3159,3210,3260,3308,3354,3399,3442,3483,3522,3559,3592,3623,3649,3672,3690,3703,3711,3713,3711,3703,3690,3672,3649,3623,3592,3559,3522,3483,3442,3399,3354,3308,3260,3210,3159,3105,3049,2991,2929,2864,2796,2724,2649,2571,2489,2405,2318,2229,2139,2048,1957,1867,1778,1691,1607,1525,1447,1372,1300,1232,1167,1105,1047,991,937,886,836,788,742,697,654,613,574,537,504,473,447,424,406,393,385,383,385,393,406,424,447,473,504,537,574,613,654,697,742,788,836,886,937,991,1047,1105,1167,1232,1300,1372,1447,1525,1607,1691,1778,1867,1957};

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
float quantum;
int lastFiltered;
int posicion = 0;
int posicion_anterior = 0;

unsigned long time_start;
unsigned int  time_calc;
float realPower, reactivePower, apparentPower, powerFactor, Vrms, Irms;
float realPowerFFT, reactivePowerFFT, apparentPowerFFT, powerFactorFFT, VrmsFFT, IrmsFFT; 
float THDv=0, THDi=0;

int div_x, div_y, inicio_escala_x, incr_escala_x, inicio_escala_y, incr_escala_y;
int min_pixel_y, max_pixel_y, min_muestra_y, max_muestra_y, num_div_y;

char V[MULTI_VARS_SIZE], I[MULTI_VARS_SIZE], P[MULTI_VARS_SIZE], S[MULTI_VARS_SIZE], Q[MULTI_VARS_SIZE], FP[MULTI_VARS_SIZE], V_FFT[MULTI_VARS_SIZE], I_FFT[MULTI_VARS_SIZE], P_FFT[MULTI_VARS_SIZE], S_FFT[MULTI_VARS_SIZE], Q_FFT[MULTI_VARS_SIZE], FP_FFT[MULTI_VARS_SIZE];


volatile bool up = 0;                                   //Bandera up
volatile bool down = 0;                                 //Bandera down
volatile bool ok = 0;                                   //Bandera ok
volatile bool salir = 0;                                //Bandera salir
bool VorI = 0;                                          //Bandera tensión o intensidad 
long T0 = 0 ;                                           //Tiempo espera de rebote al pulsar botones. Usar solo en las interrupciones(No es volatile)
//Variables screen_variables_armonicos
float rms_anterior[40] = { 0};                          //rms para borrar pantalla en screen_variables_armonicos
float THD_anterior=0;                                   //THD para borrar pantalla en screen_variables_armonicos

void setup()
{
  Serial.begin (115200);
  TFTscreen.begin();
  TFTscreen.background(0, 0, 0);
  active_Boton();
  select_pinza ();
  screenmenu(posicion, posicion_anterior);
  adc_setup ();
  Timer3.attachInterrupt(muestreo).setFrequency(6400);

}

Radix4     radix;


void loop()
{
  if (up == 1){                                 //Modifica posición al pulsar up
    up = 0;                                     //Reset bandera up
    if (posicion == 0) {                        //Está arriba del todo pasar a última
      posicion = 5;
    } else {
      posicion--;                               //Subir uno
    }
    posicion_anterior = 1;                      //Bandera para saber si has pulsado up(1) or down(0)
    screenmenu(posicion, posicion_anterior);
  } else if (down == 1){                        //Modifica posición al pulsar down                     
    down = 0;                                   //Reset bandera down
    if (posicion == 5) {                        //Está abajo del todo pasar a primera
      posicion = 0;
    } else {                                    //Bajar uno
      posicion++;
    }
    posicion_anterior = 0;                      //Bandera para saber si has pulsado up(1) or down(0)
    screenmenu(posicion, posicion_anterior);
  } else if (ok == 1) {                         //Ok pulsado
    ok = 0;                                     //Reset bandera ok
    functionmenu(posicion);
  } else if (salir == 1) {
    select_pinza ();
    posicion=0;
    posicion_anterior=0;
    salir = 0;
    screenmenu(posicion, posicion_anterior);
  }

}


void calcVI(int *Vinst, int *Iinst, float desfaseratio)
{
  //Reset accumulators
  float sqV=0, sumV=0, sqI=0, sumI=0, instP=0, sumP=0, phaseShiftedV=0;


  //-------------------------------------------------------------------------------------------------------------------------
  // 1) Lazo de cálculo
  //-------------------------------------------------------------------------------------------------------------------------


  for ( uint16_t i = 0; i < FFTSIZE; i++)
  {


    //-----------------------------------------------------------------------------
    // A) Cuadrado de la tensión
    //-----------------------------------------------------------------------------
    sqV = Vinst[i] * Vinst[i];                
    sumV += sqV;                            


    //-----------------------------------------------------------------------------
    // B) Cuadrado de la intensidad
    //-----------------------------------------------------------------------------
    sqI = Iinst[i] * Iinst[i];                
    sumI += sqI;                            

    //-----------------------------------------------------------------------------
    // C) Se calibra la tensión para compensar el retraso de tiempo entre la muestra I y la muestra V.
    //    -Si 20ms sonn 360º ver tiempo entre una muestra y la siguiente y calcular desfaseratio
    //-----------------------------------------------------------------------------
    phaseShiftedV = lastFiltered + desfaseratio * (Vinst[i] - lastFiltered);
    lastFiltered = Vinst[i];                                         //Guardo en variable global para luego usar

    //-----------------------------------------------------------------------------
    // D) Potencia instantánea
    //-----------------------------------------------------------------------------
    instP = phaseShiftedV * Iinst[i];          
    sumP += instP;                   


  }


  Vrms = V_RATIO * sqrt(sumV / FFTSIZE);                      //Aproximación para entornos de programación
  Irms = I_RATIO * sqrt(sumI / FFTSIZE);


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
  ADC->ADC_CR |= 0x02;                        //start ADC
  while ((ADC->ADC_ISR & 0xC0) == 0x00);      //Esperar que termine conversión
  if (numero_muestras >= 0 && numero_muestras < FFTSIZE) {
    valorV[numero_muestras] = ADC->ADC_CDR[7]; //Cuidado, corresponde con el pin A0
    valorI[numero_muestras] = ADC->ADC_CDR[6]; //Pin A1
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

void select_pinza (){
  int indicador=0;
  int ind_anterior=0;
  TFTscreen.background(0, 0, 0);
  screenpinza(indicador, ind_anterior); 
  while(!ok){
    if (up == 1){                                 //Modifica posición al pulsar up
      up = 0;                                     //Reset bandera up
      if (indicador == 0) {                        //Está arriba del todo pasar a última
        indicador = 2;
      } else {
        indicador--;                               //Subir uno
      }
      ind_anterior = 1;                      //Bandera para saber si has pulsado up(1) or down(0)
      screenpinza(indicador, ind_anterior);
    } else if (down == 1){                        //Modifica posición al pulsar down                     
      down = 0;                                   //Reset bandera down
      if (indicador == 2) {                        //Está abajo del todo pasar a primera
        indicador = 0;
      } else {                                    //Bajar uno
        indicador++;
      }
      ind_anterior = 0;                      //Bandera para saber si has pulsado up(1) or down(0)
      screenpinza(indicador, ind_anterior);
    }
  }
  switch(indicador){
    case 0:
    ratio_setup(247.1,23,1);        //Pinza SCT 013 030. Calibrada ya con fuente programable.
    break;
    case 1:
    ratio_setup(247.1,8.7,1);         //Pinza SCT 013 100. Falta calibrar
    break;
    case 2:
    ratio_setup(247.1,8.7,1);         //Pinza Chaubin E3N. Falta calibrar.
    break;
  }
  TFTscreen.background(0, 0, 0);
  ok=0;
}

void screenpinza (int indicador, int arriba) {
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.setTextSize(2);

  switch (indicador) {
    case 0:
      if (arriba == 1) {
        TFTscreen.text("\n\nSCT 013 030\n\n>SCT 013 100\n", 0, 0);
      } else {
        TFTscreen.text("\n\nSCT 013 030\n\n\n\n>Chaubin E3N\n", 0, 0);
      }
      TFTscreen.stroke(255, 255, 255);
      TFTscreen.text("MENU PINZAS\n\n>SCT 013 030\n\nSCT 013 100\n\nChaubin E3N\n", 0, 0);
      break;
    case 1:
      if (arriba == 1) {
        TFTscreen.text("\n\n\n\nSCT 013 100\n\n>Chaubin E3N\n", 0, 0);
      } else {
        TFTscreen.text("\n\n>SCT 013 030\n\nSCT 013 100\n\n\n", 0, 0);
      }
      TFTscreen.stroke(255, 255, 255);
      TFTscreen.text("MENU PINZAS\n\nSCT 013 030\n\n>SCT 013 100\n\nChaubin E3N\n", 0, 0);
      break;
    case 2:
      if (arriba == 1) {
        TFTscreen.text("\n\n>SCT 013 030\n\n\n\nChaubin E3N\n", 0, 0);
      } else {
        TFTscreen.text("\n\n\n\n>SCT 013 100\n\nChaubin E3N\n", 0, 0);
      }
      TFTscreen.stroke(255, 255, 255);
      TFTscreen.text("MENU PINZAS\n\nSCT 013 030\n\nSCT 013 100\n\n>Chaubin E3N\n", 0, 0);
      break;
  }
}

void ratio_setup(float VCAL, float ICAL, float Fase_Cal)
{
  quantum = ((SupplyVoltage / 1000.0) / (ADC_COUNTS));
  I_RATIO = ICAL * quantum;
  V_RATIO = VCAL * quantum;     // VCAL*3,3/ADC_COUNTS
  FASE_RATIO_TEMP = Fase_Cal;
}

void calc_FFT(int *f_r, float *magf, float *angf, float RATIO)
{
  float         f_r_ratio[HALF_FFTSIZE]     = { 0};
  float         f_i_ratio[HALF_FFTSIZE]     = { 0};
  int         f_i[FFTSIZE]   = { 0};

  //time_start = micros();
  radix.rev_bin( f_r, FFTSIZE);

  radix.fft_radix4_I( f_r, f_i, LOG2_FFT);

  radix.gain_Reset( f_r, LOG2_FFT - 1);         //La amplitud del armónico según la función fft es igual a la amplitud de la señal de entrada por la mitad del número de muestras.
  radix.gain_Reset( f_i, LOG2_FFT - 1);

  radix.get_Ratio (f_r, f_r_ratio, RATIO);
  radix.get_Ratio (f_i, f_i_ratio, RATIO);
  
  radix.get_Magnit_float(f_r_ratio, f_i_ratio, magf, angf);
  
  //time_calc  = micros() - time_start;

}

void post_FFT(float *V_mag_post, float *V_ang_post, float *I_mag_post, float *I_ang_post) {
  float sqVFFT=0, sumVFFT=0, sqIFFT=0, sumIFFT=0, instPFFT=0, sumPFFT=0, desfase = 0;
  
  for (int i = 1; i < PRIMEROS40_ARM; i++) {

    sqVFFT = V_mag_post[2*i] * V_mag[2*i];                         //Cuadrado de los múltiplos de V de la frecuencia fundamental
    sumVFFT += sqVFFT;                                        //Sumatorio de los cuadrados de V
    sqIFFT = I_mag_post[2*i] * I_mag_post[2*i];                         //Cuadrado de los múltiplos de I de la frecuencia fundamental
    sumIFFT += sqIFFT;                                        //Sumatorio de los cuadrados de I
    desfase = V_ang_post[2*i] - I_ang_post[2*i];                        //Desfase entre V e I
    instPFFT = V_mag_post[2*i] * I_mag_post[2*i] * cos(desfase);        //Potencia instantánea
    sumPFFT += instPFFT;                                      //Sumatorio de las potencias instantáneas
  }
  
  VrmsFFT = sqrt((sumVFFT / 2) + (V_mag_post[0] * V_mag_post[0]));      //El 2 es de 1/(raiz(2)*raiz(2)) del cuadrado de tensión anterior más parte continua
  IrmsFFT = sqrt((sumIFFT / 2) + (I_mag_post[0] * I_mag_post[0]));      //El 2 es de 1/(raiz(2)*raiz(2)) del cuadrado de intensidad anterior más parte continua
  realPowerFFT = (sumPFFT/2)+(V_mag_post[0]*I_mag_post[0]);             //El 2 es de 1/(raiz(2)*raiz(2)) de la tensión por intensidad anterior más parte continua
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

  for (int i = 1; i < PRIMEROS40_ARM; i++) {                   
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

  radix.get_Magnit( f_r, f_i, mag_Fast);
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
          up = 0;                                   //Reset bandera up
          down = 0;                                   //Reset bandera down
        }
      }
      if (salir) {
        salir2 = 1;
      } else {
        TFTscreen.background(0, 0, 0);
        TFTscreen.setTextSize(1);
        switch (VorI) {
          case 0:
            div_x=16; 
            div_y=8;
            inicio_escala_x=5; 
            incr_escala_x=5;
            inicio_escala_y=-360; 
            incr_escala_y=60;
            ejes("t(ms)", "V(V)", div_x, div_y, inicio_escala_x, incr_escala_x, inicio_escala_y, incr_escala_y);
            // Desde POS_EJE_X-yDiv hasta POS_EJE_X - (yDiv*num divisiones) y desde (inicio_escala_y/RATIO)+dc_offset hasta ((inicio_escala_y+(incr_escala_y*num divisiones))/RATIO)+dc_offset
            //Calculamos parámetros para escalar la onda en la pantalla
            min_pixel_y=POS_EJE_X-div_y;
            max_pixel_y=POS_EJE_X-(div_y*num_div_y);
            min_muestra_y=(inicio_escala_y/V_RATIO)+dc_offset;
            max_muestra_y=((inicio_escala_y+(incr_escala_y*(num_div_y-1)))/V_RATIO)+dc_offset;
            break;
          case 1:
            div_x=16; 
            div_y=8;
            inicio_escala_x=5; 
            incr_escala_x=5;
            inicio_escala_y=-12; 
            incr_escala_y=2;
            ejes("t(ms)", "I(A)", div_x, div_y, inicio_escala_x, incr_escala_x, inicio_escala_y, incr_escala_y);
            //Calculamos parámetros para escalar la onda en la pantalla
            min_pixel_y=POS_EJE_X-div_y;
            max_pixel_y=POS_EJE_X-(div_y*num_div_y);
            min_muestra_y=(inicio_escala_y/I_RATIO)+dc_offset;
            max_muestra_y=((inicio_escala_y+(incr_escala_y*(num_div_y-1)))/I_RATIO)+dc_offset;
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
          numero_muestras = RESET_MUESTRAS;
          switch (VorI) {
            case 0:
              onda_sen(valorV, FFTSIZE, min_pixel_y, max_pixel_y, min_muestra_y, max_muestra_y);
              break;
            case 1:
              onda_sen(valorI, FFTSIZE, min_pixel_y, max_pixel_y, min_muestra_y, max_muestra_y);
              break;
          }
          time_calc = micros() - time_start;
          while(time_calc<=200000){                      //Esperar a paso por cero de la onda. Poner en caso de no verse bien la onda
            time_calc  = micros() - time_start;
            }
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
          up = 0;                                   //Reset bandera up
          down = 0;                                   //Reset bandera down
        }
      }
      if (salir) {
        salir2 = 1;
      } else {
        TFTscreen.background(0, 0, 0);
        TFTscreen.setTextSize(1);
        switch (VorI) {
          case 0:
            div_x=15; 
            div_y=10;
            inicio_escala_x=5; 
            incr_escala_x=5;
            inicio_escala_y=1; 
            incr_escala_y=1;
            ejes("N arm", "V(%)", div_x, div_y, inicio_escala_x, incr_escala_x, inicio_escala_y, incr_escala_y);
            break;
          case 1:
            div_x=15; 
            div_y=10;
            inicio_escala_x=5; 
            incr_escala_x=5;
            inicio_escala_y=10; 
            incr_escala_y=10;
            ejes("N arm", "I(%)", div_x, div_y, inicio_escala_x, incr_escala_x, inicio_escala_y, incr_escala_y);
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
          numero_muestras = RESET_MUESTRAS;
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
          numero_muestras = RESET_MUESTRAS;
          calc_offset();
          calcVI(Vf_r, If_r, FASE_RATIO_TEMP);
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
          numero_muestras = RESET_MUESTRAS;
          calc_offset();
          calc_FFT(Vf_r, V_mag, V_ang, V_RATIO);
          calc_FFT(If_r, I_mag, I_ang, I_RATIO);
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
          numero_muestras = RESET_MUESTRAS;
          calc_offset();
          calc_FFT(Vf_r, V_mag, V_ang, V_RATIO);
          calc_FFT(If_r, I_mag, I_ang, I_RATIO);
          post_FFT(V_mag, V_ang, I_mag, I_ang);
          calc_offset();
          calcVI(Vf_r, If_r, FASE_RATIO_TEMP);
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
          up = 0;                                   //Reset bandera up
          down = 0;                                   //Reset bandera down
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
          numero_muestras = RESET_MUESTRAS;
          calc_offset(); 
          switch (VorI) {
            case 0:
              calc_FFT(Vf_r, V_mag, V_ang, V_RATIO);
              THDv=THD(V_mag, VrmsFFTTHD);    
              screen_variables_armonicos(THDv, VrmsFFTTHD);
              break;
            case 1:

              calc_FFT(If_r, I_mag, I_ang, I_RATIO);
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

  numero_muestras = RESET_MUESTRAS;
  salir = 0;
  up = 0;                                   //Reset bandera up
  down = 0;                                   //Reset bandera down
  if (posicion == 2 || posicion == 3 || posicion == 4 || salir2) {
    screenmenu(posicion, posicion_anterior);
    ok = 0;                                     //Reset bandera ok
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

void onda_sen(volatile int *onda, int longitud, int min_pixel, int max_pixel, int min_muestra, int max_muestra) {


  int xPos = POS_EJE_Y;
  int muestra;
  int muestra_anterior = map(dc_offset, min_muestra, max_muestra, min_pixel, max_pixel);


  for (int i = 0; i < longitud; i = i + 2) {

    int sensor = onda[i];

    muestra = map(sensor, min_muestra, max_muestra, min_pixel, max_pixel);

    TFTscreen.stroke(0, 0, 0);                            //Color negro
    //TFTscreen.line(xPos, 0, xPos, 118);                   //borrar posicion que vamos a pintar
    TFTscreen.line(xPos + 1, 9, xPos + 1, POS_EJE_X);               //borrar posicion que vamos a pintar
    //TFTscreen.line(xPos + 2, 0, xPos + 2, 118);
    TFTscreen.stroke(255, 255, 255);                      //Blanco *

    TFTscreen.line(xPos, muestra_anterior, xPos+1, muestra); //Representar linea desde posición anterior hasta posicion actual
    muestra_anterior = muestra;

    xPos++;

  }

}

void espectro_frec40(int *frec) {


  int xPos = 34;
  int muestra;
  int amplitud_fundamental = frec[2]/10;

  for (int i = 0; i < PRIMEROS40_ARM; i++) {                                  

    int amplitud = frec[2*i+2];

    if(amplitud>amplitud_fundamental){
      amplitud=amplitud_fundamental;
    }

    muestra = map(amplitud, 0, amplitud_fundamental, POS_EJE_X, POS_EJE_X-100);     //Cambiar segun circuito acondicionamiento

    TFTscreen.stroke(0, 0, 0);                            //Color negro
    TFTscreen.line(xPos, 9, xPos, POS_EJE_X);                   //borrar posicion que vamos a pintar
    TFTscreen.line(xPos + 1, 9, xPos + 1, POS_EJE_X);               //borrar posicion que vamos a pintar
    TFTscreen.stroke(255, 255, 255);                      //Blanco */

    TFTscreen.line(xPos, POS_EJE_X, xPos, muestra);           //Representar amplitud de la frec
    TFTscreen.line(xPos + 1, POS_EJE_X, xPos + 1, muestra);       //Representar amplitud de la frec

    xPos = xPos + 3;


  }

}


void ejes(char *abscisas, char *ordenadas, int xDiv, int yDiv, int escala_x, int incr_escala_x, int escala_y, int incr_escala_y) {
  num_div_y=0;
  char escala_eje[8];
  TFTscreen.text(ordenadas, 34, 0);
  TFTscreen.text(abscisas, 0, 120);
  TFTscreen.line(POS_EJE_Y, 0, POS_EJE_Y, 128);
  TFTscreen.line(0, POS_EJE_X, 160, POS_EJE_X);

  for (int x = POS_EJE_Y + xDiv; x < 160; x = x + xDiv) {
    TFTscreen.line(x, POS_EJE_X, x, 118);
    String escala = String(escala_x);
    escala.toCharArray(escala_eje, 8);
    TFTscreen.text(escala_eje,x-3, 120);
    escala_x += incr_escala_x;
  }

  for (int y = POS_EJE_X - yDiv; y > POS_EJE_X-105; y = y - yDiv) {
    TFTscreen.line(26, y, POS_EJE_Y, y);
    String escala = String(escala_y);
    escala.toCharArray(escala_eje, 8);
    TFTscreen.text(escala_eje,0, y-3);
    escala_y += incr_escala_y;
    num_div_y++;
  }
  
  //Desde POS_EJE_X-yDiv hasta POS_EJE_X - (yDiv*num divisiones) y desde (inicio_escala_y/RATIO)+dc_offset hasta ((inicio_escala_y+(incr_escala_y*num divisiones))/RATIO)+dc_offset
  
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
