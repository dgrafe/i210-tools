# i210-flash - Flashing/Dumping/Modifications to the i210's config EEPROM

## Dumping the entire EEPROM

The first 1MiB of the config EEPROM will be dumped and printed to stdout. This seems to be the format of the official intel supplied firmware images.

```
i210-flash --dump --bdf 1:0.0 > /tmp/dump.bin
```

## Showing information on the EEPROM

The --info switch can be used to display some information on the EEPROM.

```
i210-flash --info --bdf 1:0.0
...
Flash:
 Size(KiB): 2048
 Secure Mode: true
 Flash detected: true
 Flash present: true
 Flash in use: true
 Flash speed(KHz): 62500
```
