//file:///C:/Users/%D0%90%D0%B4%D0%BC%D0%B8%D0%BD/Downloads/NTCS0805E3103FMT-1.pdf
//https://www.vishay.com/en/thermistors/ntc-curve-list/
//https://www.vishay.com/en/thermistors/ntc-rt-calculator/
//https://www.miyklas.com.ua/p/geometria/9-klas/dekartovi-koordinati-na-ploshchini-15458/rivniannia-kola-rivniannia-priamoyi-15463/re-6a6fcb99-734f-4ee9-8ead-4a29dc848da8

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include "Views/ugfx_assert.h"


typedef struct _NTC47K
{
  float C;
  float Rt;
} NTC47K_t;

typedef struct _NTC47KPair
{
  const NTC47K_t *pitm1;
  const NTC47K_t *pitm2;
} NTC47KPair_t;

//NTCS0805E3473*HT https://imrad.com.ua/userdata/modules/productFiles/DthFeyW3_ntcs0805e3t.pdf
const NTC47K_t gNTC47k[] = {
// @formatter:off
    {-40, 1536095},//1
    {-35, 1110020},//2
    {-30, 811212},//3
    {-25, 599204},//4
    {-20, 447111},//5
    {-15, 336851},//6
    {-10, 256116},//7
    {-5, 196435},//8
    {0, 151917},//9
    {5, 118422},//10
    {10, 93012},//11
    {15, 73583},//12
    {20, 58615},//13
    {25, 47000},//14
    {30, 37925},//15
    {35, 30788},//16
    {40, 25139},//17
    {45, 20641},//18
    {50, 17038},//19
    {55, 14136},//20
    {60, 11786},//21
    {65, 9872.9},//22
    {70, 8308.1},//23
    {75, 7021.9},//24
    {80, 5959.7},//25
    {85, 5078.7},//26
    {90, 4344.9},//27
    {95, 3731.0},//28
    {100, 3215.5},//29
    {105, 2781.0},//30
    {110, 2413.2 },//31
    {115, 2101.0},//32
    {120, 1834.9},//33
    {125, 1607.3},//34
    {130, 1412.2},//35
    {135, 1244.2},//36
    {140, 1099.3},//37
    {145, 973.81},//38
    {150, 864.87 },//39
    {155, 754.87} //40//dummy, just for pair
            // @formatter:on
    };

NTC47KPair_t Ntc47k_FindWindow(double Rt)
  {
    NTC47KPair_t pair = { 0, };
    const NTC47K_t *pitm = &gNTC47k[0];

    int size = (sizeof gNTC47k) / sizeof(NTC47K_t);
    int idx = 0;
    int safetyCountr = 0;
    while (safetyCountr++ < (size / 2))
      {
        pitm = &gNTC47k[idx];

        if (pitm->Rt <= Rt)
          break;

        ++idx;
      }
    pair.pitm1 = &gNTC47k[idx - 1];
    pair.pitm2 = &gNTC47k[idx];
    return pair;
  }

//Untc = ((3.3V/(47k+Rntc))*47k)/(3,3V/4096)
double Ntc47k_calcRt(uint32_t adcNtc)
  {
    const double R2 = 47000;
    double R1;
    double U = 3.3;
    double U2 = 3.3 * adcNtc / 4096;
    R1 = (U * R2) / U2 - R2;
    return (float) R1;
  }

//https://www.miyklas.com.ua/p/geometria/9-klas/dekartovi-koordinati-na-ploshchini-15458/rivniannia-kola-rivniannia-priamoyi-15463/re-6a6fcb99-734f-4ee9-8ead-4a29dc848da8
//(x - pr.pitm2->Rt)/x2minusx1 = (y - pr.pitm2->C)/y2minus1
//x * y2minus1 - pr.pitm2->Rt * y2minus1 = y * x2minusx1 - pr.pitm2->C * x2minusx1;
//(x * y2minus1 - pr.pitm2->Rt * y2minus1 + pr.pitm2->C * x2minusx1)/x2minusx1 = y;
//y2minus1*x-pr.pitm2->Rt*y2minus1 + pr.pitm2->C*x2minusx1 = y*x2minusx1
static double Ntc47k_calcTempC(NTC47KPair_t pr, double Rt)
  {
    double x2minusx1 = pr.pitm2->Rt - pr.pitm1->Rt;
    double y2minusy1 = pr.pitm2->C - pr.pitm1->C;
    double y = (y2minusy1*Rt-pr.pitm2->Rt*y2minusy1 + pr.pitm2->C*x2minusx1)/x2minusx1;
    return y;
  }

double Ntc47k_calcC(uint32_t adcNtc){
    double Rt = Ntc47k_calcRt(adcNtc);
    NTC47KPair_t pair = Ntc47k_FindWindow(Rt);
    double C = Ntc47k_calcTempC(pair, Rt);
    return C;
}

void Ntc47k_printDouble(double v, int decimalDigits, char *buf, int len)
  {
    int i = 1;
    int intPart, fractPart;

    assert(buf != NULL);
    assert(len > 0);

    while (decimalDigits > 0)
      {
        i *= 10;
        --decimalDigits;
      }

    intPart = (int) v;
    fractPart = (int) ((v - (double) (int) v) * i);
    if (fractPart < 0)
      fractPart *= -1;

    if (v < 0 && intPart == 0)
      snprintf(buf, len, "-%i.%i", intPart, fractPart);
    else
      snprintf(buf, len, "%i.%i", intPart, fractPart);

  }
