// Definições dos pinos do LCD
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

// Definições dos pinos dos botões
sbit Button1 at RB0_bit; // Botão para rolar seleção (Bebida / Tamanho)
sbit Button2 at RB1_bit; // Botão para confirmar a seleção

// Função para exibir a saudação
void greetUser() {
  Lcd_Cmd(_LCD_CLEAR);             // Limpa o display
  Lcd_Cmd(_LCD_CURSOR_OFF);        // Cursor off
  Lcd_Out(1, 1, "Bem-Vindo!");    // Exibe a saudação
  Delay_ms(5000);                 // Aguarda 5 segundos
}

char drink2[] = "HEINEKEN     ";
char drink1[] = "BRAHMA       ";
char drink3[] = "EISENBAHN    ";
char drink4[] = "STELLA ARTOIS";
char tam1[] = " Pequena ";
char tam2[] = " Media   ";
char tam3[] = " Grande  ";

// Função para selecionar a bebida ou tamanho
int selectOption(int option) {
  int escolha = 0;
  int cursor = 1; // Inicia com a primeira opção
  Lcd_Cmd(_LCD_CLEAR);

  if (option == 1) {
    Lcd_Out(1, 1, "Escolha Bebida:");
  } else {
    Lcd_Out(1, 1, "Escolha Tamanho:");
  };

  while (1) {
    if (Button1 == 1) { // Rola as opções
      Delay_ms(200); // Debounce
      cursor++;
      if ((option == 1 && cursor > 4) || (option == 2 && cursor > 3)) {
        cursor = 1; // Volta à primeira opção
      }
    }

    if (Button2 == 1) { // Confirma a seleção
      Delay_ms(200); // Debounce
      escolha = cursor;
      break;
    }

    // Exibe as opções no LCD
    if (option == 1) { // Seleção de Bebida
      switch (cursor) {
        case 1: Lcd_Out(2, 1, drink1); break;
        case 2: Lcd_Out(2, 1, drink2); break;
        case 3: Lcd_Out(2, 1, drink3); break;
        case 4: Lcd_Out(2, 1, drink4); break;
      }
    } else { // Seleção de Tamanho
      switch (cursor) {
        case 1: Lcd_Out(2, 1, tam1); break;
        case 2: Lcd_Out(2, 1, tam2); break;
        case 3: Lcd_Out(2, 1, tam3); break;
      }
    }
  }
  return escolha;
}

// Função principal
void main() {
  TRISA = 0x00;  // Configura as portas A como saída para o LCD
  TRISB = 0x03;  // Configura as portas B como entrada para os botões

  Lcd_Init();      // Inicializa o LCD
  greetUser();     // Exibe a saudação ao usuário
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
        RB7_bit = 0;    // Desliga o LED após o tempo
      } else if (bebida == 2) {
        RB6_bit = 1;    // Acende LED para Bebida 2
        Delay_ms(3000); // Aguarda 3 segundos
        RB6_bit = 0;    // Desliga o LED após o tempo
      } else if (bebida == 3) {
        RB5_bit = 1;    // Acende LED para Bebida 3
        Delay_ms(3000); // Aguarda 3 segundos
        RB5_bit = 0;    // Desliga o LED após o tempo
      } else {
        RB4_bit = 1;    // Acende LED para Bebida 4
        Delay_ms(3000); // Aguarda 3 segundos
        RB4_bit = 0;    // Desliga o LED após o tempo
      }
    } else if (tamanho == 2) {
      if (bebida == 1){
        RB7_bit = 1;    // Acende LED para Bebida 1
        Delay_ms(5000); // Aguarda 5 segundos
        RB7_bit = 0;    // Desliga o LED após o tempo
      } else if (bebida == 2) {
        RB6_bit = 1;    // Acende LED para Bebida 2
        Delay_ms(5000); // Aguarda 5 segundos
        RB6_bit = 0;    // Desliga o LED após o tempo
      } else if (bebida == 3) {
        RB5_bit = 1;    // Acende LED para Bebida 3
        Delay_ms(5000); // Aguarda 5 segundos
        RB5_bit = 0;    // Desliga o LED após o tempo
      } else {
        RB4_bit = 1;    // Acende LED para Bebida 4
        Delay_ms(5000); // Aguarda 5 segundos
        RB4_bit = 0;    // Desliga o LED após o tempo
      }
    } else if (tamanho == 3) {
      if (bebida == 1){
        RB7_bit = 1;    // Acende LED para Bebida 1
        Delay_ms(7000); // Aguarda 7 segundos
        RB7_bit = 0;    // Desliga o LED após o tempo
      } else if (bebida == 2) {
        RB6_bit = 1;    // Acende LED para Bebida 2
        Delay_ms(7000); // Aguarda 7 segundos
        RB6_bit = 0;    // Desliga o LED após o tempo
      } else if (bebida == 3) {
        RB5_bit = 1;    // Acende LED para Bebida 3
        Delay_ms(7000); // Aguarda 7 segundos
        RB5_bit = 0;    // Desliga o LED após o tempo
      } else {
        RB4_bit = 1;    // Acende LED para Bebida 4
        Delay_ms(7000); // Aguarda 7 segundos
        RB4_bit = 0;    // Desliga o LED após o tempo
      }
    }
  }
}
