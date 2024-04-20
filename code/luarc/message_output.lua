function bot.message_output(message)
  if(message:is_group()) then
    return bot.group_output(message.group_id,message:str())
  else
    return bot.private_output(message.user_id,message:str())
  end
end