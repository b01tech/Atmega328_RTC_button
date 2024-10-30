#include <LiquidCrystal.h> 
#include <Wire.h> 
#include "RTClib.h" 
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;//Pinos para ligar o display
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);//Define os pinos que serão usados para ligar o display
RTC_DS3231 rtc; 
char tecla = ' ';

void setup() {
  initDisplay();
  initRTC();  
}

void loop() {
  tecla = lerTecla();
  DateTime data = rtc.now();

  if (tecla == '#') { 
    ajustarDataHora(data);
  } else {
    escreverData(data);
  }

  delay(50);
}

void initDisplay(){
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("INICIANDO");
  delay(500);
  lcd.clear();
}

void initRTC(){
  if (!rtc.begin()) {
    lcd.print("Erro no RTC");
    while (1); //Falha
  }

  if(rtc.lostPower()){
    rtc.adjust(DateTime(2000,01,01,00,00,00)); // data reset
  }
}

char lerTecla() {
  int leitura = analogRead(0); 
  delay(10);

  if (leitura < 100) {       // direita
    return '>';
  } else if (leitura < 200) { // cima
    return '^';
  } else if (leitura < 400) { // baixo
    return 'v';
  } else if (leitura < 600) { // esquerda
    return '<';
  } else if (leitura < 800) { // select
    return '#';
  } else {
    return ' ';
  }
}

void escreverData(DateTime data) {
  lcd.setCursor(0, 0);
  lcd.print(data.day());
  lcd.print("/");
  lcd.print(data.month());
  lcd.print("/");
  lcd.print(data.year());
  
  lcd.setCursor(0, 1);
  lcd.print(data.hour());
  lcd.print(":");
  if (data.minute() < 10) lcd.print("0");
  lcd.print(data.minute());
  lcd.print(":");
  if (data.second() < 10) lcd.print("0");
  lcd.print(data.second());
}

void ajustarDataHora(DateTime data) {
  int dia = data.day(), mes = data.month(), ano = data.year(), hora = data.hour(), minuto = data.minute(), segundo = data.second();
  int campo = 0; // 0 - dia, 1 - mês, 2 - ano, 3 - hora, 4 - minuto, 5 - segundo
  bool configurando = true;

  lcd.clear();
  lcd.print("Ajuste Data/Hora");
  delay(750);
  lcd.clear();

  while (configurando) {
    lcd.setCursor(0, 0);
    if(campo == 0) lcd.print("*");
    lcd.print(dia);
    lcd.print("/");
    if(campo == 1) lcd.print("*");
    lcd.print(mes);
    lcd.print("/");
    if(campo == 2) lcd.print("*");
    lcd.print(ano);
    lcd.print("   ");
    lcd.setCursor(0, 1);
    if(campo == 3) lcd.print("*");
    lcd.print(hora);
    lcd.print(":");
    if(campo == 4) lcd.print("*");
    if (minuto < 10) lcd.print("0");
    lcd.print(minuto);
    lcd.print(":");
    if(campo == 4) lcd.print("*");
    if (segundo < 10) lcd.print("0");
    lcd.print(segundo);
    lcd.print("   ");

    char tecla = lerTecla();

    if (tecla == '>') { 
      campo = (campo + 1) % 6;
    } else if (tecla == '<') { 
      campo = (campo - 1 + 6) % 6;
    } else if (tecla == '^') { 
      incrementarValor(campo, dia, mes, ano, hora, minuto, segundo);
    } else if (tecla == 'v') { 
      decrementarValor(campo, dia, mes, ano, hora, minuto, segundo);
    } else if (tecla == '#') { 
      rtc.adjust(DateTime(ano, mes, dia, hora, minuto, segundo));
      configurando = false;
    }

    delay(200);
  }

  lcd.clear();
  lcd.print("Data/Hora ajustada!");
  delay(1000);
  lcd.clear();
}

void incrementarValor(int campo, int &dia, int &mes, int &ano, int &hora, int &minuto, int &segundo) {
  switch (campo) {
    case 0: dia = (dia % 31) + 1; break;
    case 1: mes = (mes % 12) + 1; break;
    case 2: ano++; break;
    case 3: hora = (hora + 1) % 24; break;
    case 4: minuto = (minuto + 1) % 60; break;
    case 5: segundo = (segundo + 1) % 60; break;
  }
}

void decrementarValor(int campo, int &dia, int &mes, int &ano, int &hora, int &minuto, int &segundo) {
  switch (campo) {
    case 0: dia = (dia == 1) ? 31 : dia - 1; break;
    case 1: mes = (mes == 1) ? 12 : mes - 1; break;
    case 2: ano--; break;
    case 3: hora = (hora == 0) ? 23 : hora - 1; break;
    case 4: minuto = (minuto == 0) ? 59 : minuto - 1; break;
    case 5: segundo = (segundo == 0) ? 59 : segundo - 1; break;
  }
}


