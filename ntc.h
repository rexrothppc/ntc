#ifndef NTC_H_
#define NTC_H_


#ifdef __cplusplus
extern "C" {
#endif


double Ntc47k_calcC(uint32_t adcNtc);
double Ntc47k_calcRt(uint32_t adcNtc);
void Ntc47k_printDouble(double v, int decimalDigits, char *buf, int len);




#ifdef __cplusplus
}
#endif


#endif /* NTC_H_ */
