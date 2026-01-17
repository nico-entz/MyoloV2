# Myonendetektor - Fehlerbehebung

In diesem Dokumente finden Sie Hinweise zur Fehlersuche, sollte Ihr Detektor nicht wie gewünscht funktionieren.

  

Auf der Hauptplatine Ihres Myonendetektors befinden sich als Unterstützung bei der Fehlersuche neun Testpunkte. Auf der Platine sind diese mit TP0 bis TP9 beschriftet.

  
  

An den Testpunkten sollte - je nach Testpunkt - entweder eine Spannung oder eine bestimmte Pulsform messbar sein.

  
  

Die Spannungen können Sie mit jedem Multimeter für ein paar Euro einfach nachmessen.

Für die Messung der Pulsformen benötigen Sie ein Oszilloskop.

  
  

Um die Messungen durchzuführen, verbinden Sie die Masse des Multimeters oder des Oszilloskopes mit TP0 (GND / Masse) und die Messspitze mit dem jeweiligen Testpunkt, den Sie messen möchten.

  

Dann verbinden Sie die andere Messspitze mit den jeweiligen Testpunkten. Im Folgenden finden Sie Angaben der Spannungen bzw. der Pulsformen, die an den jeweiligen Testpunkten anliegen sollten.

  

Idealerweise arbeiten Sie sich von vorne bis hinten durch diese Liste - so finden sich die wichtigsten Fehler schneller.

  

## TP5

Typ der Messung: Spannungsmessung mit Multimeter

TP5 ist mit dem Ausgang des Spannungswandlers verbunden, der die Spannungsversorgung der Lichtdetektoren / SiPM bereitstellt. Hier sollte eine Spannung von etwa **30V** messbar sein.

  

Messen Sie eine andere (oder keine) Spannung, prüfen Sie die korrekte Verbindung aller Beinchen des Spannungswandlers MAX5026 mit der Hauptplatine - bessern Sie im Zweifel mit etwas zusätzlichem Lötzinn aus. Prüfen Sie außerdem die korrekte Bestückung aller Bauteile, die sich in diesem Bereich der Schaltung befinden (R11 bis R13, C5 bis C7, D7, L1). Haben Sie die korrekten Widerstände und Kondensatoren verbaut? Ist die Diode richtig herum eingebaut? Sind alle Komponenten sauber verlötet?

  

## TP1

Typ der Messung: Spannungsmessung mit Multimeter

  

TP1 ist mit dem **positiven** Ausgang des Spannungswandlers der symmetrischen Spannungsversorgung der Operationsverstärker verbunden. Sie sollten hier eine Spannung von etwa **+15V** messen.

  

Messen Sie eine andere (oder keine) Spannung, prüfen Sie die Einbaurichtung des Spannungswandlers SDT01F05D15. Die Beschriftung (die auch die Modellbezeichnung SDT01F05D15 enthält) muss von der Platine weg zeigen.

  

## TP2

Typ der Messung: Spannungsmessung mit Multimeter

  
  

TP1 ist mit dem **negativen** Ausgang des Spannungswandlers der symmetrischen Spannungsversorgung der Operationsverstärker verbunden. Sie sollten hier eine Spannung von etwa **-15V** messen.

  

Messen Sie eine andere (oder keine) Spannung, prüfen Sie die Einbaurichtung des Spannungswandlers SDT01F05D15. Die Beschriftung (die auch die Modellbezeichnung SDT01F05D15 enthält) muss von der Platine weg zeigen.

## TP3 & TP4

Typ der Messung: Überprüfung der Pulsform mit Oszilloskop

TP3 und TP4 sind direkt mit den Ausgängen der jeweiligen Lichtdetektoren / SiPM der Szintillatormodule verbunden. Hier sollte dementsprechend das rohe Ausgangssignal der Lichtdetektoren anliegen. Hier sollten **sehr kurze Pulse (Nanosekundenbereich)** mit einer Peak-Spannung im **zwei bis dreistelligen Millivoltbereich** messbar sein, wenn ein Teilchen das jeweilige Szintillatormodul trifft.


![TP3 und TP4 jeweils an einem Kanal des Oszilloskops. Zu sehen ist dass beide Szintillatormodule gleichzeitig ein Signal erzeugt haben (Koinzidenz) - höchstwahrscheinlich durch ein Myon erzeugt.](https://github.com/nico-entz/MyoloV2/blob/main/Bilder/TP3TP4.jpg?raw=true)

## TP6 & TP7
Typ der Messung: Überprüfung der Pulsform mit Oszilloskop

TP6 und TP7 ist mit den Ausgängen der Verstärkerschaltung verbunden. Hier sollte das verstärkte Detektorsignal anliegen. Hier sollten **sehr kurze Pulse (Nanosekundenbereich)** mit einer **Peak-Spannung im dreistelligen Millivoltbereich bis einstelligen Voltbereich** messbar sein, wenn ein Teilchen das jeweilige Szintillatormodul trifft.

![TP6 und TP7 jeweils an einem Kanal des Oszilloskops. Zu sehen ist dass beide Szintillatormodule gleichzeitig ein Signal erzeugt haben (Koinzidenz) - höchstwahrscheinlich durch ein Myon erzeugt.](https://github.com/nico-entz/MyoloV2/blob/main/Bilder/TP6TP7.jpg?raw=true)
 
## TP8 & TP9
Typ der Messung: Überprüfung der Pulsform mit Oszilloskop

An TP8 und TP9 können die Signale abgegriffen werden, die zum Analog-Digital-Wandler des Mikrocontrollers geleitet werden. Diese sollten vollständig verstärkt und verlängert sein. Hier sollten Pulse im **Mikrosekundenbereich** mit einer **Peak-Spannung im dreistelligen Millivoltbereich bis einstelligen Voltbereich** messbar sein, wenn ein Teilchen das jeweilige Szintillatormodul trifft.
![TP8 und TP9 jeweils an einem Kanal des Oszilloskops. Zu sehen ist dass beide Szintillatormodule gleichzeitig ein Signal erzeugt haben (Koinzidenz) - höchstwahrscheinlich durch ein Myon erzeugt.](https://github.com/nico-entz/MyoloV2/blob/main/Bilder/TP8TP9.jpg?raw=true)