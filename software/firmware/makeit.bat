rem zcc +clock80 main.c lcd.c gui.c test.c -o clock80.bin --list -m -create-app -a --c-code-in-asm --math-dai32
zcc +clock80 main.c lcd.c gui.c cust_fnt.c i2c.c onewire.c bme280.c font.c forecast.c -o clock80.bin --list -m -create-app --math-dai32 -pragma-define:CRT_MODEL=2
del full.bin
ren clock80.rom full.bin