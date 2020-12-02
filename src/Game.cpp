#include "Game.h"
#include <algorithm>
#include <cmath>
#include "graph.h"


const float vertex_scale_koeff = 0.1;
//to implement
GameData::PostType getPostType(GraphIdx::vertex_descriptor v) {
    return GameData::TOWN;
}

void edgeGraphics(GraphIdx& g, GraphIdx::edge_descriptor e, Window& window,
    KKSCoordsCalculator& calc, TextureManager& tm) {
    auto u = boost::source(e, g.graph);
    auto v = boost::target(e, g.graph);
    if (calc[u][0] >= calc[v][0])
        std::swap(u, v);
    sf::Texture* temp = tm.GetResource("railway");
    auto temp_x = temp->getSize().x;
    auto temp_y = temp->getSize().y;

    auto vertex_size = window.nodes_g[v].getTexture()->getSize().x * vertex_scale_koeff;
    auto edge_optimal_x_size = vertex_size / 2.0;
    auto edge_scale_koeff = edge_optimal_x_size / temp_x;

    double vertecies_distance = sqrt((calc[u][0] - calc[v][0]) * (calc[u][0] - calc[v][0]) +
        (calc[u][1] - calc[v][1]) * (calc[u][1] - calc[v][1])
    );
    sf::Texture* main_texture = tm.GetResource("railway");

    main_texture->setRepeated(true);
    window.edges_g[e] = sf::Sprite(*main_texture,
        sf::IntRect(0, 0, temp_x, round(vertecies_distance / edge_scale_koeff)));
    window.edges_g[e].setOrigin(sf::Vector2f{ (float)edge_optimal_x_size / 2, 0 });
    window.edges_g[e].setPosition(
        window.nodes_g[v].getPosition().x + vertex_size / 2,
        window.nodes_g[v].getPosition().y + vertex_size / 2
    );
    window.edges_g[e].scale(edge_scale_koeff, edge_scale_koeff);

    double tan_alpha;
    if (calc[u][1] > calc[v][1]) {
        tan_alpha = (calc[v][0] - calc[u][0]) / (calc[u][1] - calc[v][1]);
        window.edges_g[e].setRotation((float)(atan(tan_alpha) * 180.0 / 3.14159265));
    }
    else if (calc[u][1] < calc[v][1]) {
        tan_alpha = (-calc[u][1] + calc[v][1]) / (calc[v][0] - calc[u][0]);
        window.edges_g[e].setRotation((float)(90 + atan(tan_alpha) * 180.0 / 3.14159265));
    }
    else {
        window.edges_g[e].setRotation(90);
    }
}


Game::Game()
    :window_(GAME_NAME, sf::Vector2u(1000, 800), 20), clock_(), textureManager_(), g(), calc()
{
    //std::cout << "constructed\n";

    state_ = GameList::RUN;

    importGraph("GraphExamples/big_graph.json", g);

    //std::cout << window_.getWindowSize().x << " " << window_.getWindowSize().y << "\n";
    //std::cout << (size_t)window_.getDelta() << "\n";
    //std::cout << (double)window_.getWindowSize().x - 2. * ((double)window_.getDelta())<<"\n";
    //std::cout << (double)window_.getWindowSize().y - 2. * ((double)window_.getDelta()) << "\n";

    calc.calculate(
        g.graph,
        (double)window_.getWindowSize().x - 2.*((double)window_.getDelta()),
        (double)window_.getWindowSize().y - 2.*((double)window_.getDelta()),
        6
    );
   
}

Game::~Game()
{

}

void Game::renderCalculations() {

    textureManager_.RequireResource("bg");
    sf::Texture* bg_texture = textureManager_.GetResource("bg");
    window_.getBackGround().setTexture(*bg_texture);

    g.for_each_vertex([&](auto v) {
        bool b = false;
        switch (getPostType(v)) {
        case GameData::MARKET:
            b = textureManager_.RequireResource("market");
            window_.nodes_g[v] = sf::Sprite(*textureManager_.GetResource("market"));
            break;
        case GameData::TOWN:
            b = textureManager_.RequireResource("castle");
            window_.nodes_g[v] = sf::Sprite(*textureManager_.GetResource("castle"));
            break;
        case GameData::STORAGE:
            b = textureManager_.RequireResource("storage");
            window_.nodes_g[v] = sf::Sprite(*textureManager_.GetResource("storage"));
            break;
        default:
            b = textureManager_.RequireResource("circle");
            window_.nodes_g[v] = sf::Sprite(*textureManager_.GetResource("circle"));
            break;
        }
        size_t topo_size = std::min(window_.getWindowSize().x, window_.getWindowSize().y);
        window_.nodes_g[v].setPosition(sf::Vector2f{
               (float)calc[v][0] + (float)topo_size / 2.f - (*this).window_.getDelta(),
               (float)calc[v][1] + (float)topo_size / 2.f - (*this).window_.getDelta()
            });
        window_.nodes_g[v].setScale(vertex_scale_koeff, vertex_scale_koeff);
        }
    );

    g.for_each_edge([&](auto e) {
        textureManager_.RequireResource("railway");
        edgeGraphics(g, e, window_, calc, textureManager_);
        }
    );
}

void Game::update()
{
    
    if(elapsed_.asSeconds() >= frameTime)
    {
        //std::cout << "Updating ";
        switch (state_) {
        case(GameList::GameState::INIT):
            //UpdateIntro();
            break;
        case(GameList::GameState::RUN):
            //std::cout << "Running\n";
            window_.update();
            break;
        case(GameList::GameState::FINISHED):
            //UpdateFinished();
            break;
        }
        elapsed_ -= sf::seconds(frameTime);
    }
}

void Game::render()
{
        window_.beginDraw();

        switch (state_) {
        case(GameList::GameState::INIT):
            //UpdateIntro();
            break;
        case(GameList::GameState::RUN):
        {
            //Set background:
            window_.draw(window_.getBackGround());

            //edges
            g.for_each_edge([this](auto e) {
                (*this).window_.draw(this->window_.edges_g[e]);
                });
           

            //nodes
            g.for_each_vertex([this](auto v) {
                (*this).window_.draw(this->window_.nodes_g[v]);
                });

            break;
        } 
        case(GameList::GameState::FINISHED):
            //UpdateFinished();
            break;
        }

        window_.endDraw();
}

Window* Game::getWindow()
{
    return &window_;
}

void Game::handleInput()
{

}

void Game::restartClock()
{
    elapsed_ += clock_.restart();
}