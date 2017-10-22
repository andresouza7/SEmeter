# SEmeter Arduino library

Biblioteca para o arduino desenvolvida para o medidor Sicee Energy. O algoritmo usado pressupõe o uso de sensor de tensão capacitivo e
do sensor de corrente ACS712.
Para usar a biblioteca instancie um novo objeto do tipo SEMeter com o número do pino analógico usado para ler, respectivamente:
- a tensão Vcc no ponto mais próximo aos sensores de tensão e de corrente
- a saída analógica do sensor de tensão
- a saída analógica do sensor de corrente

como no exemplo abaixo: 

SEMeter meter(_PinoVcc,_PinoAnalogicoTensao,_PinoAnalogicoCorrente);

Autor: André Souza
