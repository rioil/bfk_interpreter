all:	main.cpp
	g++ -o bfk main.cpp
.PHONY: clean
clean:
	-del /Q /S *.exe