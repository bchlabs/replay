#include "common.h"
#include <iostream>
#include <curl/curl.h>

static size_t ReplyCallback(void *ptr, size_t size, size_t nmemb, void *stream)
{
    std::string *str = (std::string*)stream;
    (*str).append((char*)ptr, size*nmemb);
    return size * nmemb;
}

bool CurlRequest(const std::string& request_url, const std::string&request_auth,  const std::string& request_data,  std::string& response)
{
    CURL *curl = curl_easy_init();
    struct curl_slist *headers = NULL;
    CURLcode res;
    response.clear();
    std::string error_str ;
    
    if (curl)
    {
        headers = curl_slist_append(headers, "content-type:application/json;");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, request_url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)request_data.size());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_data.c_str());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ReplyCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);

        curl_easy_setopt(curl, CURLOPT_USERPWD, request_auth.c_str());
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_TRY);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
        res = curl_easy_perform(curl);
    }
    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
    {
        error_str = curl_easy_strerror(res);
        return false;
    }
    return true;
}


std::string RpcRun(const std::string&url, const std::string& method, const std::string& params,const std::string& auth)
{
    std::string prefix = "{\"jsonrpc\": \"1.0\", \"id\":\"curltest\", \"method\": ";
    std::string infix = ", \"params\": [";
    std::string suffix = "]}";
    std::string rpc_data = prefix + "\"" +method+"\"" + infix + params + suffix;
    std::string rpc_response;
    CurlRequest(url, auth,  rpc_data, rpc_response);
    return rpc_response;
}


bool GetRawTransaction(const std::string &txid, const std::string &url, const std::string &auth, std::vector<std::string> &vins, std::string &hex)
{
    std::string method = "getrawtransaction";
    std::string params =  "\"" + txid + "\", 1";

    std::string response = RpcRun(url, method, params, auth);
    if (response == "" || response.empty())
        return false;

    json json_data = json::parse(response);
//    std::cout << response << std::endl;

    json res = json_data["result"];
    if (res.is_null())
        return false;
    
    json vin = res["vin"];   
    for (int i = 0; i < vin.size(); ++i)
    {
        vins.push_back(vin.at(i)["txid"].get<std::string>());
    }  
    hex = res["hex"]; 

    return true;
}
