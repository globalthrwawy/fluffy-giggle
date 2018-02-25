#include <iostream>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

//State :: Game
enum class dir {STOP, LEFT, RIGHT, UP, DOWN};

auto ply_x(0), ply_y(0), fruit_x(0), fruit_y(0), score (0);
auto ply_dir = dir::STOP;
auto num_col (20), num_rows (20);
auto gameOver(false);

//State :: Engine
auto window = sf::RenderWindow();
auto heading = sf::Text();
auto framerate_disp = sf::Text();
auto font = sf::Font();
unsigned int stage_width(600), stage_height(600);
double frameRate (0);
//init sprites
sf::RectangleShape sprite_snake = sf::RectangleShape();
sf::FloatRect snake_dimensions = sf::FloatRect();
sf::CircleShape sprite_fruit = sf::CircleShape();
sf::FloatRect fruit_deminsions = sf::FloatRect();

sf::RenderTexture stageTexture;
sf::RenderTexture actorTexture;
sf::Clock deltaClock;
sf::Time eT;

//State :: Combined
auto grid_width (stage_width / num_col);
auto grid_height (stage_height / num_rows);

bool Setup() {

    //Engine Setup
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    // Create the main window
    window.setFramerateLimit(60);
    window.create(sf::VideoMode(800, 800), "Snake Game", sf::Style::Default, settings);

    sprite_snake.setSize(sf::Vector2f(grid_width, grid_height));
    snake_dimensions = sprite_snake.getGlobalBounds();
    sprite_fruit.setRadius(grid_width / 4);
    fruit_deminsions = sprite_fruit.getGlobalBounds();

    std::srand(std::time(nullptr));

    //Platform Setup
    if (!font.loadFromFile("assets/Rubik/Rubik-Medium.ttf"))
        return false;
    if (!stageTexture.create(stage_width + 1, stage_height + 1))
        return false;
    if (!actorTexture.create(stage_width, stage_height))
        return false;
    stageTexture.setSmooth(true);
    actorTexture.setSmooth(true);

    //Game Setup
    heading.setString("SNAKE!");
    heading.setFont(font);
    heading.setCharacterSize(36);
    heading.setFillColor(sf::Color(30,30,30));
    heading.setPosition(20,10);

    framerate_disp.setString(to_string(frameRate));
    framerate_disp.setFont(font);
    framerate_disp.setCharacterSize(18);
    framerate_disp.setFillColor(sf::Color(230,230,230));
    framerate_disp.setPosition(stage_width - 100,10);

    return true;
}

const sf::Sprite DrawStage() {

    auto drawGrids = [](sf::RenderTexture* stage){
        sf::RectangleShape line_horiz(sf::Vector2f(stage_width, 0.5));
        line_horiz.setFillColor(sf::Color(30,30,30));

        sf::RectangleShape line_vert(sf::Vector2f(0.8, stage_height));
        line_vert.setFillColor(sf::Color(30,30,30));

        for( int i = 0; i <= num_col; i++) {
            line_horiz.setPosition(0, i * grid_height);
            stage->draw(line_horiz);
        }
        for(int y = 0; y <= num_rows; y++){
            line_vert.setPosition( y * grid_width, 0);
            stage->draw(line_vert);
        }
    };

    stageTexture.clear(sf::Color(20,20,20));
    drawGrids(&stageTexture);
    stageTexture.display();

    sf::Sprite stage(stageTexture.getTexture());
    stage.setPosition(20, 60);
    return stage;
}

const sf::Sprite DrawActors() {
    //Fruit
    sprite_fruit.setPosition(fruit_x * grid_width + (grid_width/2 - fruit_deminsions.width/2),
                      fruit_y * grid_height + (grid_height/2 - fruit_deminsions.height/2));
    sprite_fruit.setFillColor(sf::Color(244,172,4));

    //Snake
    sprite_snake.setPosition(ply_x * grid_width + (grid_width/2 - snake_dimensions.width/2),
                      ply_y * grid_height + (grid_height/2 - snake_dimensions.height/2));
    sprite_snake.setFillColor(sf::Color(62, 192, 158));

    actorTexture.clear(sf::Color::Transparent);
    actorTexture.draw(sprite_fruit); // or any other drawable
    actorTexture.draw(sprite_snake); // or any other drawable
    actorTexture.display();

    sf::Sprite actors(actorTexture.getTexture());
    actors.setPosition(20, 60);
    return actors;
}

void Draw() {
    // Clear screen
    window.clear();
    // Draw the string
    window.draw(heading);

    framerate_disp.setString(to_string(frameRate));
    window.draw(framerate_disp);
    window.draw(DrawStage());
    window.draw(DrawActors());
    // Update the window
    window.display();
}

void Input() {
    // Process events
    auto event = sf::Event();
    while (window.pollEvent(event))
    {
        // Close window: exit
        if (event.type == sf::Event::Closed)
            window.close();
        // catch the resize events
        if (event.type == sf::Event::Resized) { // update the view to the new size of the window
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            window.setView(sf::View(visibleArea));
        }
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                gameOver = true;
                window.close();
            }
            if (event.key.code == sf::Keyboard::Left)
            {
                std::cout << "<-" << std::endl;
                ply_dir = dir::LEFT;
            }
            if (event.key.code == sf::Keyboard::Right)
            {
                std::cout << "->" << std::endl;
                ply_dir = dir::RIGHT;

            }
            if (event.key.code == sf::Keyboard::Down)
            {
                std::cout << "v" << std::endl;
                ply_dir = dir::DOWN;

            }
            if (event.key.code == sf::Keyboard::Up)
            {
                std::cout << "^" << std::endl;
                ply_dir = dir::UP;

            }
        }
    }

}

void GameReset(bool initalSetup = false) {
    if(initalSetup) {
        gameOver = false;
        ply_dir = dir::STOP;
        ply_x = num_col / 2;
        ply_y = num_rows / 2;
    }

    fruit_x = rand() % num_col;
    fruit_y = rand() % num_rows;

    cout << fruit_x << "x" << fruit_y << '\n';
    cout << rand() << "x" << rand() % num_col << '\n';

}

void GameLoop() {
    snake_dimensions = sprite_snake.getGlobalBounds();
    fruit_deminsions = sprite_fruit.getGlobalBounds();

    if(ply_dir == dir::LEFT) {
        ply_x--;
    }
    if(ply_dir == dir::RIGHT){
        ply_x++;
    }
    if(ply_dir == dir::UP){
        ply_y--;
    }
    if(ply_dir == dir::DOWN){
        ply_y++;
    }

    []{
        if(ply_x > num_rows ){
            ply_x = 0;
        } else if ( ply_x < 0 ) {
            ply_x = num_rows;
        } else if (ply_y > num_col) {
            ply_y = 0;
        } else if (ply_y < 0) {
            ply_y = num_rows;
        }
    }();

    //collision detection
    []{
        if( fruit_deminsions.intersects(snake_dimensions)){
            score++;
            GameReset();
        }
    }();

}

int main() {

    Setup();
    GameReset(true);

    sf::Time accumulator = sf::Time::Zero;
    sf::Time tickRate = sf::seconds(1.f / 10.f);

    while (window.isOpen())
    {
        eT = deltaClock.restart();
        accumulator += eT;

        frameRate = 1 / max(eT.asMilliseconds(),1);

        Input();
        //cout<<accumulator/upperFrameLimit<<'\n';

        if(accumulator/tickRate >= 0.8) {
            accumulator = sf::Time::Zero;
            GameLoop();
            Draw();
        }


    }

    return 0;
}