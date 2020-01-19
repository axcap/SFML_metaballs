// Anything after // is a comment not actual C++ code
// Comments are important and I use them to explain things
// Why not read the comments in this code

// These "include" code from the C++ library and SFML too
#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <iostream>
#include <cmath>
#include <stdint.h>

#define N 5
#define WIDTH 1200
#define HEIGHT 1000

#define center_x WIDTH / 2
#define center_y HEIGHT / 2

sf::Uint8        *pixels  = new sf::Uint8[WIDTH * HEIGHT * 4];
sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Variables Demo");

class Circle : sf::CircleShape {
public:
  Circle(int xx, int yy, double radius, sf::Color c){
    x = xx;
    y = yy;
    r = radius;
    circ = sf::CircleShape(r);
    circ.setFillColor(c);

    // Give circle speed propotional to its radius
    vel[0] = 100 / r;
    vel[1] = 100 / r;
  }

  void set_color(int col){
    circ.setFillColor(sf::Color(col, col, col));
  }

  void update(){
    if (x-r+vel[0] < 0 || x+r+vel[0] > WIDTH)
      vel[0] = -vel[0];

    if (y-r+vel[1] < 0 || y+r+vel[1] > HEIGHT)
      vel[1] = -vel[1];

    x += vel[0];
    y += vel[1];
  }

  void draw(){
    circ.setPosition(x-r-vel[0], y-r-vel[1]);
    circ.setPosition(x-r, y-r);
    circ.setFillColor(sf::Color(0,0,0,0));
    circ.setOutlineThickness(3);
    window.draw(circ);
  }

public:
  sf::CircleShape circ;
  int x;
  int y;
  float r;
  float vel[2];
};


void draw_fps(sf::RenderWindow* window, int fps, sf::Text* fps_str){
  sf::String s = std::to_string(fps) + " fps";
  fps_str->setString(s);
  fps_str->setCharacterSize(24);
  fps_str->setFillColor(sf::Color::Green);
  window->draw(*fps_str);

}

float dist(int ax, int ay, int bx, int by){
  // Calculate L2 distance (hypotenuse)
  //return hypot((ax - bx), (ay - by)); // SLower approach due to sqrt()
  return ((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
}

int distance_to_color(int x, int y, Circle* circ){
  static float d;
  d = dist(x, y, circ->x, circ->y);
  float col = 300 * circ->r * circ->r / d;
  //float col = 10 * circ->r * circ->r / d;
  return col;
}

void set_pixel_color(int x, int y, sf::Color c){
  //int gray = 0.2989 * c.r + 0.5870 * c.g + 0.1140 * c.b;
  pixels[4*(y*WIDTH + x)]   = c.r; // R?
  pixels[4*(y*WIDTH + x)+1] = c.g; // G?
  pixels[4*(y*WIDTH + x)+2] = c.b; // B?
  pixels[4*(y*WIDTH + x)+3] = 255; // A?
}
// This is the main C++ program- Duh!
// It is where our game starts from
int main()
{
  // Make a window that is 800 by 600 pixels
  // And has the title "Variables Demo"
  sf::Clock        Clock;
  sf::Text         fps_str;
  sf::Sprite       sprite;
  sf::Texture      texture;
  sf::Image        image;

  // Font for FPS output
  sf::Font font;
  font.loadFromFile("arial.ttf");
  fps_str.setFont(font);

  // Generate N circles
  Circle* circles[N];
  for (int i = 0; i<N; i++){
    // Create new circle with random radius in random position
    int r = 10 + (rand() % static_cast<int>(50 - 10 + 1));
    int x = r + (rand() % static_cast<int>(WIDTH - r + 1));
    int y = r + (rand() % static_cast<int>(HEIGHT - r + 1));
    circles[i] = new Circle(x, y, (double)r, sf::Color::Cyan);

    // Give it random color, currently color is not used
    int red = (rand() % static_cast<int>(255 + 1));
    int green = (rand() % static_cast<int>(255 + 1));
    int blue = (rand() % static_cast<int>(255 + 1));
    circles[i]->circ.setFillColor(sf::Color(red, green, blue));
  }

  while (window.isOpen()){
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        // Someone closed the window- bye
        window.close();
    }

    // Clear everything from the last run of the while loop
    window.clear();

    for(int x = 0; x < WIDTH; x++) {
        for(int y = 0; y < HEIGHT; y++) {
            // Calculate color for each pixel on screen
            int colors_sum = 0;
            for(int i=0; i<N; i++){
              colors_sum += distance_to_color(x, y, circles[i]);
            }
            // Take care of uint8 overflow in sf::Color
            colors_sum = colors_sum > 255 ? 255 : colors_sum;
            sf::Color color(colors_sum, colors_sum, colors_sum);

            //if (c.r < 10 && c.g < 10 && c.b < 10) break;
            set_pixel_color(x, y, color);
          }
      }

    // Draw pixel array to screen
    image.create(WIDTH, HEIGHT, pixels);
    texture.loadFromImage(image);
    sprite.setTexture(texture, true);
    window.draw(sprite);

    // Calculate FPS
    int fps = 1.f / Clock.getElapsedTime().asSeconds();
    Clock.restart();
    draw_fps(&window, fps, &fps_str);

    // Move each blob
    for(int i=0; i<N; i++){
      circles[i]->update();
      //circles[i]->draw();
    }

    // Show everything on the screen
    window.display();
  }

  return 0;
}
