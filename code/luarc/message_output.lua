function message_output(message)
  if(message.is_group) then
    return group_output(message.group_id,message.text)
  else
    return private_output(message.user_id,message.text)
  end
end