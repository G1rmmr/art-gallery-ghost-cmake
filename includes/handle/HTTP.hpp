#pragma once

#include <iostream>
#include <string>
#include <sys/types.h>
#include <thread>
#include <sstream>

#include <SFML/Network.hpp>

#include "../util/Types.hpp"
#include "../util/Debugger.hpp"

namespace mir{
    namespace http{
        static inline const String HOST = "localhost";
        static inline const Uint PORT = 8080;

        static inline Dictionary<String, String> Params = {};

        namespace{
            enum class Type{
                Get,
                Post
            };

            static inline String BuildQuery(){
                if(Params.empty()) {
                    debug::Log("HTTP Parameters are empty!");
                    return "";
                }

                std::stringstream stream;
                bool isFirst = true;

                for(const auto& [key, val] : Params){
                    if(!isFirst) stream << "&";
                    stream << key << "=" << val;
                    isFirst = false;
                }
                return stream.str();
            }

            static inline void Request(Type type, HTTP http, const String &endPoint){
                HTTP::Request request;

                String query = BuildQuery();

                switch(type){
                case Type::Get:
                    request.setUri(endPoint + (query == "" ? "" : "?" + query));
                    break;
                case Type::Post:
                    request.setUri(endPoint);
                    request.setBody(query);
                    request.setField("Content-Type", "application/x-www-form-urlencoded");
                    break;

                default: break;
                }

                HTTP::Response response = http.sendRequest(request);

                if(response.getStatus() == HTTP::Response::Status::Ok ||
                    response.getStatus() == HTTP::Response::Status::Created){
                    debug::Log("[Network] Success: %s (Status: %d)",
                        endPoint.c_str(), TypeCast<Int>(response.getStatus()));
                }
                else{
                    debug::Log("[Network] Failed: %s (Status: %d)",
                        endPoint.c_str(), TypeCast<Int>(response.getStatus()));
                }
            }
        }

        static inline void Get(const String& host, const Uint port, const String& endPoint){
            HTTP http(host, port);
            FireAndForget(Request, Type::Get, http, endPoint);
        }

        static inline void Post(const String& host, const Uint port, const String& endPoint){
            HTTP http(host, port);
            FireAndForget(Request, Type::Post, http, endPoint);
        }
    }
}
