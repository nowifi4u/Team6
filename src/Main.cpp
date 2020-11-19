#include "json-parser.h"
#include "KKSCoordsCalculator.h"
#include <iomanip>
#include <memory>
#include <SFML/Graphics.hpp>


int main()
{
	Graph g;

	vertexMap vMap;
	edgeMap eMap;

	//catch json_parser_error!!!
	importGraph("small_graph.json", g, vMap, eMap);

	KKSCoordsCalculator b;
	
	PositionMap pos = b.get_coords(g, 200, 200, 3);


	//// Объект, который, собственно, является главным окном приложения
	//sf::RenderWindow window(sf::VideoMode(200, 200), "SFML Works!");

	//// Главный цикл приложения. Выполняется, пока открыто окно
	//while (window.isOpen())
	//{
	//	// Обрабатываем очередь событий в цикле
	//	sf::Event event;
	//	while (window.pollEvent(event))
	//	{
	//		// Пользователь нажал на «крестик» и хочет закрыть окно?
	//		if (event.type == sf::Event::Closed)
	//			// Тогда закрываем его
	//			window.close();
	//	}
	//	// Отрисовка окна	
	//	window.display();
	//}


	return 0;
}

