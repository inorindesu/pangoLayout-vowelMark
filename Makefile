pangoLayout: main.c out/mark
	gcc main.c -o out/pangoLayout `pkg-config --libs --cflags pango pangocairo cairo-pdf cairo` -std=c99

out/mark:
	mkdir -p out
	touch out/mark

.PHONY:clean
clean:
	rm -rf out
