all:
	clear
	g++ *.cpp -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
	./sfml-app