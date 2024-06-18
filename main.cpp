#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>

// global text vars
sf::Font font;
int fontSize;
sf::Color fontColor;

// CLASSES ----------------------------------------------------------------------
class AnimCircle
{
	sf::CircleShape m_circle;
	sf::Vector2f m_speed;
	sf::Text m_text;

public:
	AnimCircle(std::string name, sf::Vector2f pos, sf::Vector2f speed, sf::Color color, int radius)
		: m_speed(speed)
	{
		m_circle.setPosition(pos);
		m_circle.setFillColor(color);
		m_circle.setRadius(radius);

		m_text.setString(name);
		m_text.setFont(font);
		m_text.setFillColor(fontColor);
		m_text.setCharacterSize(fontSize);
		m_text.setPosition(pos.x + radius - m_text.getLocalBounds().width / 2,
			pos.y + radius - fontSize / 2);
	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(m_circle);
		window.draw(m_text);
	}
};

class AnimRect
{
	sf::RectangleShape m_rect;
	sf::Vector2f m_speed;
	sf::Text m_text;

public:
	AnimRect(std::string name, sf::Vector2f pos, sf::Vector2f speed, sf::Color color, sf::Vector2f size)
		: m_speed(speed)
	{
		m_rect.setPosition(pos);
		m_rect.setFillColor(color);
		m_rect.setSize(size);

		m_text.setString(name);
		m_text.setFont(font);
		m_text.setFillColor(fontColor);
		m_text.setCharacterSize(fontSize);
		m_text.setPosition(pos.x + size.x / 2 - m_text.getLocalBounds().width / 2,
			pos.y + size.y / 2 - fontSize / 2);

	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(m_rect);
		window.draw(m_text);
	}
};

// MAIN PROGRAM ------------------------------------------------------------------



std::vector<AnimCircle> circles;
std::vector<AnimRect> rects;

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

			AnimCircle cir(name, pos, speed, sf::Color(r, g, b), radius);
			circles.push_back(cir);
		}
		else if (line.compare(RECT) == 0)
		{
			std::string name;
			sf::Vector2f pos, speed, size;
			int r, g, b;

			fin >> name;
			fin >> pos.x;
			fin >> pos.y;
			fin >> speed.x;
			fin >> speed.y;
			fin >> r;
			fin >> g;
			fin >> b;
			fin >> size.x;
			fin >> size.y;

			AnimRect rec(name, pos, speed, sf::Color(r, g, b), size);
			rects.push_back(rec);
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

		// draw shapes
		for (auto& c : circles)
		{
			c.draw(window);
		}

		for (auto& r : rects)
		{
			r.draw(window);
		}

		window.display();		// call the window display function
	}

	return 0;
}