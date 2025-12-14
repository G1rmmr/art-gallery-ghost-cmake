#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <sstream>

#include <SFML/Network.hpp>

#include "../util/Types.hpp"
#include "../util/Debugger.hpp"

namespace mir{
    namespace http{
        enum class Type{
            Get,
            Post
        };

        struct Set{
            String Host;
            String EndPoint;
            Uint Port;
        };

        namespace{
            static inline String BuildQuery(Dictionary<String, String> params){
                if(params.empty()) {
                    debug::Log("HTTP Parameters are empty!");
                    return "";
                }

                std::stringstream stream;
                bool isFirst = true;

                for(const auto& [key, val] : params){
                    if(!isFirst) stream << "&";
                    stream << key << "=" << val;
                    isFirst = false;
                }

                params.clear();
                return stream.str();
            }
        }

        static inline void Request(Type type, const Set& set, Dictionary<String, String> params){
            FireAndForget([type, params, set](){
                HTTP http(set.Host, set.Port);
                HTTP::Request request;

                const String query = BuildQuery(params);

                switch(type){
                case Type::Get:
                    request = {
                        set.EndPoint + (query.empty() ? "" : "?" + query),
                        HTTP::Request::Method::Get
                    };
                    break;
                case Type::Post:
                    request = {
                        set.EndPoint,
                        HTTP::Request::Method::Post,
                        query
                    };
                    request.setField("Content-Type", "application/x-www-form-urlencoded");
                    break;

                default: break;
                }

                HTTP::Response response = http.sendRequest(request);

                if(response.getStatus() == HTTP::Response::Status::Ok ||
                    response.getStatus() == HTTP::Response::Status::Created){
                    debug::Log("[Network] Success: %s (Status: %d)",
                        set.EndPoint.c_str(), TypeCast<Int>(response.getStatus()));
                }
                else{
                    debug::Log("[Network] Failed: %s (Status: %d)",
                        set.EndPoint.c_str(), TypeCast<Int>(response.getStatus()));
                }
            });
        }
    }
}
