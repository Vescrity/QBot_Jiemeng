#include "Jiemeng_CQJson.hpp"
#include "Jiemeng_String.hpp"
namespace Jiemeng {

std::string json2CQ(const json &message) {
    std::string cqCode;
    for (const auto &item : message) {
        std::string cqType = item["type"];
        json cqData = item["data"];
        if (cqType == "text") {
            std::string text = cqData["text"].is_null() ? "" : cqData["text"];
            str_replace(text, "&", "&amp;");
            str_replace(text, "[", "&#91;");
            str_replace(text, "]", "&#93;");
            cqCode += text;
            continue;
        }
        cqCode += "[CQ:" + cqType;
        for (auto it = cqData.begin(); it != cqData.end(); ++it) {
            std::string key = it.key();
            std::string value = it.value().is_string() ? string(it.value())
                                                       : to_string(it.value());
            str_replace(value, "&", "&amp;");
            str_replace(value, "[", "&#91;");
            str_replace(value, "]", "&#93;");
            str_replace(value, ",", "&#44;");
            cqCode += "," + key + "=" + value;
        }
        cqCode += "]";
    }
    return cqCode;
}
json CQ2json(const string &message) {
    json result;
    json temp;
    size_t endPos;
    size_t startPos = 0;
    string cqType;
    bool flg;
    do {
        json cqData;
        flg = 0;
        endPos = message.find("[CQ:", startPos);
        if (endPos == startPos) {
            flg = 1;
            endPos = message.find("]", startPos) + 1;
        }
        string cqCode = message.substr(startPos, endPos - startPos);
        if (flg) {
            size_t colonPos = cqCode.find(":");
            size_t fstPos = cqCode.find(",", colonPos);
            size_t feqPos, fedPos;
            string cqKey, value;
            if (fstPos == string::npos) {
                cqType = cqCode.substr(colonPos + 1, endPos - colonPos - 2);
                cqData = json::object();
                goto ret;
            }
            cqType = cqCode.substr(colonPos + 1, fstPos - colonPos - 1);
            do {
                feqPos = cqCode.find("=", fstPos);
                cqKey = cqCode.substr(fstPos + 1, feqPos - fstPos - 1);
                fstPos = cqCode.find(",", feqPos);
                fedPos = fstPos;
                fedPos =
                    (fedPos == string::npos) ? (cqCode.length() - 1) : fstPos;
                value = cqCode.substr(feqPos + 1, fedPos - feqPos - 1);
                str_replace(value, "&#91;", "[");
                str_replace(value, "&#93;", "]");
                str_replace(value, "&#44;", ",");
                str_replace(value, "&amp;", "&");
                cqData[cqKey] = value;
            } while (fstPos != string::npos);
        } else {
            cqType = "text";
            if (cqCode.length() == 0)
                continue;
            str_replace(cqCode, "&#91;", "[");
            str_replace(cqCode, "&#93;", "]");
            str_replace(cqCode, "&amp;", "&");
            cqData["text"] = cqCode;
        }
    ret:
        startPos = endPos;
        result.push_back({{"type", cqType}, {"data", cqData}});
    } while (endPos != string::npos);
    return result;
}

void CQMessage::generate_json() const {
    if (!string_ready)
        throw runtime_error("未初始化");
    js = CQ2json(cq);
    json_ready = 1;
}
void CQMessage::generate_string() const {
    if (!json_ready)
        throw runtime_error("未初始化");
    cq = json2CQ(js);
    string_ready = 1;
}
string CQMessage::true_str() const {
    if (no_true_str)
        return "";
    get_json();
    string rt;
    for (const auto &item : js) {
        std::string cqType = item["type"];
        if (cqType == "text")
            rt += item["data"]["text"];
    }
    return rt;
}

} // namespace Jiemeng
