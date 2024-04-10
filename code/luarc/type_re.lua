function bot.type_reRecv(group_id,user_id,str)
  local msg = Message.new()
  msg:change(str)
  if(#group_id>4)then
    msg.place.group_id = group_id
    msg.place:set_group()
  else
    msg.place:set_private()
  end
  msg.place.user_id = user_id
  msg.place:get_level()
  bot.process_message(msg)
  return ''
end