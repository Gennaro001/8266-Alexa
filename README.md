# 8266 Alexa fauxmoESP

Questo sketch consente di controllare lampade tramite Alexa senza l'utilizzo di Skill particolari.

E', inoltre, possibile l'aggiornamento tramite OTA ed il debug remoto tramite telnet.

Il progetto prevede, oltre al controllo con Alexa, anche la possibilità di controllare il gruppo lampade tramite un interruttore touch che può essere occultato dietro un tappo della cassetta interruttori consentendo, quindi, modifiche minime al preesistente impianto.

## Hardware usato

Wemos D1 mini
TP223 Touch
Relé HF41F
1N4001
resistenza 520 ohm

## Schema elettrico

lo schema elettrico è presentato qui di seguito. Allo scopo di ridurre le dimensioni del circuito si è utilizzato un relé HF41F di ridotte dimensioni ma con capace di soli 6 ampere, per correnti maggiori occorre utilizzare altri tipi di relè nella vasta gamma di relé disponibili sul mercato.

La resistenza R1 è indicativa poiché la corrente che il transistor Q1 deve erogare dipende dal tipo di relè scelto. In alternativa è possibile utilizzare uno shield relè che integra il gruppo R1,D1,Q1,Relay.
![image](https://user-images.githubusercontent.com/52027701/114725410-c1cd7c00-9d3c-11eb-96b9-7f2e85764558.png)


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
