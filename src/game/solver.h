#pragma once

#include "game/data/post.h"
#include "game/solver/task/TakeProduct.h"
#include "game/solver/task/TakeArmor.h"
#include "game/solver/task/BringToTown.h"
#include "game/solver/task/UpgradeTown.h"
#include "game/solver/task/UpgradeTrain.h"
#include "game/data.h"
#include "game/data/train.h"
#include "game/controller.h"
#include "game/solver/dispatcher.h"

class GameSolver
{
private:
    GameData& gamedata;
    TaskDispatcher dispatcher;
    GameController& controller;

public:
    GameSolver(GameData& gamedata, GameController& controller)
        : gamedata(gamedata), controller(controller)
    {
        SPDLOG_DEBUG("constructing Game solver ");
    }

    void init()
    {
        for(const auto& [idx, post] : gamedata.posts )
        {
            switch (post->type())
            {
                case Posts::PostType::MARKET:
                    dispatcher.add(std::shared_ptr<Task>(new TakeProductTask(controller,
                                                                             std::dynamic_pointer_cast<Posts::Market>(post))));
                    break;
                case Posts::PostType::STORAGE:
                    dispatcher.add(std::shared_ptr<Task>(new TakeArmorTask(controller,
                                                                           std::dynamic_pointer_cast<Posts::Storage>(post))));
                    break;
                case Posts::PostType::TOWN:
                    dispatcher.add(std::shared_ptr<Task>(new BringToTownTask(controller,
                                                                             std::dynamic_pointer_cast<Posts::Town>(post))));
                    break;
            }
        }

        dispatcher.add(std::shared_ptr<Task>(new UpgradeTownTask(controller,
                                                                std::dynamic_pointer_cast<Posts::Town>(gamedata.posts[gamedata.post_idx]))));

        //const auto&  player =  gamedata.players[gamedata.player_idx];
        /*for(auto& [idx, train] : player.trains)
        {
            dispatcher.add(std::shared_ptr<Task>(new UpgradeTrainTask(controller,
                                                                      std::make_shared<Trains::Train>(train))));
        }*/

        for(auto const& [idx, post] : gamedata.posts)
        {
            SPDLOG_DEBUG("Some action with post: {}", *post);
        }
//exit(1);

    }

    void doTurn()
    {
        dispatcher.dispatchAll();

    }

};

