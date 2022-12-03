all: VM GUI c

VM: VM.c
	gcc `pkg-config --cflags gtk4` VM.c -o VM `pkg-config --libs gtk4`

GUI: CompGUI.c
	gcc `pkg-config --cflags gtk4` CompGUI.c -o GUI `pkg-config --libs gtk4`

c: Compilador.c
	gcc Compilador.c -o c
