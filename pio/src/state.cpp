#include "schema.hpp"

void railschema::TrainState::to_json(json &j)
{
  j = json::object();
  j["direction"] = direction;
  j["speed"] = speed;
  j["id"] = id;
}

void railschema::State::to_json(json &j)
{
  j = json::object();
  j["id"] = id;
}
