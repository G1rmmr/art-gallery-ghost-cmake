#pragma once

#include <Mir>

namespace network{
    const mir::String HOST = "127.0.0.1";
    const mir::Uint PORT = 8080;

    static inline void PostScore(const mir::String& name, const mir::Uint score){
        mir::Dictionary<mir::String, mir::String> params = {
            {"name", name},
            {"score", mir::ToString(score)}
        };

        mir::http::Request(mir::http::Type::Post,
            {HOST, "/submit", PORT}, params
        );
    }

    static inline void GetTopPlayers(){
        mir::Dictionary<mir::String, mir::String> params = {};
        mir::http::Request(mir::http::Type::Get, {HOST, "/top5", PORT}, params);
    }
}
