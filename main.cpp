#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>

class AnimCircle
{
	sf::CircleShape m_circle;
	std::string m_name;
	sf::Vector2f m_speed;

public:
	AnimCircle(std::string name, sf::Vector2f pos, sf::Vector2f speed, sf::Color color, int radius)
		: m_name(name)
		, m_speed(speed)
	{
		m_circle.setPosition(pos);
		m_circle.setFillColor(color);
		m_circle.setRadius(radius);
	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(m_circle);

		// todo: draw shape name
	}
};

// vars
sf::Font font;
int fontSize;
sf::Color fontColor;

std::vector<AnimCircle> circles;
std::vector<sf::RectangleShape> rects;

void loadShapes(std::ifstream& fin)
{
	const std::string FONT = "Font";
	const std::string CIRCLE = "Circle";
	const std::string RECT = "Rectangle";

	std::string line;

	// as long as there's still stuff to read in
	while (fin >> line)
	{
		// this line is a font
		if (line.compare(FONT) == 0)
		{
			std::string fileName;
			int r, g, b;

			fin >> fileName;
			fin >> fontSize;
			fin >> r;
			fin >> g;
			fin >> b;

			fontColor = sf::Color(r, g, b);

			// attempt to create the font
			if (!font.loadFromFile(fileName))
			{
				// if we can't load the font, print an error to the error console and exit
				std::cerr << "Could not load font!\n";
				exit(-1);
			}
		}
		else if (line.compare(CIRCLE) == 0)
		{
			std::string name;
			sf::Vector2f pos, speed;
			int r, g, b;
			int radius;

			fin >> name;
			fin >> pos.x;
			fin >> pos.y;
			fin >> speed.x;
			fin >> speed.y;
			fin >> r;
			fin >> g;
			fin >> b;
			fin >> radius;

			std::cout << "name: " << name << ", pos.x: " << pos.x << ", pos.y: " << pos.y << "\n";

			AnimCircle c(name, pos, speed, sf::Color(r, g, b), radius);
			circles.push_back(c);
		}
		else if (line.compare(RECT) == 0)
		{

		}

	}

}
int main(int argc, char* argv[])
{
	std::ifstream fin("config.txt");
	std::string line;

	// 1) INIT WINDOW
	fin >> line;	// first line will just be "Window"

	int wWidth = 0;
	int wHeight = 0;

	fin >> wWidth;
	fin >> wHeight;

	sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "Assignment 1: Bouncing Shapes");
	window.setFramerateLimit(60);

	// 2) SHAPES
	loadShapes(fin);


	// let's make a shape that we will draw to the screen
	//sf::CircleShape circle(50);				// create a circle shape with radius 50
	//circle.setFillColor(sf::Color::Green);	// set the circle color to green
	//// how to specify rgb color: circle.setFillColor(sf::Color(r, g, b));

	//circle.setPosition(300.0f, 300.0f);		// set the top-left position of the circle
	//float circleMoveSpeed = 0.1f;			// we will use this to move the circle later

	// set up the text object that will be drawn to the screen
	sf::Text text("Sample Text", font, fontSize);

	// position the top-left corner of the text so that the text aligns on the bottom
	// text character size is in pixels, so move the text up from the bottom by its height
	text.setPosition(0, wHeight - (float)text.getCharacterSize());
	text.setFillColor(fontColor);

	// main loop - continues for each frame while window is open
	while (window.isOpen())
	{
		// event handling
		sf::Event event;
		while (window.pollEvent(event))
		{
			// this event triggers when the window is closed
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			// this event is triggered when a key is pressed
			if (event.type == sf::Event::KeyPressed)
			{
				// print the key that was pressed to the console
				std::cout << "Key pressed with code = " << event.key.code << "\n";

				// example, what happenes when x is pressed
				if (event.key.code == sf::Keyboard::X)
				{
					// reverse the direction of the circle on the screen
					//circleMoveSpeed *= -1.0f;
				}
			}
		}

		// basic animation - move each frame if it's still in frame
		//circle.setPosition(circle.getPosition().x + circleMoveSpeed, circle.getPosition().y + circleMoveSpeed);

		// basic rendering function calls
		window.clear();			// clear the window of anything previously drawn
		window.draw(text);		// draw the text

		for (auto c : circles)
		{
			c.draw(window);
		}

		window.display();		// call the window display function
	}

	return 0;
}