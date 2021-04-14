# 8266 Alexa fauxmoESP

Questo sketch consente di controllare lampade tramite Alexa senza l'utilizzo di Skill particolari.

E', inoltre, possibile l'aggiornamento tramite OTA ed il debug remoto tramite telnet.

Il progetto prevede, oltre al controllo con Alexa, anche la possibilità di controllare il gruppo lampade tramite un interruttore touch che può essere occultato dietro un tappo della cassetta interruttori consentendo, quindi, modifiche minime al preesistente impianto.

## Hardware usato

<img title="" src="http://connt.net/wp-content/uploads/2018/08/Wemos-D1-Mini-02-1.jpg" alt="Wemos D1 Mini ESP8266 - Cos'è e come utilizzarlo - Parte 1" width="197" data-align="inline"> Wemos D1 mini<img title="" src="https://cdn.shopify.com/s/files/1/2386/9605/products/ttp-223-latching-momentary-touch-button-pmdway-6_755x516.jpg?v=1572235004" alt="TTP223 Latching/Momentary Single Channel Touch Sensor - Five Pack — PMD Way" width="200" data-align="inline">TTP223 Touch

<img src="https://www.el-component.com/images/bipolar-transistor/bc237-pinout.jpg" title="" alt="BC237 npn transistor complementary pnp, replacement, pinout, pin  configuration, substitute, equivalent smd, datasheet" width="187"> transistor BC237<img src="https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcR-KqvOLp0MkWdyo0RIoHwiq8m_3ZN8zrGKTzLoAVzvkzkGAp5JglejykeWL5ZLG-2DtSgjDoM&usqp=CAc" title="" alt="" width="166">Relé HF41F

<img title="" src="https://imgaz3.staticbg.com/thumb/large/oaupload/banggood/images/97/5E/fc0f44f8-5561-4019-a606-8b86460e8605.jpg.webp" alt="" width="152">AC-DC 5V 700mA 3.5W<img src="data:image/jpeg;base64,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" title="" alt="" width="113"> 1N4001

<img src="https://upload.wikimedia.org/wikipedia/commons/6/62/560_ohms_5%25_axial_resistor.jpg" title="" alt="File:560 ohms 5% axial resistor.jpg - Wikimedia Commons" width="118">resistenza 520 ohm

## Schema elettrico

lo schema elettrico è presentato qui di seguito. Allo scopo di ridurre le dimensioni del circuito si è utilizzato un relé HF41F di ridotte dimensioni ma con capace di soli 6 ampere, per correnti maggiori occorre utilizzare altri tipi di relè nella vasta gamma di relé disponibili sul mercato.

La resistenza R1 è indicativa poiché la corrente che il transistor Q1 deve erogare dipende dal tipo di relè scelto. In alternativa è possibile utilizzare uno shield relè che integra il gruppo R1,D1,Q1,Relay.

<img title="" src="file:///home/gennaro/.var/app/com.github.marktext.marktext/config/marktext/images/2021-04-13-15-57-49-image.png" alt="" width="834">

## La realizzazione

![](/home/gennaro/.var/app/com.github.marktext.marktext/config/marktext/images/2021-04-13-19-45-36-image.png)

## Il codice

Nel codice sono riportate svariate note esplicative per cui per un approfondimento dello sketch si rimanda alle note del codice.

Per utilzzare lo sketch occorre modificare alcune righe:

1) editare le due righe per inserire ssid e password della propria rete

```
const char* ssid ="xxxxx";
const char* password = "yyyyy";
```

2) sostituite la parola test con il nome del dispositivo da controllare

```
#define LAMPADA1 "test"
```

Qualora non si voglia usare il debug remoto commentare la riga

```
#define DEBUG_DISABLED
```

Ricordare, infine, che per connettersi con il debug remoto si potrà usare il collegamento al nome attribuito al punto 2, nel caso in esame

```
telnet test
```
