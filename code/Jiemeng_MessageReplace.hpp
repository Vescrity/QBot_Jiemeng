#ifndef JIEMENG_MESSAGEREPLACE
#define JIEMENG_MESSAGEREPLACE
#include <string>
namespace Jiemeng {
class Message;
void message_replace(std::string &str, const Message &place);
} // namespace Jiemeng

#endif
