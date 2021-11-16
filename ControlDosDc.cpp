#include <Arduino.h>
#include "ControlDosDc.h"


ControlDosDc::ControlDosDc(int M1,int M1a,int M1h,int M2,int M2a,int M2h,int XXc, int YYc,int XXa,int YYa,int pasoA,int pasoD,int pasoR)
{
  pinMode(M1,OUTPUT);
  pinMode(M1a,OUTPUT);
  pinMode(M1h,OUTPUT);
  pinMode(M2,OUTPUT);
  pinMode(M2a,OUTPUT);
  pinMode(M2h,OUTPUT);
  pinMode(XXc,INPUT);
  pinMode(YYc,INPUT);
  pinMode(XXa,INPUT);
  pinMode(YYa,INPUT);
  _M1=M1; _M1a=M1a; _M1h=M1h; 
  _M2=M2; _M2a=M2a; _M2h=M2h;
  _XXc=XXc;  _YYc=YYc; _XXa=XXa; _YYa=YYa;
  _pasoA=pasoA; _pasoD=pasoD;_pasoR=pasoR; _Vm1=0,_Vm2=0;
}

void ControlDosDc::direccion(){
   if(_Vm1==0&&_Vm2==0){
    _Vm1=1;
    _Vm2=1;
   }   
  if(Vc()=="adelante"){
    analogWrite(_M1,_Vm1);
    digitalWrite(_M1a,HIGH);
    digitalWrite(_M1h,LOW);

    analogWrite(_M2,_Vm2);
    digitalWrite(_M2a,HIGH);
    digitalWrite(_M2h,LOW);
    movimiento();
  }else if(Vc()=="atras"){
    analogWrite(_M1,_Vm1);
    digitalWrite(_M1a,LOW);
    digitalWrite(_M1h,HIGH);
    analogWrite(_M2,_Vm2);
    digitalWrite(_M2a,LOW);
    digitalWrite(_M2h,HIGH);
    movimiento();   
  }else if(Vc()=="izquierda"){
      _Vm1=0;
      analogWrite(_M1,_Vm1);
      digitalWrite(_M1a,LOW);
      digitalWrite(_M1h,LOW);
      analogWrite(_M2,_Vm2);
      digitalWrite(_M2a,HIGH);
      digitalWrite(_M2h,LOW);
      movimiento();
  }else if(Vc()=="derecha"){
      analogWrite(_M1,_Vm1);
      digitalWrite(_M1a,HIGH);
      digitalWrite(_M1h,LOW);
      _Vm2=0;
      analogWrite(_M2,_Vm2);
      digitalWrite(_M2a,LOW);
      digitalWrite(_M2h,LOW);
      movimiento();
  }else if(Vc()=="diaDerA"){
      _Vm2=int(_Vm2/2);
      analogWrite(_M2,_Vm2);
      digitalWrite(_M2a,HIGH);
      digitalWrite(_M2h,LOW);
      imprimir(_Vm1,_Vm2);
      movimiento();
   }else if(Vc()=="diaIzqA"){
      _Vm1=int(_Vm1/2);
      analogWrite(_M1,_Vm1);
      digitalWrite(_M1a,HIGH);
      digitalWrite(_M1h,LOW);
      imprimir(_Vm1,_Vm2);
      movimiento();
          
    }else if(Vc()=="diaDerR"){
      _Vm2=int(_Vm2/2);
      analogWrite(_M2,_Vm2);
      digitalWrite(_M2a,LOW);
      digitalWrite(_M2h,HIGH);
      imprimir(_Vm1,_Vm2);
      movimiento(); 

    }else if(Vc()=="diaIzqR"){
      _Vm1=int(_Vm1/2);
      analogWrite(_M1,_Vm1);
      digitalWrite(_M1a,LOW);
      digitalWrite(_M1h,HIGH);
      imprimir(_Vm1,_Vm2);
      movimiento();   
    }    
}

