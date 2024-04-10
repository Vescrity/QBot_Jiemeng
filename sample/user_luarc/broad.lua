function group_broad(str)
  for i,v in ipairs(broad_list)do 
    bot.group_output(v,str)
    bot.sleep(1111)
  end
  return ''
end