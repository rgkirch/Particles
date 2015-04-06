#include <iostream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
using namespace std;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main( int argc, char *argv[] )
{
	const char *filename = argc > 1 ? argv[1] : "data.txt";
	FILE *f = fopen( filename, "r" );
	if( f == NULL )
	{
		printf( "failed to find %s \n", filename );
		return 1;
	}
	int n;
	float size;
	// read how many points, and size (scales the points)
	fscanf( f, "%d%g", &n, &size );
	// set circle radius, set how many sides
	sf::CircleShape circle(4, 8);
	// circle.setPointCount(5);
	circle.setFillColor( sf::Color::Black );
	// create window
	sf::RenderWindow window( sf::VideoMode( WINDOW_WIDTH, WINDOW_HEIGHT ), "SFML Visualizer!" );
	window.setVerticalSyncEnabled( true );
	window.setFramerateLimit( 15 );
	while( window.isOpen() )
	{
		sf::Event event;
		while( window.pollEvent( event ) )
		{
			if( event.type == sf::Event::Closed )
			{
				window.close();
			} 
		}
		// clear the previous frame
		window.clear( sf::Color::White );
		// make a temp copy of circle
		sf::CircleShape temp = circle;
		float x = 0.0;
		float y = 0.0;
		int particle_count = 0;
		for( int i = 0; i < n; ++i )
		{
			// if we didn't read two things, exit
			if( fscanf( f, "%g%g", &x, &y ) != 2 )
			{
				window.close();
			}
			temp.setPosition( x / size * WINDOW_WIDTH, y / size * WINDOW_HEIGHT );
			window.draw( temp );
			particle_count++;
		}
		cout << particle_count << endl;
		window.display();
	}
	fclose( f );
	return 0;
}
