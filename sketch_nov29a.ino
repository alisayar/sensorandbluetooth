// 1=49 ... 9=57 // 1 gelirse dediğimiz veri bu tarafta 49 a tekabül eder. Yani Decimali gelir
//1 - 49(Decimal)  gelirse sürekli ileri yap      - Klavyede ileri tuşu basılı tutulunca gelen veri
//2 - 50(Decimal) gelirse sürekli ileriyi durdur - Klavyede ileri tuşu basılı olmadan çekilirse gelen veri

//3 - 51(Decimal) gelirse sürekli geri yap      - Klavyede geri tuşu basılı tutulunca gelen veri
//4 - 52(Decimal) gelirse sürekli geriyi durdur - Klavyede geri tuşu basılı olmadan çekilirse gelen veri

//-----

//5 - 53(Decimal) gelirse sağa dön - Klavyede sağ yön tuşuna basılırsa gelicek değer
//6 - 54(Decimal) gelirse sağa dön iptal - Klavyede sol yön tuşuna basılırsa gelicek değer

//7 - 55(Decimal) gelirse sola dön - Klavyede sağ yön tuşuna basılırsa gelicek değer
//8 - 56(Decimal) gelirse sola dön iptal - Klavyede sol yön tuşuna basılırsa gelicek değer


#include<SoftwareSerial.h>
SoftwareSerial bt_com(50,51);


//ultrasonik mesafe sensör pinleri
int Trig_Pin = 6; //Trig-pin 
int Echo_Pin = 7; //Echo-pin
//----------------

int distance_range=4; //minimum fren mesafesi
boolean wall=false; //engel varlığının kontrol değişkeni
boolean ileri=false;
boolean geri =false;
boolean sag  =false;
boolean sol  =false;
int incomingByte=0;
int updownpinone=8;
int updownpintwo=9;
int leftrightpinone=10;
int leftrightpintwo=11;
int powerofcar = 255; //arabayı harekete geçirecek güç // !!! 255 altına düşmesi karta zarar verir

void setup() {
  bt_com.begin(9600);
  pinMode(updownpinone,OUTPUT);//ileri geri pinleri
  pinMode(updownpintwo,OUTPUT);//ileri geri pinleri 
  pinMode(leftrightpinone,OUTPUT);//sagsol  pinleri 
  pinMode(leftrightpintwo,OUTPUT);//sagsol  pinleri 

//ultrasonik mesafe sensörü için
  pinMode(Trig_Pin, OUTPUT); // ses dalgası çıkış
  pinMode(Echo_Pin, INPUT); // ses dalgası giriş
  delay(3000);
}

void loop() {

      

        
       // wallControl(); 
        
  // eğer veri geldiyse oku - hangi veri geldiyse ona göre uygun işlemi burada çalıştırıyoruz
        if (bt_com.available() > 0) {
              
                // gelen veri
                incomingByte = bt_com.read();
                if(incomingByte==49){//ileri gitme
                  if(wall) {
                    ileri=false;
                    fren();
                  }
                  else{
                    ileri=true;
                    ilerigit();
                  }
                  
                }else if(incomingByte==50){//ileri gitme iptal
                  ileri=false;
                  ilerigit();
                }else if(incomingByte==51){//geri gitme
                  geri=true;
                  gerigit();
                }else if(incomingByte==52){//geri gitme iptal
                  geri=false;
                  gerigit();
                }

                // ileri vey geri giderken sağa veya sola dönme kabiliyetini sağlamak
                // için else if olarak değil de ayrı bir if olarak yapıldı.
                // gelen veri
               
                if(incomingByte==53){//sağa dönme
                  sag=true;
                  sagadon();
                }else if(incomingByte==54){//sağa dönme iptal
                  sag=false;
                  sagadon();
                }else if(incomingByte==55){//sola dönme
                  sol=true;
                  soladon();
                }else if(incomingByte==56){//sola dönme iptal
                  sol=false;
                  soladon();
                }


                
                // gelen veriyi bastık - ihtiyac yok görmek için
                //bt_com.print("I received: ");
                //bt_com.println(incomingByte, DEC);
        } 
}

void fren(){
  analogWrite(updownpinone,0);
  analogWrite(updownpintwo,0);
}


//arabanın saga dönmesini sağlar
void sagadon(){
  if(sag){
     analogWrite(leftrightpinone,0);
     analogWrite(leftrightpintwo,powerofcar);
  }else{
     analogWrite(leftrightpinone,0);
     analogWrite(leftrightpintwo,0);
  }
}

//arabanın sola dönmesini sağlar
void soladon(){
  if(sol){
     analogWrite(leftrightpinone,powerofcar);
     analogWrite(leftrightpintwo,0);
  }else{
     analogWrite(leftrightpinone,0);
     analogWrite(leftrightpintwo,0);
  }
}

//arabanın ileri gitmesini sağlar 
void ilerigit(){
    if(ileri){
     analogWrite(updownpinone,0);
     analogWrite(updownpintwo,powerofcar);
    }else{
       analogWrite(updownpinone,0);
       analogWrite(updownpintwo,0);
    }
  
  
}

//arabanın geri gitmesini sağlar
void gerigit(){
  if(geri){
     analogWrite(updownpinone,powerofcar);
     analogWrite(updownpintwo,0);
  }else{
      //geri gitme iptal edilirken ileri gitmeyi etkilememesi için 
      if(!ileri){
        analogWrite(updownpinone,0);
        analogWrite(updownpintwo,0); 
      }
  }
}

boolean wallControl(){
  long sure,mesafe;
  digitalWrite(Trig_Pin, LOW); //Trigden süre tutup ses sinyali gönderiyor
  //delayMicroseconds(1);
  digitalWrite(Trig_Pin, HIGH);
  //delayMicroseconds(1);
  digitalWrite(Trig_Pin, LOW);
  sure = pulseIn(Echo_Pin, HIGH);
  mesafe = (sure/2)/29.2; // giden ve gelen olarak süreyi 2'ye bölüyoruz
  //Serial.println(mesafe);
  if(mesafe <= distance_range){
    wall=true;
  }else{
    wall=false;
  }
  //delay(2);
  return true;
 }





