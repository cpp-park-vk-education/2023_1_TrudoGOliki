#pragma once

#include <optional>
#include <string>

struct Option {
    std::string_view flag;
    std::optional<std::string> argument;
};

struct Command {
    std::string_view cmd_title;
    std::optional<std::string> object;
};

constexpr std::string_view OPT0_HELP{"--help"};
constexpr std::string_view OPT0_UPDATE{"--update"};
constexpr std::string_view OPT0_STATUS{"--status"};

constexpr std::string_view OPT1_ADD{"--add"};
constexpr std::string_view OPT1_PATH{"--path"};
constexpr std::string_view OPT1_REMOVE{"--remove"};
constexpr std::string_view OPT1_MKDIR{"--mkdir"};
constexpr std::string_view OPT1_GETFILE{"--getfile"};
constexpr std::string_view OPT1_TIMETO{"--timeto"};
constexpr std::string_view OPT1_IP{"--ip"};

constexpr std::string_view CMD0_HELP{"help"};
constexpr std::string_view CMD0_ttt{"ttt"};
constexpr std::string_view CMD0_fff{"fff"};

constexpr std::string_view CMD1_LOOKFILE{"lookfile"};
constexpr std::string_view CMD1_SHAREFILE{"sharefile"};
constexpr std::string_view CMD1_HIDEFILE{"hidefile"};
constexpr std::string_view CMD1_STATUS{"status"};
