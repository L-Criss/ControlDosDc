#ifndef ControlDosDc_h
#define ControlDosDc_h
#include <Arduino.h>
//String indicador;
//int Vm1=0;
//int Vm2=0;
class ControlDosDc{
 //String indicador;
//int Vm1;
 //int Vm2;


  public: ControlDosDc
(int M1,int M1a,int M1h,int M2a,int M2,int M2h,int XXc, int YYc,int XXa,int YYa,int pasoA,int pasoD,int pasoR);
  void direccion();
  void movimiento();
  void acelerar();
  void condicional();
  void reposo();
  String Vc();
  String Va();
  void imprimir(int V1m,int V2m);
  private: int  _M1, _M1a, _M1h,_M2, _M2a,_M2h,_XXc, _YYc, _XXa, _YYa, _pasoA, _pasoD,_pasoR;
  private: String _indicador;
  private: int _Vm1,_Vm2;
  };
  #endif
