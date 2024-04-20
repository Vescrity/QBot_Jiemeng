function bot.type_reRecv(group_id,user_id,str)
  local msg = Message.new()
  msg:change(str)
  if(#group_id>4)then
    msg.group_id = group_id
    msg:set_group()
  else
    msg:set_private()
  end
  msg.user_id = user_id
  msg:get_level()
  bot.process_message(msg)
  return ''
end