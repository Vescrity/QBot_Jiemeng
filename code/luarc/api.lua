---
---@param url string
---@param api string
---@param is_Get boolean
---@param data_table table
---@param return_json boolean
---@return json|table
function bot._api(url, api, is_Get, data_table, return_json)
    -- 
    local req = Request.new()
    req:set_url(url)
    req:set_api(api)
    local js = jsonlib.table2json(data_table)
    req:set_data(js)
    local rt = json.new()
    if (is_Get) then
        rt = req:js_get()
    else
        rt = req:js_post()
    end
    if (return_json) then return rt end
    return jsonlib.json2table(rt)
end

---
---@param Data table
---@param return_json boolean (false)
---@return json|table
--- Data.url string  
--- Data.api string  
--- Data.data table  
function bot.api(Data, return_json)
    local get = Data.get or false
    local rtj = return_json or false
    return bot._api(Data.url, Data.api, get, Data.data, rtj)
end
