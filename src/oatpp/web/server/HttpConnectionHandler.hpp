/***************************************************************************
 *
 * Project         _____    __   ____   _      _
 *                (  _  )  /__\ (_  _)_| |_  _| |_
 *                 )(_)(  /(__)\  )( (_   _)(_   _)
 *                (_____)(__)(__)(__)  |_|    |_|
 *
 *
 * Copyright 2018-present, Leonid Stryzhevskyi <lganzzzo@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************************/

#ifndef oatpp_web_server_HttpConnectionHandler_hpp
#define oatpp_web_server_HttpConnectionHandler_hpp

#include "./HttpProcessor.hpp"
#include "./handler/ErrorHandler.hpp"
#include "./HttpRouter.hpp"

#include "oatpp/network/server/ConnectionHandler.hpp"
#include "oatpp/network/Connection.hpp"

namespace oatpp { namespace web { namespace server {

/**
 * Simple ConnectionHandler (&id:oatpp::network::server::ConnectionHandler;) for handling HTTP communication. <br>
 * Will create one thread per each connection to handle communication.
 */
class HttpConnectionHandler : public base::Countable, public network::server::ConnectionHandler {
private:
  std::shared_ptr<HttpProcessor::Components> m_components;
public:

  /**
   * Constructor.
   * @param components - &id:oatpp::web::server::HttpProcessor::Components;.
   */
  HttpConnectionHandler(const std::shared_ptr<HttpProcessor::Components>& components);

  /**
   * Constructor.
   * @param router - &id:oatpp::web::server::HttpRouter; to route incoming requests.
   */
  HttpConnectionHandler(const std::shared_ptr<HttpRouter>& router)
    : HttpConnectionHandler(std::make_shared<HttpProcessor::Components>(router))
  {}

  /**
   * Constructor.
   * @param router - &id:oatpp::web::server::HttpRouter; to route incoming requests.
   * @param config - &id:oatpp::web::server::HttpProcessor::Config;.
   */
  HttpConnectionHandler(const std::shared_ptr<HttpRouter>& router,
                        const std::shared_ptr<HttpProcessor::Config>& config)
    : HttpConnectionHandler(std::make_shared<HttpProcessor::Components>(router, config))
  {}

public:

  /**
   * Create shared HttpConnectionHandler.
   * @param router - &id:oatpp::web::server::HttpRouter; to route incoming requests.
   * @return - `std::shared_ptr` to HttpConnectionHandler.
   */
  static std::shared_ptr<HttpConnectionHandler> createShared(const std::shared_ptr<HttpRouter>& router);

  /**
   * Set root error handler for all requests coming through this Connection Handler.
   * All unhandled errors will be handled by this error handler.
   * @param errorHandler - &id:oatpp::web::server::handler::ErrorHandler;.
   */
  void setErrorHandler(const std::shared_ptr<handler::ErrorHandler>& errorHandler);

  /**
   * Set request interceptor. Request intercepted after route is resolved but before corresponding route endpoint is called.
   * @param interceptor - &id:oatpp::web::server::handler::RequestInterceptor;.
   */
  void addRequestInterceptor(const std::shared_ptr<handler::RequestInterceptor>& interceptor);

  /**
   * Implementation of &id:oatpp::network::server::ConnectionHandler::handleConnection;.
   * @param connection - &id:oatpp::data::stream::IOStream; representing connection.
   */
  void handleConnection(const std::shared_ptr<IOStream>& connection, const std::shared_ptr<const ParameterMap>& params) override;

  /**
   * Tell all worker threads to exit when done.
   */
  void stop() override;
  
};
  
}}}

#endif /* oatpp_web_server_HttpConnectionHandler_hpp */
