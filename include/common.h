#ifndef __COMMON_H__
#define __COMMON_H__

#include <string>
#include "json.hpp"

using json = nlohmann::json;

bool CurlRequest(const std::string& request_url, const std::string&request_auth,  const std::string& request_data,  std::string& response);

std::string RpcRun(const std::string&url, const std::string& method, const std::string& params,const std::string& auth);

bool GetRawTransaction(const std::string &txid, const std::string &url, const std::string &auth, std::vector<std::string> &vins, std::string &hex);

#endif
