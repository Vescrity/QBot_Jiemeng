local algo = {}

--- @param s1 string
--- @param s2 string
--- @param useUtf8 boolean?
--- @return integer length(单位取决于 useUtf8 参数)
function algo.LCS(s1, s2, useUtf8)
    useUtf8 = useUtf8 or false

    local getCodepoints

    if useUtf8 then
        assert(utf8, "UTF-8 mode requires Lua 5.3+ with the utf8 library.")
        getCodepoints = function(s)
            local codes = {}
            for _, codepoint in utf8.codes(s) do
                table.insert(codes, codepoint)
            end
            return codes
        end
    else
        getCodepoints = function(s)
            local codes = {}
            for _, byte_val in ipairs({ string.byte(s, 1, -1) }) do
                table.insert(codes, byte_val)
            end
            return codes
        end
    end

    local codes1 = getCodepoints(s1)
    local codes2 = getCodepoints(s2)
    local len1 = #codes1
    local len2 = #codes2

    if len1 == 0 or len2 == 0 then
        return 0
    end

    local dp = {}
    for j = 0, len2 do
        dp[j] = 0
    end

    local maxLength = 0

    for i = 1, len1 do
        local prev = 0
        for j = 1, len2 do
            local temp = dp[j]
            if codes1[i] == codes2[j] then
                dp[j] = prev + 1
                if dp[j] > maxLength then
                    maxLength = dp[j]
                end
            else
                dp[j] = 0
            end
            prev = temp
        end
    end

    return maxLength
end
return algo
