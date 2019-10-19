all:	main.cpp
	g++ -o bfk main.cpp -finput-charset=CP932 -fexec-charset=CP932
.PHONY: clean
clean:
	-del /Q /S *.exe