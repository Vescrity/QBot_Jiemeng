function bot._api(url, api, is_Get, data_table, return_json)
    -- 
    local req = Request.new()
    req:set_url(url)
    req:set_api(api)
    local js = jsonlib.table2json(data_table)
    req:set_data(js)
    local rt = req:js_post()
    if (return_json) then return rt end
    return jsonlib.json2table(rt)
end

function bot.api(Data, return_json)
    --  @param table Data
    --  --  string url
    --  --  string api
    --  --  [bool get = false]
    --  --  table data
    --  @param [bool return_json=false]
    --  @return table
    local get = Data.get or false
    local rtj = return_json or false
    return bot._api(Data.url, Data.api, get, Data.data, rtj)
end