void ControlDosDc::movimiento(){
  if(Va()!="aNeutro"){
    acelerar();
  }else if(Va()=="aNeutro"){
    _indicador=Vc();
    String recuerdo=Vc();
    while(Va()=="aNeutro" && Vc()==recuerdo){
      if(_Vm1<10&&_Vm2<10){
        if(_Vm1!=0){
          _Vm1=_Vm1+_pasoA;
        }
        if(_Vm2!=0){
          _Vm2=_Vm2+_pasoA;
        }
      }
      imprimir(_Vm1,_Vm2);    
    }
    condicional();
  }
}

void ControlDosDc::acelerar(){
  _indicador=Vc();
  if(Va()=="desacelerar"){
    String recuerdo=Vc();
    _indicador=Vc();
    while(Va()=="desacelerar" && Vc()==recuerdo && (_Vm1>0||_Vm2>0)){
      if(_Vm1!=0&&_Vm1>0){
        _Vm1=_Vm1-_pasoD; 
      }
      if(_Vm2!=0&&_Vm2>0){
        _Vm2=_Vm2-_pasoD;
      }
      analogWrite(_M1,_Vm1);
      analogWrite(_M2,_Vm2);
     // Serial.println(_Vm1);
      //Serial.println(_Vm2);
      imprimir(_Vm1,_Vm2); 
    }
    condicional();
  }else if(Va()=="acelerar"){
    _indicador=Vc();
    String recuerdo=Vc();
    while(Va()=="acelerar" && recuerdo==Vc()){
      if(_Vm1<255||_Vm2<255){
  
        if(_Vm1!=0&&_Vm1<255){
          _Vm1=_Vm1+_pasoA;

        }
        if(_Vm2!=0&&_Vm2<255){
          _Vm2=_Vm2+_pasoA;
        }
      }
      analogWrite(_M1,_Vm1);
      analogWrite(_M2,_Vm2);
      //Serial.println(_Vm1);
      //Serial.println(_Vm2);
      imprimir(_Vm1,_Vm2); 
    }
    condicional();
  } 
}
void ControlDosDc::condicional(){
  if(_indicador=="adelante"||_indicador=="diaDerA"||_indicador=="diaIzqA"){
    String a=Vc();
    if(a=="derecha"){
      _Vm2=0;
      analogWrite(_M2,_Vm2);
      digitalWrite(_M2a,LOW);
      digitalWrite(_M2h,LOW);
      imprimir(_Vm1,_Vm2);
      movimiento();
    }else if(a=="izquierda"){
      _Vm1=0;
      analogWrite(_M1,_Vm1);
      digitalWrite(_M1a,LOW);
      digitalWrite(_M1h,LOW);
      imprimir(_Vm1,_Vm2);
      movimiento();
    }else if(a=="atras"){
       if(_Vm1>_Vm2){
        _Vm2=_Vm1;
      }else if(_Vm2>_Vm1){
        _Vm1=_Vm2;   
      }
      analogWrite(_M1,_Vm1);
      analogWrite(_M2,_Vm2);
      reposo();
    }else if(a=="adelante"){
      if(_Vm1>_Vm2){
        _Vm2=_Vm1;
      }else if(_Vm2>_Vm1){
        _Vm1=_Vm2;
        
      }
      analogWrite(_M1,_Vm1);
      analogWrite(_M2,_Vm2);
      movimiento();
    }else if(a=="cNeutro"){
      if(_Vm1>_Vm2){
        _Vm2=_Vm1;
      }else if(_Vm2>_Vm1){
        _Vm1=_Vm2;
      }
      analogWrite(_M1,_Vm1);
      analogWrite(_M2,_Vm2);
      reposo();
    }else if(a=="diaDerA"){
      _Vm2=int(_Vm2/2);
      analogWrite(_M2,_Vm2);
      digitalWrite(_M2a,HIGH);
      digitalWrite(_M2h,LOW);
      imprimir(_Vm1,_Vm2);
      movimiento();
    }else if(a=="diaIzqA"){
      _Vm1=int(_Vm1/2);
      analogWrite(_M1,_Vm1);
      digitalWrite(_M1a,HIGH);
      digitalWrite(_M1h,LOW);
      imprimir(_Vm1,_Vm2);
      movimiento();
          
    }else{
       if(_Vm1>_Vm2){
        _Vm2=_Vm1;
      }else if(_Vm2>_Vm1){
        _Vm1=_Vm2;  
      }
      analogWrite(_M1,_Vm1);
      analogWrite(_M2,_Vm2);
    reposo();
    }
  }else if(_indicador=="atras"||_indicador=="diaDerR"||_indicador=="diaIzqR"){
    String a=Vc();

    if(a=="derecha"){
      _Vm2=0;
      analogWrite(_M2,_Vm2);
      digitalWrite(_M2a,LOW);
      digitalWrite(_M2h,LOW);
      imprimir(_Vm1,_Vm2);
      movimiento(); 
    }else if(a=="izquierda"){
      _Vm1=0;
      analogWrite(_M1,_Vm1);
      digitalWrite(_M1a,LOW);
      digitalWrite(_M1h,LOW);
      imprimir(_Vm1,_Vm2);
      movimiento(); 
    }else if(a=="atras"){
       if(_Vm1>_Vm2){
        _Vm2=_Vm1;
      }else if(_Vm2>_Vm1){
        _Vm1=_Vm2;     
      }
      analogWrite(_M1,_Vm1);
      analogWrite(_M1,_Vm2);
      movimiento();
    }else if(a=="adelante"){
      if(_Vm1>_Vm2){
        _Vm2=_Vm1;
      }else if(_Vm2>_Vm1){
        _Vm1=_Vm2;
      }
      analogWrite(_M1,_Vm1);
      analogWrite(_M2,_Vm2);
      reposo();
    }else if(a=="cNeutro"){
       if(_Vm1>_Vm2){
        _Vm2=_Vm1;
      }else if(_Vm2>_Vm1){
        _Vm1=_Vm2;
      }
      analogWrite(_M1,_Vm1);
      analogWrite(_M2,_Vm2);
      reposo();
    }else if(a=="diaDerR"){
      _Vm2=int(_Vm2/2);
      analogWrite(_M2,_Vm2);
      digitalWrite(_M2a,LOW);
      digitalWrite(_M2h,HIGH);
      imprimir(_Vm1,_Vm2);
      movimiento(); 

    }else if(a=="diaIzqR"){
      _Vm1=int(_Vm1/2);
      analogWrite(_M1,_Vm1);
      digitalWrite(_M1a,LOW);
      digitalWrite(_M1h,HIGH);
      imprimir(_Vm1,_Vm2);
      movimiento();
      
    }else{
      if(_Vm1>_Vm2){
        _Vm2=_Vm1;
      }else if(_Vm2>_Vm1){
        _Vm1=_Vm2;
      }
      analogWrite(_M1,_Vm1);
      analogWrite(_M2,_Vm2);
      reposo();
    } 
  }
}

