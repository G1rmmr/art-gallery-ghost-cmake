#pragma once

#include <Mir>

namespace network{
    const mir::String HOST = "localhost";
    const mir::Uint PORT = 8080;

    static inline void PostScore(const mir::String& name, const mir::Uint score){
        mir::http::Params = {
            {"Name", name},
            {"Score", mir::ToString(score)}
        };

        mir::http::Post(HOST, PORT, "/submit");
    }

    static inline void GetTopPlayers(){
        mir::http::Get(HOST, PORT, "/top5");
    }
}
