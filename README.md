<h1>Projeto com GPS, RTC e RFID</h1> <br>

Este é um projeto que utiliza o módulo GPS, o RTC e o RFID para capturar informações de tempo e localização e ler informações de uma tag RFID.

<h2>Bibliotecas utilizadas</h2>
SPI - Já incluída no Arduino IDE <br>
Wire - Já incluída no Arduino IDE <br>
MFRC522 - Biblioteca para o módulo RFID MFRC522 <br>
RTClib - Biblioteca para o módulo RTC <br>
TinyGPS++ - Biblioteca para o módulo GPS <br>

<h2>Pinagem </h2>
SS_PIN: 21 <br>
RST_PIN: 22<br>
RXD2: 16<br>
TXD2: 17<br>
<h2>Como usar</h2>

Clone o repositório para sua máquina <br>
Abra o arquivo .ino com o Arduino IDE <br>
Conecte seus módulos GPS, RTC e RFID de acordo com a pinagem definida <br>
Carregue o código para sua placa Arduino <br>
Abra o monitor serial e verifique as informações capturadas <br>
<h2>Resultados esperados</h2> 
O programa irá imprimir no monitor serial o NUID da tag RFID quando lida com sucesso, juntamente com a data e hora atual capturadas pelo RTC. Também serão exibidas informações de localização capturadas pelo GPS.