void ControlDosDc::reposo(){
  String recuerdo1=Vc();
  String recuerdo2=Va();
  while(recuerdo1==Vc() && recuerdo2==Va() && (_Vm1>0||_Vm2>0)){
    if(_Vm1!=0&&_Vm1>=5){
      _Vm1=_Vm1-5*_pasoR;
    }else if(_Vm1!=0&&_Vm1>0&&_Vm1<5){
      _Vm1=_Vm1-_pasoR;  
    }
    if(_Vm2!=0&&_Vm2>=5){
      _Vm2=_Vm2-5*_pasoR;
    }else if(_Vm2!=0&&_Vm2>0&&_Vm2<5){
      _Vm2=_Vm2-_pasoR; 
    }
    analogWrite(_M1,_Vm1);
    analogWrite(_M2,_Vm2);
    imprimir(_Vm1,_Vm2); 
  }
  if(_Vm1==0&&_Vm2==0){
    _indicador="cNeutro";
    analogWrite(_M1,0);
    analogWrite(_M2,0);
    digitalWrite(_M1a,LOW);
    digitalWrite(_M1h,LOW);
    digitalWrite(_M2a,LOW);
    digitalWrite(_M2h,LOW);
    imprimir(_Vm1,_Vm2);  
  }else if(_Vm1!=0||_Vm2!=0){
    condicional();
  }
}

