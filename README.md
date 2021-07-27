# maniekTemperature
<a href="http://munin-monitoring.org/">munin</a> plugin to monitor temperatures from Maniek custom temperature device

<img src="screenshot.png">

## INSTALL
* Set the tty device (`$tty=` line).
* Copy the file to `/etc/munin/plugins` (or to `/usr/share/munin/plugins/` and then `ln -s /usr/share/munin/plugins/maniekTemperature /etc/munin/plugins/`).
* Check if $tty is accessible by munin (you can do it by running the plugin from the munin user: `sudo -u munin /etc/munin/plugins/maniekTemperature`), if not allow for it (for example with `chmod 666 /dev/ttyUSB0`).

## ABOUT Maniek custom temperature device
```
==========================================================
Pomoc dla czujnika temperatury
showtemp - pokazuje temperature
showaddr - pokazuje adresy czujników temp. w kolejności
start - automatyczne włączenie wypisywania temperatur
stop - zatrzymanie automatycznego wypisywania temperatur
delay=<s> - czas odczytów w tyrbie auto od 1 do 255 (sekund)
save - zapisuje aktualne ustawienia
==========================================================
```

## REQUIREMENTS
PHP
