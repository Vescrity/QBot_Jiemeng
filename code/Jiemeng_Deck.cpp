#include "Jiemeng_Deck.hpp"
#include "Jiemeng_Exception.hpp"
#include "Jiemeng_Random.hpp"
#include "Jiemeng_String.hpp"
#include <filesystem>
#include <fstream>

// TODO: 优化
namespace fs = std::filesystem;
namespace Jiemeng {

string Deck::list() {
    string rt;
    for (auto it = js.begin(); it != js.end(); ++it) {
        if (it.key()[0] != '_')
            rt += it.key() + "\n";
    }
    return rt;
}
string Deck::draw(const string &str, int times) {
    string rt;
    if (times > 20 || times <= 0)
        return "Invalid Times";
    for (int i = times; i > 0; i--)
        rt += get_output(string("{%") + str + "}") + (i > 1 ? "\n" : "");
    mp.clear();
    return rt;
}
Deck::Deck() {
    init("./deck");
}
void Deck::init(const string &folderPath) {
    debug_lable("[Deck]");
    dout << "开始从 " << folderPath << " 加载牌堆\n";
    json &mergedJSON = js;
    json jsonData;
    for (const auto &entry : fs::directory_iterator(folderPath)) {
        if (entry.path().extension() == ".json") {
            ifstream file(entry.path());
            if (file) {
                // json jsonData;
                file >> jsonData;
                file.close();
                for (auto it = jsonData.begin(); it != jsonData.end(); ++it)
                    mergedJSON[it.key()] = it.value();
            }
        }
    }
    debug_lable("[Deck]");
    debug_puts("牌堆加载完毕");
}
string Deck::get_output(string str) {
    if (get_st(str.c_str(), "::") == 0) {
        int q = get_st(str.c_str() + 2, "::");
        str = str.c_str() + q + 4;
    }
    string opt = "";
    int l, r;

    /// 由 `%` 而需用来调节偏移
    /// 同时用来控制是否重复抽取，含%表示可重复抽取，否则不重复抽取
    bool if_flg = 0;
    /// 获取复制起点
    l = get_st(str.c_str(), "{");
    string ss;
    while (l != -1) {
        if_flg = (str[l + 1] != '%');
        ss = str.substr(0, l);
        opt += ss;
        r = get_st(str.c_str() + l, "}") + l;
        l += 2 - if_flg;
        ss = str.substr(l, r - l);
        int rjs;
        int clc = clock();
        try {
            do {
                /// 抽取索引
                rjs = r_js(ss);
                if ((clock() - clc) * 1000.0 / CLOCKS_PER_SEC > 100)
                    return "TIME_OUT";
            } while (mp[ss][rjs]); // 检查是否允许抽取
            /// 标记是否允许再次抽取
            mp[ss][rjs] = if_flg;
            /// 递归式获取结果
            opt = opt + get_output(string(js[ss][rjs]));
        } catch (std::exception &e) { JM_EXCEPTION("[Deck]") }

        /// 向后移位
        str = str.c_str() + r + 1;
        l = get_st(str.c_str(), "{");
    }
    /// 合并余下部分
    return opt + str;
}

int Deck::r_js(const string &key) {
    int n = js[key].size();
    if (n == 0)
        throw invalid_argument(key + " not found.");
    string s;
    int n_lst[n + 1];
    memset(n_lst, 0, sizeof n_lst);
    int t;
    for (int i = 0; i < n; i++) {
        s = js[key][i];
        t = 1;
        if (get_st(s.c_str(), "::") == 0) {
            sscanf(s.c_str(), "::%d::", &t);
        }
        n_lst[i + 1] = n_lst[i] + t;
    }
    int r = Rands(0, n_lst[n] - 1);
    for (int i = 1; i <= n; i++) {
        if (n_lst[i] > r)
            return i - 1;
    }
    return n - 1;
}
} // namespace Jiemeng