String ControlDosDc::Vc(){
  int X=analogRead(_XXc);
  int Y=analogRead(_YYc);
  String controlador;
  if(Y>612&&X>412 && X<612){
    controlador="adelante";
  }else if(Y<412 && X>412 && X<612){
    controlador="atras";
  }else if(X<412 && Y>412 && Y<612){
    controlador="izquierda";
  }else if(X>612 && Y>412 && Y<612){
    controlador="derecha";
  }else if(X>412 && X<612 && Y>412 && Y<612){
    controlador="cNeutro";
  }else if(Y>612&&X>612){
    controlador="diaDerA";
  }else if(Y>612&&X<412){
    controlador="diaIzqA";
  }else if(X>612&&Y<412){
    controlador="diaDerR";
  }else if(X<412&&Y<412){
    controlador="diaIzqR";
  }
  return controlador;
}

String ControlDosDc::Va(){
 int Xa=analogRead(_XXa);
 int Ya=analogRead(_YYa);

  boolean acelerar=(Ya>612 && Ya<1023 && Ya>Xa && Ya>-Xa+1023)||(Ya>612 && Ya<1023&&Xa>412&& Xa<612);
  boolean desacelerar=(Ya>0 && Ya<412 && Ya<Xa&& Ya<-Xa+1023)||(Ya>0 && Ya<412&&Xa>412 && Xa<612);
  boolean aNeutro=(Xa>412 && Xa<612 && Ya<612 && Ya>412);
  boolean izquierda=(Xa>0 && Xa<412 && Ya>Xa&& Ya<-Xa+1023)||(Xa>0 && Xa<412&&Ya>412 && Ya<612);
  boolean derecha=(Xa>612 && Xa<1023 && Ya<Xa && Ya>-Xa+1023)||(Xa>612 && Xa<1023&&Ya>412 && Ya<612);
  String acelerador;
  if(acelerar){
    acelerador="acelerar";
  }else if(desacelerar){
    acelerador="desacelerar";
  }else if(aNeutro||izquierda||derecha){
    acelerador="aNeutro";
  }
  return acelerador;
}

void ControlDosDc::imprimir(int _vm1, int _vm2){
  /*lcd.setCursor(0,0);
  lcd.print("Vm1:");
  lcd.setCursor(4,0);
  lcd.print("   ");
  lcd.setCursor(4,0);
  lcd.print(_vm1);
  
  lcd.setCursor(0,1);
  lcd.print("Vm2:");
  lcd.setCursor(4,1);
  lcd.print("   ");
  lcd.setCursor(4,1);
  lcd.print(_vm2);
  
  lcd.setCursor(8,0);
  lcd.print("          ");
  lcd.setCursor(8,0);
  lcd.print(Vc());
  //lcd.clear();
  lcd.setCursor(8,1);
  lcd.print("          ");
  lcd.setCursor(8,1);
  lcd.print(Va());*/
  
  Serial.print("Velocidad M1: ");
  Serial.print(_Vm1);
  Serial.print("  --->  ");
  Serial.print(Vc());
  Serial.print("  |  Velocidad M2: ");
  Serial.print(_Vm2);
  Serial.print("  --->  ");
  Serial.print(Vc());
  Serial.print("  (");Serial.print(Va());Serial.println(")");
/*Serial.print(_Vm1);Serial.print(",");Serial.print(_Vm2);Serial.print(",");Serial.println(Vc());*/
}
