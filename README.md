# Monitoramento_Energia
Projeto final da disciplina de Microprocessadores da UFJF

Testes a serem feitos na próxima aula:

Ajustar o LM2596 para 4V;

Ajustar a data e hora do RTC, recarregar o código e ver se vai continuar ajustada;

Conectar as entradas PD0 e PD1 nas entradas TX e RX do SIM800L, ajustar a velocidade e enviar um comando AT

UCSRB = (1 << TXEN) | (1 << RXEN);
UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
UBRRH = (uint8_t)(BAUD_PRESCALLER >> 8);
UBRRL = (uint8_t)(BAUD_PRESCALLER);

        char msg[3] = "AT\r";
        for(int i = 0; i < 3; i++)
        {
            putchar(msg[i]);
        }
        if(rx_message)
        {
            rx_message=0;
            for (i=0, j=rx_counter; i<j ; i++) 
            {
               print(getchar());
            }   
        }
