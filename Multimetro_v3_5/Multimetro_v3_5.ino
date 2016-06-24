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

float V_mag[HALF_FFTSIZE] = { 0};     // valor absoluto de I
float I_mag[HALF_FFTSIZE] = { 0};     // valor absoluto de I
float IrmsFFTTHD[40] = { 0};
float VrmsFFTTHD[40] = { 0};
int posicion = 0;
int posicion_anterior = 0;

float THDv=0, THDi=0;

bool up = 0;
bool down = 0;
bool ok = 0;
bool salir = 0;
bool VorI = 0;

void setup()
{
  Serial.begin (115200);
  Serial.println("Setup...");
  TFTscreen.begin();
  TFTscreen.background(0, 0, 0);
  screenmenu(posicion, posicion_anterior);
  //ratio_setup(248.8, 30.5, 1);
  //adc_setup ();
  Timer3.attachInterrupt(muestreo).setFrequency(6400);
  //active_Boton();
}

Radix4     radix;


void loop()
{
  posicion=5;
  ok=1;
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
    Serial.println("Pongo a 0 pos 1");
    functionmenu(posicion);
  }

}

void muestreo(){
  //ADC->ADC_CR |= 0x02;                       //start ADC
  //while ((ADC->ADC_ISR & 0xC0) == 0x00);
  if (numero_muestras >= 0 && numero_muestras < FFTSIZE) {
    //valorV[numero_muestras] = ADC->ADC_CDR[7]; //Cuidado, corresponde con el pin A0??
    //valorI[numero_muestras] = ADC->ADC_CDR[6]; //Pin A1*/
  }

  numero_muestras++;
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
        ok=1;
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
           // screen_armonicos("Armonicos   THDv=", "V  =       V  V  =       V");
            break;
          case 1:
            //screen_armonicos("Armonicos   THDi=", "I  =       A  I  =       A");
            break;
        }
        Timer3.start();
      }

      while (!salir) {
        if (numero_muestras >= FFTSIZE) {
          Timer3.stop();
          Serial.println("while !salir case 5...");
          //time_start = micros();
          numero_muestras = -10;
          //calc_offset(); 
          switch (VorI) {
            case 0:
              //calc_FFT(Vf_r, V_mag, V_ang, V_RATIO, FASE_RATIO);
              Serial.println("case V");
              Serial.println(ok);
              THD(V_mag, VrmsFFTTHD, THDv);     //ok pasa a 0 sin motivo cada vez que entra
              Serial.println(ok);
              //screen_variables_armonicos(THDv, VrmsFFTTHD);
              break;
            case 1:
              Serial.println("case I: VorI:");
              Serial.println(VorI);
              //calc_FFT(If_r, I_mag, I_ang, I_RATIO, FASE_RATIO);
              THD(I_mag, IrmsFFTTHD, THDi);
              Serial.println(VorI);
              //screen_variables_armonicos(THDi, IrmsFFTTHD);
              break;
          }
          //time_calc  = micros() - time_start;
          if (!salir) {
            Timer3.start();
          }
        }
      }
      Serial.print("\tArmonicos: ");
      //Serial.println(time_calc);
      TFTscreen.background(0, 0, 0);
      break;

  }
  Timer3.stop();

  numero_muestras = -10;
  salir = 0;
  up = 0;
  down = 0;
  if (posicion == 2 || posicion == 3 || posicion == 4 || salir2) {
    screenmenu(posicion, posicion_anterior);
    Serial.println("Pongo a 0 pos 2");
    ok = 0;
  }

}


