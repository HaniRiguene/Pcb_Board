int i;
const byte enable1=8;
const byte enable2=9;
const byte PIN_SEGMENT_A = 7;
const byte PIN_SEGMENT_B = 6;
const byte PIN_SEGMENT_C = 5;
const byte button=3;
const byte PIN_SEGMENT_D = 4;
const byte PIN_SEGMENT_E = 2;
const byte PIN_SEGMENT_F = 1;
const byte PIN_SEGMENT_G = 0;
const byte PIN_SEGMENT_DP = 8;
const byte PIN_SEGMENT_DP1 = 9;
const byte LUT_ETATS_SEGMENTS[] = {
  0b00111111,
  0b00000110,
  0b01011011,
  0b01001111,
  0b01100110,
  0b01101101,
  0b01111101,
  0b00000111,
  0b01111111,
  0b01101111,
 
};



void setup() {
  
  pinMode(PIN_SEGMENT_A, OUTPUT);

  pinMode(PIN_SEGMENT_B, OUTPUT);

  pinMode(PIN_SEGMENT_C, OUTPUT);
  pinMode(PIN_SEGMENT_D, OUTPUT);
 
  pinMode(PIN_SEGMENT_E, OUTPUT);

  pinMode(PIN_SEGMENT_F, OUTPUT);

  pinMode(PIN_SEGMENT_G, OUTPUT);

  pinMode(PIN_SEGMENT_DP, OUTPUT);

    pinMode(PIN_SEGMENT_DP1, OUTPUT);
  
  pinMode(button,INPUT_PULLUP);
  attachInterrupt(1,event,FALLING);
}


void loop() {
  int v,d;
  v=i%10;
  d=i/10;
  affiche_chiffre_7seg(v,0);
  delay(10);
  affiche_chiffre_7seg(d,1);
  delay(10);
}

void affiche_chiffre_7seg(byte chiffre, byte dp) {

  if (chiffre > 9)
    return; 

  byte segments = LUT_ETATS_SEGMENTS[chiffre];
  digitalWrite(enable1,dp);
  digitalWrite(enable2,!dp);
  digitalWrite(PIN_SEGMENT_A, bitRead(segments, 0));
  digitalWrite(PIN_SEGMENT_B, bitRead(segments, 1));
  digitalWrite(PIN_SEGMENT_C, bitRead(segments, 2));
  digitalWrite(PIN_SEGMENT_D, bitRead(segments, 3));
  digitalWrite(PIN_SEGMENT_E, bitRead(segments, 4));
  digitalWrite(PIN_SEGMENT_F, bitRead(segments, 5));
  digitalWrite(PIN_SEGMENT_G, bitRead(segments, 6));
 
  }
  
  

void event()
{ 
  i=(i+1)%99;
}
void init_port(void) {
    DDRB =0b00000010;
    PORTB=0b00000000;

    DDRC =0b00000000;
    PORTC=0b00000000;

    DDRD =0b01001110;
    PORTD=0b00000000;

    MCUCR |= (1u<<PUD);
    CLKPR = 0b10000111;
    //CLKPR = 0b10000000;
    WDTCSR = (0<<WDE) | (0<<WDIE);
}
void init_uart() {
    UBRR0H = (unsigned char) ((MYUBBR)>>8);
    UBRR0L = (unsigned char) MYUBBR;
    /*Enable receiver and transmitter and enable interrupt*/
    UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}
void USART_send( unsigned char datatosend ) {
    while ( !( UCSR0A & (1<<UDRE0))  );    /* Wait for empty transmit buffer */
    UDR0 = datatosend;                    /* Put data into buffer, sends the data */
}
void USART_putstring(char* StringPtr) { // sends the characters from the string one at a time to the USART    example: USART_putstring("Hello There.");
    while(*StringPtr != 0x00) {            // volatile uint8_t buffer[20];
        USART_send(*StringPtr);            // USART_putstring(buffer);
        StringPtr++;
    }
}

unsigned char USART_receive(void){
    uint16_t timeout=500;
    while( (!(UCSR0A & (1<<RXC0))) && timeout) timeout--;
    if (timeout==0) return 0;
    else return UDR0;
}

