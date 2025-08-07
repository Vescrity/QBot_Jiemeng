#include "Jiemeng_Request.hpp"
#include "Jiemeng_String.hpp"
#include "Jiemeng_Basic.h"
#include "Jiemeng_IO.hpp"
#include <curl/curl.h>
#include <string>
#include <string_view>
#include <cctype>
#include <iomanip>
#include <sstream>


std::string url_encode(std::string_view input) {
    std::ostringstream encoded;
    encoded << std::hex;

    for (auto ch : input) {
        if (std::isalnum(static_cast<unsigned char>(ch)) || 
            ch == '-' || ch == '_' || ch == '.' || ch == '~') {
            encoded << ch;
        } else {
            encoded << '%' 
                   << std::setw(2) 
                   << static_cast<int>(static_cast<unsigned char>(ch));
        }
    }

    return encoded.str();
}

json Request::js_curl_get() {
    string ss = curl_get();
    try {
        json sjs = json::parse(ss);
        return sjs;
    } catch (...) { return json::parse("null"); }
}
json Request::js_curl_post() {
    string ss = curl_post();
    try {
        json sjs = json::parse(ss);
        return sjs;
    } catch (...) { return json::parse("null"); }
}
json Request::js_get() {
    string ss = Get();
    try {
        json sjs = json::parse(ss);
        return sjs;
    } catch (...) { return json::parse("null"); }
}
json Request::js_post() {
    string ss = Post();
    try {
        json sjs = json::parse(ss);
        return sjs;
    } catch (const std::exception &e) {
        std::string msg = "Exception caught: ";
        msg += e.what();
        error_lable("[Post]");
        error_puts(msg);
        error_lable("[Post]");
        error_print("接收到的内容: ");
        error_puts(ss);
        return json::parse("null");
    }
}
string Request::datas_urlencode() {
    string rt = "";
    int i = 0;
    for (auto &elem : data.items()) {
        rt = rt + ((i == 0) ? "?" : "&");
        string tmp = string(elem.key());
        rt = rt + url_encode(tmp) + "=";
        if (elem.value().is_number())
            tmp = std::to_string((long long)(elem.value()));
        else
            tmp = string(elem.value());
        rt = rt + url_encode(tmp);
        i++;
    }
    return rt;
}
string Request::curl_get() {
    string cmds = "curl -s \"";
    cmds = cmds + url_link() + datas_urlencode();
    // char *temps = new char[1 << 14];
    // memset(temps, 0, 1 << 14);
    cmds = cmds + "\"";
    return execmd(cmds);
}
//?
static size_t Wset(void *buf, size_t size, size_t nmemb, void *userp) {
    if (userp == nullptr)
        return 0;
    string *str = (string *)userp;
    size_t totalBytes = size * nmemb;
    str->append((char *)buf, totalBytes);
    return totalBytes;
}
// libcurl-Post
string Request::Post() {
    string rt;
    if (js_default) {
        add_Headers("Content-Type: application/json");
        msgs = to_string(data);
    }
    CURL *curl = curl_easy_init();
    if (!curl)
        throw std::runtime_error("CURL_INIT_ERROR");
    int nhds = Headers.size();
    curl_slist *headers = NULL;
    if (nhds) {
        for (int i = 0; i < nhds; i++) {
            headers = curl_slist_append(headers, Headers[i].c_str());
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }
    curl_easy_setopt(curl, CURLOPT_URL, url_link().c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, msgs.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Wset);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&rt);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    return rt;
}
FILE *Request::FPost(FILE *fp) {
    if (js_default) {
        add_Headers("Content-Type: application/json");
        msgs = to_string(data);
    }
    CURL *curl = curl_easy_init();
    if (!curl)
        throw std::runtime_error("CURL_INIT_ERROR");
    int nhds = Headers.size();
    curl_slist *headers = NULL;
    if (nhds) {
        for (int i = 0; i < nhds; i++) {
            headers = curl_slist_append(headers, Headers[i].c_str());
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }

    curl_easy_setopt(curl, CURLOPT_URL, url_link().c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, msgs.c_str());

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    return fp;
}
string Request::Get() {
    string rt;
    CURL *curl = curl_easy_init();
    if (!curl)
        throw std::runtime_error("CURL_INIT_ERROR");
    curl_easy_setopt(curl, CURLOPT_URL,
                     (url_link() + datas_urlencode()).c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Wset);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&rt);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return rt;
}
FILE *Request::FGet(FILE *fp) {
    CURL *curl = curl_easy_init();
    if (!curl)
        throw std::runtime_error("CURL_INIT_ERROR");
    curl_easy_setopt(curl, CURLOPT_URL,
                     (url_link() + datas_urlencode()).c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return fp;
}
string Request::curl_post() {
    if (js_default) {
        add_Headers("Content-Type: application/json");
    }
    string p;
    p = js_default ? to_string(data) : msgs;
    string len = std::to_string(p.length());
    msgs = escape_string(p);

    add_Headers("Content-Length: " + len);
    string cmds = "curl -s -X POST ";
    int nsize = Headers.size();
    for (int i = 0; i < nsize; i++)
        cmds = cmds + "-H \"" + Headers[i] + "\" ";
    cmds = cmds + "-d " + msgs + " \"" + url_link() + "\"";
    return execmd(cmds);
}
