rem zcc +clock80 main.c lcd.c gui.c test.c -o clock80.bin --list -m -create-app -a --c-code-in-asm --math-dai32
zcc +clock80 test_gr.c lcd.c gui.c cust_fnt.c -o test_gr.bin --list -m -create-app --math-dai32 -pragma-define:CRT_MODEL=1
del full.bin
ren test_gr.rom full.bin