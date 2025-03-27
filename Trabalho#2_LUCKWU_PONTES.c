// Defini��es dos pinos do LCD
sbit LCD_RS at RA2_bit;
sbit LCD_EN at RA3_bit;
sbit LCD_D4 at RA4_bit;
sbit LCD_D5 at RA5_bit;
sbit LCD_D6 at RA6_bit;
sbit LCD_D7 at RA7_bit;
sbit LCD_RS_Direction at TRISA2_bit;
sbit LCD_EN_Direction at TRISA3_bit;
sbit LCD_D4_Direction at TRISA4_bit;
sbit LCD_D5_Direction at TRISA5_bit;
sbit LCD_D6_Direction at TRISA6_bit;
sbit LCD_D7_Direction at TRISA7_bit;

// Defini��es dos pinos dos bot�es
sbit Button1 at RB0_bit; // Bot�o para rolar sele��o (Bebida / Tamanho)
sbit Button2 at RB1_bit; // Bot�o para confirmar a sele��o

// Fun��o para exibir a sauda��o
void greetUser() {
  Lcd_Cmd(_LCD_CLEAR);             // Limpa o display
  Lcd_Cmd(_LCD_CURSOR_OFF);        // Cursor off
  Lcd_Out(1, 1, "Bem-Vindo!");    // Exibe a sauda��o
  Delay_ms(5000);                 // Aguarda 5 segundos
}

char drink2[] = "HEINEKEN     ";
char drink1[] = "BRAHMA       ";
char drink3[] = "EISENBAHN    ";
char drink4[] = "STELLA ARTOIS";
char tam1[] = " Pequena ";
char tam2[] = " Media   ";
char tam3[] = " Grande  ";

// Fun��o para selecionar a bebida ou tamanho
int selectOption(int option) {
  int escolha = 0;
  int cursor = 1; // Inicia com a primeira op��o
  Lcd_Cmd(_LCD_CLEAR);

  if (option == 1) {
    Lcd_Out(1, 1, "Escolha Bebida:");
  } else {
    Lcd_Out(1, 1, "Escolha Tamanho:");
  };

  while (1) {
    if (Button1 == 1) { // Rola as op��es
      Delay_ms(200); // Debounce
      cursor++;
      if ((option == 1 && cursor > 4) || (option == 2 && cursor > 3)) {
        cursor = 1; // Volta � primeira op��o
      }
    }

    if (Button2 == 1) { // Confirma a sele��o
      Delay_ms(200); // Debounce
      escolha = cursor;
      break;
    }

    // Exibe as op��es no LCD
    if (option == 1) { // Sele��o de Bebida
      switch (cursor) {
        case 1: Lcd_Out(2, 1, drink1); break;
        case 2: Lcd_Out(2, 1, drink2); break;
        case 3: Lcd_Out(2, 1, drink3); break;
        case 4: Lcd_Out(2, 1, drink4); break;
      }
    } else { // Sele��o de Tamanho
      switch (cursor) {
        case 1: Lcd_Out(2, 1, tam1); break;
        case 2: Lcd_Out(2, 1, tam2); break;
        case 3: Lcd_Out(2, 1, tam3); break;
      }
    }
  }
  return escolha;
}

// Fun��o principal
void main() {
  TRISA = 0x00;  // Configura as portas A como sa�da para o LCD
  TRISB = 0x03;  // Configura as portas B como entrada para os bot�es

  Lcd_Init();      // Inicializa o LCD
  greetUser();     // Exibe a sauda��o ao usu�rio
  Lcd_Cmd(_LCD_CLEAR);
  
  while (1) {
    int bebida = selectOption(1); // Seleciona a bebida
    int tamanho = selectOption(2); // Seleciona o tamanho

    // Exibe a bebida e o tamanho selecionados
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1, 1, "Bebida:");
    switch (bebida) {
        case 1: Lcd_Out(2, 1, drink1); break;
        case 2: Lcd_Out(2, 1, drink2); break;
        case 3: Lcd_Out(2, 1, drink3); break;
        case 4: Lcd_Out(2, 1, drink4); break;
    }
    // Acende o LED da bebida pelo tempo do tamanho da bebida
    if (tamanho == 1) {
      if (bebida == 1){
        RB7_bit = 1;    // Acende LED para Bebida 1
        Delay_ms(3000); // Aguarda 3 segundos
        RB7_bit = 0;    // Desliga o LED ap�s o tempo
      } else if (bebida == 2) {
        RB6_bit = 1;    // Acende LED para Bebida 2
        Delay_ms(3000); // Aguarda 3 segundos
        RB6_bit = 0;    // Desliga o LED ap�s o tempo
      } else if (bebida == 3) {
        RB5_bit = 1;    // Acende LED para Bebida 3
        Delay_ms(3000); // Aguarda 3 segundos
        RB5_bit = 0;    // Desliga o LED ap�s o tempo
      } else {
        RB4_bit = 1;    // Acende LED para Bebida 4
        Delay_ms(3000); // Aguarda 3 segundos
        RB4_bit = 0;    // Desliga o LED ap�s o tempo
      }
    } else if (tamanho == 2) {
      if (bebida == 1){
        RB7_bit = 1;    // Acende LED para Bebida 1
        Delay_ms(5000); // Aguarda 5 segundos
        RB7_bit = 0;    // Desliga o LED ap�s o tempo
      } else if (bebida == 2) {
        RB6_bit = 1;    // Acende LED para Bebida 2
        Delay_ms(5000); // Aguarda 5 segundos
        RB6_bit = 0;    // Desliga o LED ap�s o tempo
      } else if (bebida == 3) {
        RB5_bit = 1;    // Acende LED para Bebida 3
        Delay_ms(5000); // Aguarda 5 segundos
        RB5_bit = 0;    // Desliga o LED ap�s o tempo
      } else {
        RB4_bit = 1;    // Acende LED para Bebida 4
        Delay_ms(5000); // Aguarda 5 segundos
        RB4_bit = 0;    // Desliga o LED ap�s o tempo
      }
    } else if (tamanho == 3) {
      if (bebida == 1){
        RB7_bit = 1;    // Acende LED para Bebida 1
        Delay_ms(7000); // Aguarda 7 segundos
        RB7_bit = 0;    // Desliga o LED ap�s o tempo
      } else if (bebida == 2) {
        RB6_bit = 1;    // Acende LED para Bebida 2
        Delay_ms(7000); // Aguarda 7 segundos
        RB6_bit = 0;    // Desliga o LED ap�s o tempo
      } else if (bebida == 3) {
        RB5_bit = 1;    // Acende LED para Bebida 3
        Delay_ms(7000); // Aguarda 7 segundos
        RB5_bit = 0;    // Desliga o LED ap�s o tempo
      } else {
        RB4_bit = 1;    // Acende LED para Bebida 4
        Delay_ms(7000); // Aguarda 7 segundos
        RB4_bit = 0;    // Desliga o LED ap�s o tempo
      }
    }
  }
}
