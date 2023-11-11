#ifndef BOT_SEND_CHECK
#define BOT_SEND_CHECK
class Bot_Send
{
private:
  int buf[10];
  int it;

public:
  Bot_Send() { it = 0; }
  void in(const int &id)
  {
    buf[it++] = id;
    if (it >= 9)
      it = 0;
  }
  bool check(const int &id) const
  {
    if (id == 0)
      return 1;
    for (auto i : buf)
    {
      if (id == i)
        return 0;
    }
    return 1;
  }
} bot_check;
#endif