#include "packet-parser.h"

#include <algorithm>

namespace Packets {

	namespace decode {

	} // namespace decode

	namespace encode {

		std::string Login::encodeJSON(const Login& val)
		{
			json j{
				{"name", val.name}
			};
			if (val.password.has_value()) j["password"] = val.password.value();
			if (val.game.has_value()) j["game"] = val.game.value();
			if (val.num_turns.has_value()) j["num_turns"] = val.num_turns.value();
			if (val.num_players.has_value()) j["num_players"] = val.num_players.value();

			return Packets::encodeString(Action::LOGIN, j.dump());
		}

		std::string Player::encodeJSON()
		{
			return Packets::encodeString(Action::PLAYER);
		}

		std::string Logout::encodeJSON()
		{
			return Packets::encodeString(Action::LOGOUT);
		}

		std::string Map::encodeJSON(const Map& val)
		{
			json j{ 
				{"layer", val.layer} 
			};

			return Packets::encodeString(Action::MAP, j.dump());
		}

		std::string Move::encodeJSON(const Move& val)
		{
			json j{
				{"line_idx", val.line_idx},
				{"speed", val.speed},
				{"train_idx", val.train_idx}
			};

			return Packets::encodeString(Action::MOVE, j.dump());
		}

		std::string Upgrade::encodeJSON(const Upgrade& val)
		{
			json j{
				{"posts", val.posts},
				{"trains", val.trains}
			};

			return Packets::encodeString(Action::UPGRADE, j.dump());
		}

		std::string Turn::encodeJSON()
		{
			return Packets::encodeString(Action::TURN);
		}

		std::string Games::encodeJSON()
		{
			return Packets::encodeString(Action::GAMES);
		}

	} // namespace encode

} // namespace Packets