# Myonendetektor - Fehlerbehebung
In diesem Dokumente finden Sie Hinweise zur Fehlersuche, sollte Ihr Detektor nicht wie gewünscht funktionieren.

Auf der Hauptplatine Ihres Myonendetektors befinden sich als Unterstützung bei der Fehlersuche neun Testpunkte. Auf der Platine sind diese mit TP1 bis TP9 beschriftet.


An den Testpunkten sollte - je nach Testpunkt - entweder eine Spannung oder eine bestimmte Pulsform messbar sein.


Die Spannungen können Sie mit jedem Multimeter für ein paar Euro einfach nachmessen.
Für die Messung der Pulsformen benötigen Sie ein Oszilloskop.


Um die Messungen durchzuführen, verbinden Sie die Massen-Messspitze des Multimeters oder des Oszilloskopes mit den Pins am STM32, die mit GND beschriftet sind. Für die Verbindung eignen sich bspw. Krokodilklemmen. Oder Sie löten zusätzlich ein kurzes Kabel an die Kontakte. 

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

TP3 und TP4 sind direkt mit den Ausgängen der jeweiligen Lichtdetektoren / SiPM der Szintillatormodule verbunden. Hier sollte dementsprechend das rohe Ausgangssignal der Lichtdetektoren anliegen. Hier sollte ein sehr kurzer Puls (Nanosekundenbereich) mit steiler Flanke auf dem Bildschirm des Oszilloskopes zu sehen sein.

## TP6 & TP7
Typ der Messung: Überprüfung der Pulsform mit Oszilloskop

TP3 und TP4 sind direkt mit den Ausgängen der jeweiligen Lichtdetektoren / SiPM der Szintillatormodule verbunden. Hier sollte dementsprechend das rohe Ausgangssignal der Lichtdetektoren anliegen. Hier sollte ein sehr kurzer Puls (Nanosekundenbereich) mit steiler Flanke auf dem Bildschirm des Oszilloskopes zu sehen sein.

## TP8 & TP9

